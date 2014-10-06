#include "EventEditDialog.h"
#include "ui_EventEditDialog.h"
#include "EventButtonItem.h"
#include "Assets.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

struct ConditionItem : public QTreeWidgetItem
{
	ConditionItem(QTreeWidgetItem* parent) : QTreeWidgetItem(parent)
	{
	}

	Conditional* conditional;
};

EventEditDialog::EventEditDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::EventEditDialog)
{
	ui->setupUi(this);

	connect(ui->bCancel, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::CANCEL;

		close();
	});
	connect(ui->bSave, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::OK;

		// save results here

		close();
	});
	connect(ui->eventTree, &QTreeWidget::itemChanged, [this](QTreeWidgetItem* i, int column)
	{
		QLayoutItem* lchild;
		while ((lchild = ui->scrollArea->layout()->takeAt(0)) != nullptr)
			delete lchild->widget();

		if (column == 0)
			return;
		else if (column == 1)
		{
			ConditionItem* item = dynamic_cast<ConditionItem*>(i);
			for (size_t i = 0; i < item->conditional->blocks.size(); ++i)
			{
				QGroupBox* group = new QGroupBox(ui->scrollArea);
				group->setLayout(new QHBoxLayout());

				Conditional::ConditionPair& pair = item->conditional->blocks[i];
				group->setTitle(pair.condition->name);
				for (Param& param : pair.params)
				{
					group->layout()->addWidget(new QLabel(param.param->name, group));
					switch (param.param->type)
					{
						case EventSystem::ParamType::ASSET:
							{
								QComboBox* dropdown = new QComboBox(group);
								dropdown->setEditable(false);

								for (auto& asset : Assets::getTiles())
									dropdown->addItem(asset->path);
								for (auto& asset : Assets::getObjects())
									dropdown->addItem(asset->path);
								for (auto& asset : Assets::getEntities())
									dropdown->addItem(asset->path);

								int index = 0;
								for (int i = 0 ; i < dropdown->count(); ++i)
								{
									if (param.value == dropdown->itemText(i))
									{
										index = i;
										break;
									}
								}
								dropdown->setCurrentIndex(index);

								void(QComboBox:: *dropdownSignal)(int) = &QComboBox::currentIndexChanged;
								connect(dropdown, dropdownSignal, [&param, dropdown](int)
								{
									param.value = dropdown->currentText();
								});
								group->layout()->addWidget(dropdown);
							}
							break;
						case EventSystem::ParamType::COMPARISON:
							{
								QComboBox* dropdown = new QComboBox(group);
								dropdown->setEditable(false);
								dropdown->addItem("==");
								dropdown->addItem("!=");
								dropdown->addItem(">=");
								dropdown->addItem("<=");
								dropdown->addItem(">");
								dropdown->addItem("<");

								int index = 0;
								for (int i = 0 ; i < dropdown->count(); ++i)
								{
									if (param.value == dropdown->itemText(i))
									{
										index = i;
										break;
									}
								}
								dropdown->setCurrentIndex(index);

								void(QComboBox:: *dropdownSignal)(int) = &QComboBox::currentIndexChanged;
								connect(dropdown, dropdownSignal, [&param, dropdown](int)
								{
									param.value = dropdown->currentText();
								});
								group->layout()->addWidget(dropdown);
							}
							break;
						case EventSystem::ParamType::NUMBER:
							{
								QLineEdit* lineedit = new QLineEdit(group);
								lineedit->setValidator(new QIntValidator(lineedit));
								lineedit->setText(param.value);

								connect(lineedit, &QLineEdit::textChanged, [&param, lineedit](QString)
								{
									param.value = lineedit->text();
								});
								group->layout()->addWidget(lineedit);
							}
							break;
						case EventSystem::ParamType::OBJECT_ID:
							{
								// dont have object id's yet, add here
							}
							break;
					}
				}

				ui->scrollArea->layout()->addWidget(group);

				if (i < item->conditional->connectors.size())
				{
					QComboBox* dropdown = new QComboBox(ui->scrollArea);
					dropdown->setEditable(false);
					dropdown->addItem("AND");
					dropdown->addItem("OR");
					dropdown->setCurrentIndex(item->conditional->connectors[i] == "AND" ? 0 : 1);
					void(QComboBox:: *dropdownSignal)(int) = &QComboBox::currentIndexChanged;
					connect(dropdown, dropdownSignal, [item, dropdown, i](int)
					{
						item->conditional->connectors[i] = dropdown->currentText();
					});
					ui->scrollArea->layout()->addWidget(dropdown);
				}
			}
			QComboBox* dropdown = new QComboBox(ui->scrollArea);
			dropdown->setEditable(false);
			dropdown->addItem("+");
			for (const EventSystem::StaticCondition& condition : EventSystem::getSystem()->getConditions())
				dropdown->addItem(condition.name);
			dropdown->addItem("AND");
			dropdown->addItem("OR");

			void(QComboBox:: *dropdownSignal)(int) = &QComboBox::currentIndexChanged;
			connect(dropdown, dropdownSignal, [this, item, dropdown](int index)
			{
				if (index == 0)
					return;

				QComboBox* addbutton = dynamic_cast<QComboBox*>(ui->scrollArea->layout()->takeAt(ui->scrollArea->layout()->count() - 1)->widget());

				QGroupBox* group = new QGroupBox(ui->scrollArea);
				group->setLayout(new QHBoxLayout());
				item->conditional->blocks.push_back(Conditional::ConditionPair());
				Conditional::ConditionPair& pair = item->conditional->blocks[item->conditional->blocks.size() - 1];
				if (dropdown->currentText() == "AND" || dropdown->currentText() == "OR")
				{
					item->conditional->connectors.push_back(dropdown->currentText());
					int index = item->conditional->connectors.size() - 1;

					QComboBox* cond = new QComboBox(ui->scrollArea);
					cond->setEditable(false);
					cond->addItem("AND");
					cond->addItem("OR");
					cond->setCurrentIndex(dropdown->currentText() == "AND" ? 0 : 1);
					void(QComboBox:: *condSignal)(int) = &QComboBox::currentIndexChanged;
					connect(cond, condSignal, [item, cond, index](int)
					{
						item->conditional->connectors[index] = cond->currentText();
					});
					ui->scrollArea->layout()->addWidget(dropdown);

					pair.condition = &EventSystem::getSystem()->getConditions()[0]; // hardcoded
					pair.params.push_back({&pair.condition->params[0], "=="}); // hardcoded
					pair.params.push_back({&pair.condition->params[1], "0"}); // hardcoded
					QLineEdit* lineedit = new QLineEdit(group);
					lineedit->setValidator(new QIntValidator(lineedit));
					lineedit->setText("0"); // hardcoded

					connect(lineedit, &QLineEdit::textChanged, [this, &pair, lineedit](QString) // hardcoded params[1]
					{
						pair.params[1].value = lineedit->text(); // hardcoded params[1]
					});
					group->layout()->addWidget(lineedit);
				}
				else
				{
					// get the condition that the user selected, add its control to the scrollArea, add the actual condition block to the conditional, set values for both
				}
				ui->scrollArea->layout()->addWidget(group);
				ui->scrollArea->layout()->addWidget(addbutton);
			});

			ui->scrollArea->layout()->addWidget(dropdown);
		}
		else if (column == 2)
		{
			// set up the stuff for actions
		}
	});
}

EventEditDialog::~EventEditDialog()
{
	delete ui;
}

DialogResult EventEditDialog::showDialog(EventButtonItem* item)
{
	updateTree(item);

	return CSDialog::showDialog();
}

void EventEditDialog::updateTree(EventButtonItem* item)
{
	Event& event = item->getEvent();
	QTreeWidgetItem* root = ui->eventTree->topLevelItem(0);
	root->setText(0, event.event->name);

	for (Conditional& conditional : event.conditionals)
	{
		ConditionItem* child = new ConditionItem(root);
		child->conditional = &conditional;

		QString text;
		for (size_t i = 0; i < conditional.blocks.size(); ++i)
		{
			text += conditional.blocks[i].condition->name;
			if (i < conditional.connectors.size())
				text += " " + conditional.connectors[i] + " ";
			else
				text += " ";
		}
		child->setText(1, text);

		root->addChild(child);
	}
}
