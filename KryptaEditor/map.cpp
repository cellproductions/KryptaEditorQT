#include "Map.h"
#include "PrjSetupDialog.h"
#include "LayerOptionsItem.h"
#include "Resources.h"
#include "Utilities.h"
#include <System/Filesystem.h>
#include <Utilities/StringConvert.h>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <set>

struct TextElement
{
	QString name;
	QString text;
	QString attrVal;
};

std::shared_ptr<Map> Map::single;
QString Map::projectname;

void Map::resetMap()
{
    layers.clear();
    name.clear();
	currentLayer.reset();
}

std::shared_ptr<Map> Map::createMap(const QString& name, const Tile& defaulttile, QListWidget* layerList)
{
	single.reset(new Map(name, layerList->count()));

    for (int i = 0; i < layerList->count(); ++i)
    {
		if (layerList->item(i)->type() != LayerOptionsItem::LayerOptionType)
            continue;
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(layerList->item(i));
		Layer* layer = new Layer { std::move(std::vector<Tile>(option->getWidth() * option->getHeight(), defaulttile)), option->getDescription(),
									{option->getWidth(), option->getHeight()}, defaulttile.asset->resource->rawresource->getDimensions() };

        single->layers.emplace_back(layer);
    }

    layerList->clear();
	single->currentLayer = single->layers[0];
    return single;
}

TextElement readElement(QXmlStreamReader& reader)
{
	TextElement element;
	reader.readNext();
	element.name = reader.name().toString();
	if (reader.isEndElement())
		return element;
	if (reader.attributes().size() > 0)
		element.attrVal = reader.attributes()[0].value().toString();
	reader.readNext();
	element.text = reader.text().toString();
	reader.readNext();
	return element;
}

std::shared_ptr<Map> Map::loadFromFile(const QString& path)
{
	using namespace kry;

	QFile file(path);
	file.open(QIODevice::ReadOnly);
	if (!file.isOpen())
		throw kry::Util::Exception("Failed to open file! [path=" + qToKString(path) + ']', KRY_EXCEPTION_DATA);

	TextElement element;
	QXmlStreamReader reader(&file);
	reader.readNext();

	reader.readNext(); // projectfile
		reader.readNext();
		reader.readNext(); // project
			reader.readNext();
			element = readElement(reader);
			QString name = element.text;
			reader.readNext();
			element = readElement(reader);
			size_t layercount = element.text.toUInt();
			single.reset(new Map(name, layercount));
			reader.readNext();
			reader.readNext();
		reader.readNext();
		reader.readNext(); // layers
			for (size_t i = 0; i < layercount; ++i)
			{
				reader.readNext();
				reader.readNext(); // layer
					reader.readNext();
					element = readElement(reader);
					auto description = element.text;
					reader.readNext();
					element = readElement(reader);
					int width = element.text.toInt();
					reader.readNext();
					element = readElement(reader);
					int height = element.text.toInt();
					reader.readNext();
					element = readElement(reader);
					int tilewidth = element.text.toInt();
					reader.readNext();
					element = readElement(reader);
					int tileheight = element.text.toInt();
					Layer* layer = new Layer { std::move(std::vector<Tile>(width * height, Tile())), description, {width, height}, {tilewidth, tileheight}};
					single->layers.emplace_back(layer);
					reader.readNext();
					reader.readNext();
			}
			reader.readNext();
			reader.readNext();
		reader.readNext();
		reader.readNext(); // tiledata
			for (size_t i = 0; i < layercount; ++i)
			{
				std::shared_ptr<Layer>& layer = single->layers[i];

				for (size_t j = 0; j < layer->tiles.size(); ++j)
				{
					Tile& tile = layer->tiles[j];
					reader.readNext();
					reader.readNext(); // tile
						reader.readNext(); // layerid
						element = readElement(reader);
						reader.readNext();
						element = readElement(reader);
						tile.asset = Assets::getTileByIni(element.text).get();
						reader.readNext();
						element = readElement(reader);
						size_t objectcount = element.text.toUInt();
						tile.objects.reserve(objectcount == 0 ? 1 : objectcount);
						reader.readNext();
						reader.readNext(); // properties (IGNORED FOR NOW. MUST BE IMPLEMENTED)
						reader.readNext();
						reader.readNext();
						reader.readNext(); // objects
						if (objectcount == 0)
							reader.readNext();
						else
						{
							for (size_t n = 0; n < objectcount; ++n)
							{
								reader.readNext();
								reader.readNext(); // object
									Object* object = new Object;

									reader.readNext();
									element = readElement(reader); // tileid
									reader.readNext();
									element = readElement(reader);
									auto assetpath = element.text;
									if (assetpath.startsWith("assets\\objects"))
										object->asset = Assets::getObjectByIni(assetpath).get();
									else if (assetpath.startsWith("assets\\entities"))
										object->asset = Assets::getEntityByIni(assetpath).get();
									reader.readNext();
									reader.readNext(); // properties
									int sections = reader.attributes()[0].value().toInt();
									object->properties = object->asset->properties;
										for (int section = 0; section < sections; ++section)
										{
											reader.readNext();
											reader.readNext(); // section
											int keys = reader.attributes()[0].value().toInt();
											kry::Util::String strsection = qToKString(reader.name().toString());
											for (int key = 0; key < keys; ++key)
											{
												reader.readNext();
												element = readElement(reader); // key
												kry::Util::String strkey = qToKString(element.name);
												object->properties[strsection][strkey] = qToKString(element.text);
											}
											reader.readNext();
											reader.readNext();
										}
										reader.readNext();
										reader.readNext();
									object->sprite.texture = object->asset->resource->rawresource;
									object->sprite.dimensions = object->asset->resource->rawresource->getDimensions();
									if (assetpath.startsWith("assets\\objects"))
										object->sprite.position = {kry::Util::toDecimal<float>(object->properties["object"]["posx"]),
																  kry::Util::toDecimal<float>(object->properties["object"]["posy"])};
									else if (assetpath.startsWith("assets\\entities"))
										object->sprite.position = {kry::Util::toDecimal<float>(object->properties["entity"]["posx"]),
																  kry::Util::toDecimal<float>(object->properties["entity"]["posy"])};
									tile.objects.emplace_back(object);
									reader.readNext();
									reader.readNext();
							}
							reader.readNext();
							reader.readNext();
						}
						reader.readNext();
						reader.readNext();
				}
			}
	file.close();

	single->currentLayer = single->layers[0];
	return single;
}

void Map::saveToFile(const QString& name)
{
	using namespace kry;

	QFile file(name);
	file.open(QIODevice::WriteOnly);

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();

	writer.writeStartElement("projectfile");
	{
		writer.writeStartElement("project");
		{
			writer.writeTextElement("name", single->getName());
			writer.writeTextElement("layercount", QString::number(single->layers.size()));
		}
		writer.writeEndElement();
		writer.writeStartElement("layers");
		{
			for (size_t i = 0; i < single->layers.size(); ++i)
			{
				std::shared_ptr<Layer>& layer = single->layers[i];
				writer.writeStartElement("layer");
				writer.writeAttribute("id", QString::number(i));
				{
					writer.writeTextElement("description", layer->description);
					writer.writeTextElement("width", QString::number(layer->size[0]));
					writer.writeTextElement("height", QString::number(layer->size[1]));
					writer.writeTextElement("tilewidth", QString::number(layer->tilesize[0]));
					writer.writeTextElement("tileheight", QString::number(layer->tilesize[1]));
				}
				writer.writeEndElement();
			}
		}
		writer.writeEndElement();
		writer.writeStartElement("tiledata");
		{
			for (size_t i = 0; i < single->layers.size(); ++i)
			{
				std::shared_ptr<Layer>& layer = single->layers[i];

				for (size_t j = 0; j < layer->tiles.size(); ++j)
				{
					Tile& tile = layer->tiles[j];

					writer.writeStartElement("tile");
					writer.writeAttribute("id", QString::number(j));
					{
						writer.writeTextElement("layerid", QString::number(i));
						writer.writeTextElement("asset", tile.asset->path);
						writer.writeTextElement("objectcount", QString::number(tile.objects.size()));
						writer.writeStartElement("properties");
						writer.writeAttribute("sections", QString::number(tile.properties.getSectionNames().size() == 0 ? 0 : tile.properties.getSectionNames().size() - 1));
						{
							for (auto& section : tile.properties.getSectionNames())
							{
								if (section.isEmpty())
									continue;
								writer.writeStartElement(kryToQString(section));
								writer.writeAttribute("keys", QString::number(tile.properties[section].getKeyNames().size()));
								{
									for (auto& key : tile.properties[section].getKeyNames())
										writer.writeTextElement(kryToQString(key), kryToQString(tile.properties[section][key]));
								}
								writer.writeEndElement();
							}
						}
						writer.writeEndElement();
						writer.writeStartElement("objects");
						{
							for (size_t n = 0; n < tile.objects.size(); ++n)
							{
								std::shared_ptr<Object>& object = tile.objects[n];
								writer.writeStartElement("object");
								writer.writeAttribute("id", QString::number(n));
								{
									writer.writeTextElement("tileid", QString::number(j));
									writer.writeTextElement("asset", object->asset->path);
									writer.writeStartElement("properties");
									writer.writeAttribute("sections", QString::number(object->properties.getSectionNames().size() == 0 ? 0 : object->properties.getSectionNames().size() - 1));
									{
										for (auto& section : object->properties.getSectionNames())
										{
											if (section.isEmpty())
												continue;
											writer.writeStartElement(kryToQString(section));
											writer.writeAttribute("keys", QString::number(object->properties[section].getKeyNames().size()));
											{
												for (auto& key : object->properties[section].getKeyNames())
													writer.writeTextElement(kryToQString(key), kryToQString(object->properties[section][key]));
											}
											writer.writeEndElement();
										}
									}
									writer.writeEndElement();
								}
								writer.writeEndElement();
							}
						}
						writer.writeEndElement();
					}
					writer.writeEndElement();
				}
			}
		}
		writer.writeEndElement();
	}
	writer.writeEndElement();

	writer.writeEndDocument();

	file.close();
}

template <typename TypeAsset, typename TypeContainer>
size_t getAssetIndex(TypeAsset* asset, const TypeContainer& container)
{
	size_t index = 0;
	for ( ; index < container.size(); ++index)
		if (container[index].get() == asset)
			return index;
	return index;
}

void Map::exportToFile(const QString& name, kry::Media::Config& prjconfig)
{
	try
	{
		auto kname = qToKString(name);
		if (!kry::System::pathExists(kname))
			kry::System::createPath(kname);
		kname += "\\data";
		if (!kry::System::pathExists(kname))
			kry::System::createPath(kname);
		if (!kry::System::pathExists(kname + "\\images"))
			kry::System::createPath(kname + "\\images");

		std::vector<kry::Util::String> lines;

		lines.push_back("[Settings]");
		lines.push_back("name = " + qToKString(single->getName()));
		lines.push_back("iconImage = icon.png");
		lines.push_back("checksum = 0");
		lines.push_back("fogOfWar = 1");
		lines.push_back("playerSpawnFloor = " + prjconfig["player"]["layer"]);
		lines.push_back("playerSpawnPosition = { " + prjconfig["player"]["tilex"] + ", " + prjconfig["player"]["tiley"] + " }");
		lines.push_back("; just for prototype");
		lines.push_back("[Objectives]");
		lines.push_back("objective1=0");
		lines.push_back("[objective1]");
		lines.push_back("type = loot");
		lines.push_back("minimumLootRequired = 100");
		lines.push_back("[Floors]");
		for (size_t i = 0; i < single->getLayers().size(); ++i)
		{
			auto str = kry::Util::toString(i);
			lines.push_back("floor" + str + '=' + str);
		}
		std::set<Asset<kry::Graphics::Texture>*> tilesused;
		for (size_t i = 0; i < single->getLayers().size(); ++i)
		{
			std::shared_ptr<Layer>& layer = single->getLayers()[i];
			lines.push_back("[floor" + kry::Util::toString(i) + ']');
			lines.push_back("dimensions={ " + kry::Util::toString(layer->size[0]) + ", " + kry::Util::toString(layer->size[1]) + " }");

			for (size_t j = 0; j < layer->tiles.size(); ++j)
				tilesused.insert(layer->tiles[j].asset);
		}

		lines.push_back("[Tiles]"); // only one floor supported for now
		unsigned index = 0;
		for (Asset<kry::Graphics::Texture>* asset : tilesused)
			lines.push_back("tile" + kry::Util::toString(index++) + '=' + kry::Util::toString(getAssetIndex(asset, Assets::getTiles())));
		index = 0;
		for (Asset<kry::Graphics::Texture>* asset : tilesused)
		{
			lines.push_back("[tile" + kry::Util::toString(index++) + ']');
			lines.push_back("type=void");
			lines.push_back("skinConfig=data/TileSkins.txt");
			auto skin = qToKString(asset->resource->name);
			lines.push_back("skin=" + skin.substring(0, skin.indexOf('.')));
			lines.push_back("heuristic = 1");
		}

		lines.push_back("[Entities]"); // no entities supported for now

		kry::System::writeAllLines(kname + '\\' + qToKString(name) + ".txt", lines);
		lines.clear();

		for (size_t y = 0; y < single->getLayers()[0]->size[1]; ++y) // only one floor supported for now
		{
			kry::Util::String line;
			for (size_t x = 0; x < single->getLayers()[0]->size[0]; ++x)
			{
				size_t index = y * single->getLayers()[0]->size[0] + x;
				line += kry::Util::toString(getAssetIndex(single->getLayers()[0]->tiles[index].asset, Assets::getTiles())) + ' ';
			}
			line = line.substring(0, line.getLength() - 1);
			lines.push_back(line);
		}
		kry::System::writeAllLines(kname + '\\' + "floor1.txt", lines); /** #TODO(change) change all of these .txt types to something much more representitive of what they hold */
		lines.clear();

		lines.push_back("[Skins]");
		for (Asset<kry::Graphics::Texture>* asset : tilesused)
		{
			auto skin = qToKString(asset->resource->name);
			lines.push_back(skin.substring(0, skin.indexOf('.')) + '=');
		}
		for (Asset<kry::Graphics::Texture>* asset : tilesused)
		{
			auto skin = qToKString(asset->resource->name);
			lines.push_back('[' + skin.substring(0, skin.indexOf('.')) + ']');
			lines.push_back("frames=1"); // only 1 frame supported for now
			lines.push_back("fps=1");
			lines.push_back("sheetDimensions = { 1, 1 } ");
			lines.push_back("sheetImage = data/images/" + skin);
			lines.push_back("framePivot= { " + kry::Util::toString(asset->resource->rawresource->getDimensions()[0] / 2) + ", " +
					kry::Util::toString(asset->resource->rawresource->getDimensions()[1] / 2) + " }");
		}
		kry::System::writeAllLines(kname + '\\' + "TileSkins.txt", lines);

		for (Asset<kry::Graphics::Texture>* asset : tilesused)
			kry::System::copyFile(qToKString(asset->resource->path), kname + "\\images\\" + qToKString(asset->resource->name));
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Export Failure", e.what(), QMessageBox::Ok);
	}
}

std::shared_ptr<Map> Map::getMap()
{
    return single;
}

Map::Map(const QString& name, size_t layerCount) : name(name)
{
    layers.reserve(layerCount);
}
