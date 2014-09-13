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
#include <QDebug>

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

std::shared_ptr<Map> Map::getMap()
{
    return single;
}

Map::Map(const QString& name, size_t layerCount) : name(name)
{
    layers.reserve(layerCount);
}
