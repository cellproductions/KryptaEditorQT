#include "AudioManagerDialog.h"
#include "ui_AudioManagerDialog.h"
#include "Resources.h"
#include "Assets.h"
#include <Media\AudioFactory.h>
#include <Audio\Audio.h>
#include <Process\Thread.h>
#include "Utilities.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <Windows.h>

AudioManagerDialog::AudioManagerDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::AudioManagerDialog)
{
	ui->setupUi(this);
	kry::Audio::DeviceContext::createDeviceContext();
	kry::Audio::Listener::createListener();
	
	void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
	connect(ui->cbAudio, animsSignal, [this](int index)
	{
		if (index < 0)
		{
			while (ui->tAudioProps->rowCount() > 0)
				ui->tAudioProps->removeRow(0);
			return;
		}

		while (ui->tAudioProps->rowCount() > 0)
			ui->tAudioProps->removeRow(0);
		if (index >= Assets::getSounds().size()) // its music
		{
			int i = index - Assets::getSounds().size();
			auto asset = Assets::getMusic()[i];
			for (auto& key : asset->properties["global"].getKeyNames())
			{
				int index = ui->tAudioProps->rowCount();
				ui->tAudioProps->insertRow(index);
				ui->tAudioProps->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
				ui->tAudioProps->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				ui->tAudioProps->setItem(index, 1, new QTableWidgetItem(kryToQString(asset->properties["global"][key])));
				ui->tAudioProps->item(index, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			}
		}
		else // its a sound
		{
			auto asset = Assets::getSounds()[index];
			for (auto& key : asset->properties["global"].getKeyNames())
			{
				int index = ui->tAudioProps->rowCount();
				ui->tAudioProps->insertRow(index);
				ui->tAudioProps->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
				ui->tAudioProps->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				ui->tAudioProps->setItem(index, 1, new QTableWidgetItem(kryToQString(asset->properties["global"][key])));
				ui->tAudioProps->item(index, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			}
		}
		
		lastindex = index;
	});
	connect(ui->bPlay, &QPushButton::clicked, [this](bool)
	{
		if (lastindex < 0)
			return;
		
		static const unsigned WAIT_TIME = 3000;
		ui->bPlay->setEnabled(false);
		if (lastindex >= Assets::getSounds().size()) // its music
		{
			int index = lastindex - Assets::getSounds().size();
			auto asset = Assets::getMusic()[index];
			auto path = qToKString(asset->resource->path);
			kry::Audio::Source source;
			kry::Process::Thread* thread = new kry::Process::Thread([&source, &path]()
			{
				kry::Media::mp3FileStream(path, source);
			});
			Sleep(10);
			unsigned timer = 0;
			while (timer < WAIT_TIME)
			{
				Sleep(10);
				timer += 10;
			}
			try
			{
				if (source.getStatus() == kry::Audio::Status::PLAYING)
					source.stop();
				thread->join();
				delete thread;
			}
			catch (const kry::Util::Exception&) /** #TODO(note) fugly hack, stops the thread from throwing, but doesnt close it nicely i dont think */
			{
			}
		}
		else // its a sound
		{
			auto asset = Assets::getSounds()[lastindex];
			kry::Audio::Source* source = new kry::Audio::Source(*asset->resource->rawresource);
			source->play();
			unsigned timer = 0;
			while (timer < WAIT_TIME && source->getStatus() == kry::Audio::Status::PLAYING)
			{
				Sleep(10);
				timer += 10;
			}
			delete source;
		}
		ui->bPlay->setEnabled(true);
	});
	connect(ui->bAdd, &QPushButton::clicked, [this](bool)
	{
		QString file = QFileDialog::getOpenFileName(this, "Create New Audio", QApplication::applicationDirPath() + "\\assets\\resources\\audio", "Audio files (*.mp3)");
		if (!file.isNull())
		{
			QInputDialog nameinput(this);
			nameinput.setWindowTitle("Create New Audio");
			nameinput.setLabelText("Enter a name for the audio:");
			nameinput.exec();
			QString name = nameinput.textValue();
			if (name.trimmed().isEmpty() || nameinput.result() == QDialog::Rejected)
				return;

			QString type = QInputDialog::getItem(this, "Create New Audio", "Select a type for your audio:", {"Sound", "Music"}, 0, false);
			if (type.isNull() || type.isEmpty())
				return;
			if (type == "Sound")
			{
				typedef kry::Audio::Buffer Buffer;
				Resource<Buffer>* resource = new Resource<Buffer>;
				resource->name = name;
				resource->path = file;
				resource->type = ResourceType::SOUND;
				kry::Audio::Format format;
				unsigned frequency;
				auto pcm = kry::Media::mp3FileToPCM(qToKString(file), format, frequency);
				resource->rawresource = new Buffer(pcm, format, frequency);

				Asset<Buffer>* asset = new Asset<Buffer>;
				asset->path;
				asset->path = file;
				asset->resource = resource;
				asset->type = AssetType::SOUND;
				asset->properties["global"]["name"] = qToKString(name);
				asset->properties["global"]["resource"] = qToKString(file);
				asset->properties["global"]["type"] = "SOUND";

				int index = Assets::getSounds().size();
				ui->cbAudio->insertItem(index, QString::number(index) + ':' + name);
				const_cast<std::vector<std::shared_ptr<Asset<Buffer>>>&>(Assets::getSounds()).emplace_back(asset);
				const_cast<std::vector<std::shared_ptr<Resource<Buffer>>>&>(Resources::getSounds()).emplace_back(resource);

				ui->cbAudio->setCurrentIndex(index);
			}
			else
			{
				typedef kry::Audio::Source Source;
				Resource<Source>* resource = new Resource<Source>;
				resource->name = name;
				resource->path = file;
				resource->type = ResourceType::MUSIC;
				resource->rawresource = nullptr;

				Asset<Source>* asset = new Asset<Source>;
				asset->path;
				asset->path = file;
				asset->resource = resource;
				asset->type = AssetType::MUSIC;
				asset->properties["global"]["name"] = qToKString(name);
				asset->properties["global"]["resource"] = qToKString(file);
				asset->properties["global"]["type"] = "MUSIC";
				
				int index = Assets::getMusic().size();
				ui->cbAudio->addItem(QString::number(index) + ':' + name);
				const_cast<std::vector<std::shared_ptr<Asset<Source>>>&>(Assets::getMusic()).emplace_back(asset);
				const_cast<std::vector<std::shared_ptr<Resource<Source>>>&>(Resources::getMusic()).emplace_back(resource);

				ui->cbAudio->setCurrentIndex(index + Assets::getSounds().size());
			}
		}
		if (ui->cbAudio->count() > 0)
			ui->bDelete->setEnabled(true);
	});
	connect(ui->bDelete, &QPushButton::clicked, [this](bool)
	{
		if (lastindex < 0)
			return;
		
		if (lastindex >= Assets::getSounds().size()) // its music
		{
			int index = lastindex = Assets::getSounds().size();
			typedef kry::Audio::Source Source;
			const_cast<std::vector<std::shared_ptr<Resource<Source>>>&>(Resources::getMusic()).erase(const_cast<std::vector<std::shared_ptr<Resource<Source>>>&>(Resources::getMusic()).begin() + index);
			const_cast<std::vector<std::shared_ptr<Asset<Source>>>&>(Assets::getMusic()).erase(const_cast<std::vector<std::shared_ptr<Asset<Source>>>&>(Assets::getMusic()).begin() + index);
		}
		else // its a sound
		{
			typedef kry::Audio::Buffer Buffer;
			const_cast<std::vector<std::shared_ptr<Resource<Buffer>>>&>(Resources::getSounds()).erase(const_cast<std::vector<std::shared_ptr<Resource<Buffer>>>&>(Resources::getSounds()).begin() + lastindex);
			const_cast<std::vector<std::shared_ptr<Asset<Buffer>>>&>(Assets::getSounds()).erase(const_cast<std::vector<std::shared_ptr<Asset<Buffer>>>&>(Assets::getSounds()).begin() + lastindex);
		}

		setupDropdown();
		if (ui->cbAudio->count() <= 0)
			ui->bDelete->setEnabled(false);
	});
	connect(ui->bDone, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::OK;
		close();
	});
}

AudioManagerDialog::~AudioManagerDialog()
{
	delete ui;
}

DialogResult AudioManagerDialog::showDialog()
{
	lastindex = -1;
	setupDropdown();
	if (ui->cbAudio->count() <= 0)
		ui->bDelete->setEnabled(false);

	this->exec();

	return lastresult;
}

void AudioManagerDialog::setupDropdown()
{
	ui->cbAudio->clear();
	unsigned index = 0;
	for (auto sound : Assets::getSounds())
		ui->cbAudio->addItem(QString::number(index++) + ':' + kryToQString(sound->properties["global"]["name"]));
	index = 0;
	for (auto music : Assets::getMusic())
		ui->cbAudio->addItem(QString::number(index++) + ':' + kryToQString(music->properties["global"]["name"]));
}
