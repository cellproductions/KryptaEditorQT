#include "Map.h"
#include "PrjSetupDialog.h"
#include "LayerOptionsItem.h"
#include "Resources.h"
#include "Utilities.h"
#include "GLPanel.h"
#include <System/Filesystem.h>
#include <Utilities/StringConvert.h>
#include <Media/Zip.h>
#include <QJSONDocument>
#include <QJSONObject>
#include <QJSONArray>
#include <QJSONValue>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <set>
#include <fstream>

unsigned Object::increment(0);

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
									{option->getWidth(), option->getHeight()}, defaulttile.asset->resource->rawresource->getDimensions(), static_cast<unsigned>(i) };
		unsigned resourceindex = 0;
		for (auto& resource : Resources::getAnimations())
		{
			if (resource.get() == defaulttile.asset->resource)
				break;
			++resourceindex;
		}
		unsigned index = 0;
		for (Tile& tile : layer->tiles)
		{
			tile.properties = defaulttile.asset->properties;
			tile.properties["global"]["id"] = kry::Util::toString(static_cast<unsigned>(i) * 10000u + (index++));
			auto type = tile.properties["global"]["hardtype"];
			for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["floor"].getKeyNames())
				tile.hardproperties["floor"][key] = "";
			for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
				tile.hardproperties[type][key] = "";
			tile.hardproperties["floor"]["skin"] = kry::Util::toString(resourceindex);
		}

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

std::shared_ptr<Map> Map::loadFromFile(const QString& path, kry::Media::Config& prjsettings) /** #TODO(change) whole thing will need a readover. things have changed */
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
		reader.readNext(); // player
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["layer"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["tilex"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["tiley"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["dimensionsx"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["dimensionsy"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["seeInFog"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["directions"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["maxHeuristic"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["viewDistance"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["moveAcceleration"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["turnAcceleration"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["maxMoveSpeed"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["maxTurnSpeed"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["skinIdle"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["skinRun"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["skinDead"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["health"] = qToKString(element.text);
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
					Layer* layer = new Layer { std::move(std::vector<Tile>(width * height, Tile())), description, {width, height}, {tilewidth, tileheight}, i};
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
									if (assetpath.startsWith("assets\\entities"))
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

void Map::saveToFile(const QString& name, kry::Media::Config& prjsettings) /** #TODO(note) when saving project settings, must save the objectidindex as well so id's can continue to be generated from where they were left off */
{																		/** #TODO(note) same with item id count */
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
		writer.writeStartElement("player");
		{
			writer.writeTextElement("layer", kryToQString(prjsettings["player"]["layer"]));
			writer.writeTextElement("tilex", kryToQString(prjsettings["player"]["tilex"]));
			writer.writeTextElement("tiley", kryToQString(prjsettings["player"]["tiley"]));
			writer.writeTextElement("dimensionsx", kryToQString(prjsettings["player"]["dimensionsx"]));
			writer.writeTextElement("dimensionsy", kryToQString(prjsettings["player"]["dimensionsy"]));
			writer.writeTextElement("seeInFog", kryToQString(prjsettings["player"]["seeInFog"]));
			writer.writeTextElement("directions", kryToQString(prjsettings["player"]["directions"]));
			writer.writeTextElement("maxHeuristic", kryToQString(prjsettings["player"]["maxHeuristic"]));
			writer.writeTextElement("viewDistance", kryToQString(prjsettings["player"]["viewDistance"]));
			writer.writeTextElement("moveAcceleration", kryToQString(prjsettings["player"]["moveAcceleration"]));
			writer.writeTextElement("turnAcceleration", kryToQString(prjsettings["player"]["turnAcceleration"]));
			writer.writeTextElement("maxMoveSpeed", kryToQString(prjsettings["player"]["maxMoveSpeed"]));
			writer.writeTextElement("maxTurnSpeed", kryToQString(prjsettings["player"]["maxTurnSpeed"]));
			writer.writeTextElement("skinIdle", kryToQString(prjsettings["player"]["skinIdle"]));
			writer.writeTextElement("skinRun", kryToQString(prjsettings["player"]["skinRun"]));
			writer.writeTextElement("skinDead", kryToQString(prjsettings["player"]["skinDead"]));
			writer.writeTextElement("health", kryToQString(prjsettings["player"]["health"]));
		}
		writer.writeEndElement();
		writer.writeStartElement("layers");
		{
			for (size_t i = 0; i < single->layers.size(); ++i)
			{
				std::shared_ptr<Layer>& layer = single->layers[i];
				writer.writeStartElement("layer");
				writer.writeAttribute("id", QString::number(layer->index));
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
					writer.writeAttribute("id", kryToQString(tile.properties["global"]["id"]));
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
						writer.writeStartElement("hardproperties");
						writer.writeAttribute("sections", QString::number(tile.hardproperties.getSectionNames().size() == 0 ? 0 : tile.hardproperties.getSectionNames().size() - 1));
						{
							for (auto& section : tile.hardproperties.getSectionNames())
							{
								if (section.isEmpty())
									continue;
								writer.writeStartElement(kryToQString(section));
								writer.writeAttribute("keys", QString::number(tile.hardproperties[section].getKeyNames().size()));
								{
									for (auto& key : tile.hardproperties[section].getKeyNames())
										writer.writeTextElement(kryToQString(key), kryToQString(tile.hardproperties[section][key]));
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
								writer.writeAttribute("id", kryToQString(object->properties["global"]["id"]));
								{
									writer.writeTextElement("tileid", kryToQString(tile.properties["global"]["id"]));
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
									writer.writeStartElement("hardproperties");
									writer.writeAttribute("sections", QString::number(object->hardproperties.getSectionNames().size() == 0 ? 0 : object->hardproperties.getSectionNames().size() - 1));
									{
										for (auto& section : object->hardproperties.getSectionNames())
										{
											if (section.isEmpty())
												continue;
											writer.writeStartElement(kryToQString(section));
											writer.writeAttribute("keys", QString::number(object->hardproperties[section].getKeyNames().size()));
											{
												for (auto& key : object->hardproperties[section].getKeyNames())
													writer.writeTextElement(kryToQString(key), kryToQString(object->hardproperties[section][key]));
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
	return ~0;
}

size_t getAnimIndex(Resource<kry::Graphics::Texture>* anim)
{
	size_t index = 0;
	for ( ; index < Resources::getAnimations().size(); ++index)
		if (Resources::getAnimations()[index].get() == anim)
			return index;
	return ~0;
}

kry::Util::Vector2f coordToTileCoord(const kry::Util::Vector2f& coord)
{
	kry::Util::Vector2i dim = Map::getMap()->getCurrentLayer()->tiles[0].asset->resource->rawresource->getDimensions();
	kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};
//	auto pos = coord + halfdim;

	float x = (coord[0] / halfdim[0] + coord[1] / halfdim[1]) * 0.5f;  // black magic goin on here?
	float y = (coord[1] / halfdim[1] - coord[0] / halfdim[0]) * 0.5f * -1;

	return {x, y};
}

std::vector<unsigned char> linesToBin(const std::vector<kry::Util::String>& lines)
{
	std::vector<unsigned char> data;

	for (auto& line : lines)
	{
		for (unsigned i = 0; i < line.getLength(); ++i)
			data.push_back(static_cast<unsigned char>(line[i]));
		data.push_back('\r');
		data.push_back('\n');
	}

	return data;
}

std::vector<unsigned char> fileToBin(const QString& filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	auto bytes = file.readAll();
	std::vector<unsigned char> data(bytes.begin(), bytes.end());

	return data;
}

kry::Util::String boolToKBool(const kry::Util::String& boolean) /** #TODO(note) use this to convert "true/false" to "1/0" */
{
	return boolean == "true" ? kry::Util::String("1") : kry::Util::String("0");
}

struct ExpTile
{
	Object* tile;
	Object* wall;
};

bool operator<(const ExpTile& left, const ExpTile& right)
{
	return left.tile < right.tile && left.wall < right.wall;
}

bool operator==(const ExpTile& left, const ExpTile& right)
{
	if (left.wall != nullptr || right.wall != nullptr)
		return left.wall == right.wall;
	return left.tile == right.tile;
}

struct UsedAnim
{
	std::shared_ptr<Animation<>> animation;
	kry::Util::String skinfile;
	unsigned directions;
	size_t animid;
};

bool operator<(const UsedAnim& left, const UsedAnim& right)
{
	return left.animid < right.animid && left.animation.get() < right.animation.get() && left.directions < right.directions;
}

bool operator==(const UsedAnim& left, const UsedAnim& right)
{
	return left.animid == right.animid && left.directions == right.directions;
}

void Map::exportToFile(const QString& name, kry::Media::Config& prjconfig) /** #TODO(note) if a key is empty, dont export it (let the game crash or print error, etc.) */
{
	using namespace kry;

	try
	{
		Media::Zip zipfile;

		auto kname = qToKString(name);

		std::vector<Util::String> lines;
// MAP SETTINGS
		lines.push_back("[Settings]");
		lines.push_back("name = " + qToKString(single->getName()));
		lines.push_back("iconImage = " + prjconfig["project"]["iconImage"]);
		lines.push_back("checksum = ce114e4501d2f4e2dcea3e17b546f339");
		lines.push_back("fogOfWar = " + prjconfig["project"]["fogOfWar"]);
		lines.push_back("revealOfWar = " + prjconfig["project"]["revealOfWar"]);
		lines.push_back("fogTint = " + prjconfig["project"]["fogTint"]);
		lines.push_back("tileDimensions = 700"); /** #TODO(change) hardcoded */
		lines.push_back("floorFadeTime = " + prjconfig["project"]["floorFadeTime"]);
		lines.push_back("loadingSound = " + prjconfig["project"]["loadingSound"]);
		lines.push_back("loadingImage = " + prjconfig["project"]["loadingImage"]);
// OBJECTIVE SETTINGS
		lines.push_back("[Objectives]");
// FLOORS/FLOOR SETTINGS
		lines.push_back("[Floors]");
		for (auto& floor : Map::getMap()->getLayers())
			lines.push_back("floor" + Util::toString(floor->index) + '=' + Util::toString(floor->index));
		
		for (auto& floor : Map::getMap()->getLayers())
		{
			lines.push_back("[floor" + Util::toString(floor->index) + ']');
			lines.push_back("dimensions= { " + Util::toString(floor->size[0]) + ", " + Util::toString(floor->size[1]) + " }");
			lines.push_back("floorBinary=floors/floor" + Util::toString(floor->index) + ".txt");
		}
// ENTITY DECLARATIONS
		std::vector<Object*> objects;
		std::set<UsedAnim> objectanimsused;
		lines.push_back("[Entities]");
		lines.push_back("player=-1");
		unsigned count = 0;
		for (auto& floor : Map::getMap()->getLayers())
		{
			for (auto& tile : floor->tiles)
			{
				for (auto& object : tile.objects)
				{
					if (object->properties["global"]["hardtype"] == "wall")
						continue;
					lines.push_back("entity" + Util::toString(count++) + '=' + object->properties["global"]["id"]);
					objects.push_back(object.get());
				}
			}
		}
// ENTITY SETTINGS /** #TODO(incomplete) add the keys 'path' and 'loopPath' to these settings (player doesnt need them) */
		lines.push_back("[player]");
		lines.push_back("type=player");
		if (!prjconfig["player"]["floor"].isEmpty())
			lines.push_back("floor=" + prjconfig["player"]["floor"]);
		if (!prjconfig["player"]["position"].isEmpty())
			lines.push_back("position=" + prjconfig["player"]["position"]);
		if (!prjconfig["player"]["dimensions"].isEmpty())
			lines.push_back("dimensions=" + prjconfig["player"]["dimensions"]);
		if (!prjconfig["player"]["seeInFog"].isEmpty())
			lines.push_back("seeInFog=" + prjconfig["player"]["seeInFog"]);
		if (!prjconfig["player"]["directions"].isEmpty())
			lines.push_back("directions=" + prjconfig["player"]["directions"]);
		if (!prjconfig["player"]["maxHeuristic"].isEmpty())
			lines.push_back("maxHeuristic=" + prjconfig["player"]["maxHeuristic"]);
		if (!prjconfig["player"]["viewDistance"].isEmpty())
			lines.push_back("viewDistance=" + prjconfig["player"]["viewDistance"]);
		if (!prjconfig["player"]["moveAcceleration"].isEmpty())
			lines.push_back("moveAcceleration=" + prjconfig["player"]["moveAcceleration"]);
		if (!prjconfig["player"]["turnAcceleration"].isEmpty())
			lines.push_back("turnAcceleration=" + prjconfig["player"]["turnAcceleration"]);
		if (!prjconfig["player"]["maxMoveSpeed"].isEmpty())
			lines.push_back("maxMoveSpeed=" + prjconfig["player"]["maxMoveSpeed"]);
		if (!prjconfig["player"]["maxTurnSpeed"].isEmpty())
			lines.push_back("maxTurnSpeed=" + prjconfig["player"]["maxTurnSpeed"]);
		if (!prjconfig["player"]["skinIdle"].isEmpty() || !prjconfig["player"]["skinRun"].isEmpty() || !prjconfig["player"]["skinDead"].isEmpty())
			if (prjconfig["player"]["skinIdle"] != "-1" || prjconfig["player"]["skinRun"] != "-1" || prjconfig["player"]["skinDead"] != "-1")
				lines.push_back("skinConfig=PlayerSkins.txt");
		if (!prjconfig["player"]["skinIdle"].isEmpty())
			lines.push_back("skinIdle=" + prjconfig["player"]["skinIdle"]);
		if (!prjconfig["player"]["skinRun"].isEmpty())
			lines.push_back("skinRun=" + prjconfig["player"]["skinRun"]);
		if (!prjconfig["player"]["skinDead"].isEmpty())
			lines.push_back("skinDead=" + prjconfig["player"]["skinDead"]);
		if (!prjconfig["player"]["health"].isEmpty())
			lines.push_back("health=" + prjconfig["player"]["health"]);
		if (!prjconfig["player"]["direction"].isEmpty())
			lines.push_back("direction=" + prjconfig["player"]["direction"]);
		if (!prjconfig["player"]["group"].isEmpty())
			lines.push_back("group=" + prjconfig["player"]["group"]);
		{
			auto directions = 1;
			if (!prjconfig["player"]["directions"].isEmpty())
				directions = kry::Util::toUIntegral<unsigned>(prjconfig["player"]["directions"]);
			auto skinsfile = prjconfig["player"]["directions"] + "EntitySkins.txt";
			if (!prjconfig["player"]["skinIdle"].isEmpty() && prjconfig["player"]["skinIdle"] != "-1")
			{
				auto index = Util::toUIntegral<size_t>(prjconfig["player"]["skinIdle"]);
				objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, directions, index });
			}
			if (!prjconfig["player"]["skinRun"].isEmpty() && prjconfig["player"]["skinRun"] != "-1")
			{
				auto index = Util::toUIntegral<size_t>(prjconfig["player"]["skinRun"]);
				objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, directions, index });
			}
			if (!prjconfig["player"]["skinDead"].isEmpty() && prjconfig["player"]["skinDead"] != "-1")
			{
				auto index = Util::toUIntegral<size_t>(prjconfig["player"]["skinDead"]);
				objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, directions, index });
			}
		}

		count = 0;
		for (auto& object : objects)
		{
			auto section = object->hardproperties["Skins"]["name"];
			lines.push_back("[entity" + Util::toString(count++) + ']');
			lines.push_back("type=" + object->properties["global"]["hardtype"]);
			auto skinsfile = object->hardproperties["all"]["directions"] + "EntitySkins.txt";
			object->hardproperties["all"]["skinConfig"] = skinsfile;
			auto directions = Util::toUIntegral<unsigned>(object->hardproperties["all"]["directions"]);
			for (auto& key : object->hardproperties["all"].getKeyNames())
			{
				if (!object->hardproperties["all"][key].isEmpty())
				{
					lines.push_back(key + '=' + object->hardproperties["all"][key]);
					if (const_cast<Media::Config&>(Assets::getHardTypes())["all"][key] == "ANIM_ID")
					{
						if (object->hardproperties["all"][key] != "-1")
						{
							auto index = Util::toUIntegral<size_t>(object->hardproperties["all"][key]);
							objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, directions, index });
						}
					}
				}
			}
			for (auto& key : object->hardproperties[section].getKeyNames())
			{
				if (!object->hardproperties[section][key].isEmpty())
				{
					lines.push_back(key + '=' + object->hardproperties[section][key]);
					if (const_cast<Media::Config&>(Assets::getHardTypes())[section][key] == "ANIM_ID")
					{
						if (object->hardproperties[section][key] != "-1")
						{
							auto index = Util::toUIntegral<size_t>(object->hardproperties[section][key]);
							objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, directions, index });
						}
					}
				}
			}
		}

		std::set<ExpTile> tilesused;
		for (auto& layer : Map::getMap()->getLayers())
		{
			for (auto& tile : layer->tiles)
			{
				ExpTile exptile { &tile, nullptr };
				for (auto& object : tile.objects)
				{
					if (object->properties["global"]["hardtype"] == "wall")
					{
						exptile.wall = object.get();
						break;
					}
				}
				tilesused.insert(exptile);
			}
		}
// TILE/WALL DECLARATIONS
		lines.push_back("[Tiles]");
		count = 0;
		for (auto& exptile : tilesused)
			lines.push_back("tile" + Util::toString(count++) + '=' + Util::toString(exptile.wall != nullptr ? getAssetIndex(exptile.wall->asset, Assets::getTiles()) : getAssetIndex(exptile.tile->asset, Assets::getTiles())));
// TILE/WALL SETTINGS 
		count = 0;
		for (auto& exptile : tilesused)
		{
			lines.push_back("[tile" + Util::toString(count++) + ']');
			Object* object = exptile.wall != nullptr ? exptile.wall : exptile.tile;
			auto type = object->properties["global"]["hardtype"];
			Util::String skinsfile = "1TileSkins.txt";
			object->hardproperties["floor"]["skinConfig"] = skinsfile;
			if (type == "wall")
			{
				object->hardproperties[type]["floorTile"] = Util::toString(getAssetIndex(exptile.tile->asset, Assets::getTiles())); /** #TODO(change) not sure about this */
				object->hardproperties["floor"]["sortDepth"] = "1";
				object->hardproperties["floor"]["heuristic"] = "0";
			}
			for (auto& key : object->hardproperties["floor"].getKeyNames())
			{
				if (!object->hardproperties["floor"][key].isEmpty())
				{
					lines.push_back(key + '=' + object->hardproperties["floor"][key]);
					if (const_cast<Media::Config&>(Assets::getHardTypes())["floor"][key] == "ANIM_ID")
					{
						if (object->hardproperties["floor"][key] != "-1")
						{
							auto index = Util::toUIntegral<size_t>(object->hardproperties["floor"][key]);
							objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, 1, index });
						}
					}
				}
			}
			for (auto& key : object->hardproperties[type].getKeyNames())
			{
				if (!object->hardproperties[type][key].isEmpty())
				{
					lines.push_back(key + '=' + object->hardproperties[type][key]);
					if (const_cast<Media::Config&>(Assets::getHardTypes())[type][key] == "ANIM_ID")
					{
						if (object->hardproperties[type][key] != "-1")
						{
							auto index = Util::toUIntegral<size_t>(object->hardproperties[type][key]);
							objectanimsused.insert({ Resources::getAnimations()[index], skinsfile, 1, index });
						}
					}
				}
			}
		}

		zipfile[""]["Map.txt"] = linesToBin(lines);
		lines.clear();
// FLOOR FILES
		count = 0;
		for (auto& layer : Map::getMap()->getLayers())
		{
			for (size_t x = 0; x < static_cast<size_t>(layer->size[0]); ++x) // only one floor supported for now
			{
				Util::String line;
				for (size_t y = 0; y < static_cast<size_t>(layer->size[1]); ++y)
				{
					size_t index = y * layer->size[0] + x;
					ExpTile exptile { &layer->tiles[index], nullptr };
					for (auto& object : layer->tiles[index].objects)
					{
						if (object->properties["global"]["hardtype"] == "wall")
						{
							exptile.wall = object.get();
							break;
						}
					}
					line += Util::toString(exptile.wall != nullptr ? getAssetIndex(exptile.wall->asset, Assets::getTiles()) : getAssetIndex(exptile.tile->asset, Assets::getTiles())) + ' ';
				}
				line = line.substring(0, line.getLength() - 1);
				lines.push_back(line);
			}

			zipfile[""]["floor" + Util::toString(count++) + ".txt"] = linesToBin(lines);
			lines.clear();
		}

// SKIN FILES
		/** #TODO(note) if an object has 1 direction, export North(0), else if 4, export every second starting from 0, otherwise, export all. (section name end numbers must be sequential (0-8)) */
#if 0
		lines.push_back("[Settings]");
		lines.push_back("name = " + qToKString(single->getName()));
		lines.push_back("iconImage = icon.png");
		lines.push_back("checksum = 0");
		lines.push_back("tileDimensions = 700");
		lines.push_back("fogOfWar = 1");
		lines.push_back("playerSpawnFloor = " + prjconfig["player"]["layer"]);
		lines.push_back("playerSpawnPosition = { " + prjconfig["player"]["tilex"] + ", " + prjconfig["player"]["tiley"] + " }");
		lines.push_back("; just for prototype");
		lines.push_back("[Objectives]");
		//lines.push_back("objective1=0");
		lines.push_back("objectives"); // wut?
		lines.push_back("[objective1]");
		lines.push_back("type = loot");
		lines.push_back("minimumLootRequired = 100");
		lines.push_back("[Floors]");
		for (size_t i = 0; i < single->getLayers().size(); ++i)
		{
			auto str = Util::toString(i);
			lines.push_back("floor" + str + '=' + str);
		}
		std::set<Asset<Graphics::Texture>*> tilesused;
		std::set<Asset<Graphics::Texture>*> objectsused;
		for (size_t i = 0; i < single->getLayers().size(); ++i)
		{
			std::shared_ptr<Layer>& layer = single->getLayers()[i];
			lines.push_back("[floor" + Util::toString(i) + ']');
			lines.push_back("dimensions={ " + Util::toString(layer->size[0]) + ", " + Util::toString(layer->size[1]) + " }");
			lines.push_back("floorBinary=floor" + Util::toString(i) + ".txt");

			for (Tile& tile : layer->tiles)
			{
				tilesused.insert(tile.asset);
				for (std::shared_ptr<Object>& object : tile.objects)
				{
					if (object->asset->type == AssetType::STATIC_TILE_DECAL) // for now, just for walls
						tilesused.insert(object->asset);
					if (object->asset->type != AssetType::STATIC_TILE_DECAL)
						objectsused.insert(object->asset);
				}
			}
		}
		size_t wallindex; // ffs
		lines.push_back("[Tiles]"); // only one floor supported for now
		unsigned index = 0;
		size_t first = ~0;
		for (Asset<Graphics::Texture>* asset : tilesused)
		{
			size_t assetindex = getAssetIndex(asset, Assets::getTiles());
			if (first == ~0)
				first = assetindex;
			if (assetindex == ~0)
				assetindex = (wallindex = getAssetIndex(asset, Assets::getObjects()) + Assets::getTiles().size());
			lines.push_back("tile" + Util::toString(index++) + '=' + Util::toString(assetindex));
		}
		index = 0;
		for (Asset<Graphics::Texture>* asset : tilesused)
		{
			lines.push_back("[tile" + Util::toString(index++) + ']');
			if (asset->type != AssetType::STATIC_TILE_DECAL)
				lines.push_back("type=solid");
			else
				lines.push_back("type=wall"); // change this to wall
			lines.push_back("skinConfig=TileSkins.txt");
			auto skin = qToKString(asset->resource->name);
			lines.push_back("skin=" + skin.substring(0, skin.indexOf('.')));
			if (asset->type != AssetType::STATIC_TILE_DECAL)
				lines.push_back("heuristic = 1");
			else
				lines.push_back("heuristic = 0");
			if (asset->type == AssetType::STATIC_TILE_DECAL)
				lines.push_back("sortDepth=1");
			else
				lines.push_back("sortDepth=0");
			lines.push_back("sortPivotOffset = { 0, 0 }");
			if (asset->type == AssetType::STATIC_TILE_DECAL)
				lines.push_back("floorTile=" + Util::toString(first));
		}

		lines.push_back("[Entities]"); // no entities supported for now
		lines.push_back("player=0");
		lines.push_back("exit=3");

		index = 4;
		for (auto& tile : single->getLayers()[0]->tiles)
		{
			for (auto& object : tile.objects)
			{
				if (object->asset->type != AssetType::STATIC_TILE_DECAL)
				{
					lines.push_back("entity" + Util::toString(index) + "=" + Util::toString(index));
					index++;
				}
			}
		}

		lines.push_back("[player]");
		lines.push_back("type=player");
		lines.push_back("spawnFloor = 0");
		lines.push_back("position = { " + prjconfig["player"]["tiley"] + ".5, " + prjconfig["player"]["tilex"] + ".5 }");
		lines.push_back("skinConfig = EntitySkins.txt");
		lines.push_back("skin = player");
		lines.push_back("[exit]");
		lines.push_back("type = exit");
		lines.push_back("spawnFloor = 0");
		lines.push_back("position = { 0.5, 9.4 }");
		lines.push_back("skinConfig = EntitySkins.txt");
		lines.push_back("skin = exitPortal");

		index = 4;
		for (auto& tile : single->getLayers()[0]->tiles)
		{
			for (auto& object : tile.objects)
			{
				if (object->asset->type == AssetType::STATIC_TILE_DECAL)
					continue;

				lines.push_back("[entity" + Util::toString(index++) + ']');
				if (object->asset->type == AssetType::STATIC_TILE_DECAL)
					lines.push_back("type=smallGoldLoot");
				else
					lines.push_back("type=smallGoldLoot");
				lines.push_back("spawnFloor=0");

				kry::Util::Vector2f fpos = 0.5f;
				if (object->asset->properties["object"].keyExists("relativex"))
					fpos[0] = kry::Util::toDecimal<float>(object->asset->properties["object"]["relativex"]);
				if (object->asset->properties["object"].keyExists("relativey"))
					fpos[1] = kry::Util::toDecimal<float>(object->asset->properties["object"]["relativey"]);
				auto pos = coordToTileCoord(object->sprite.position + fpos * object->sprite.dimensions);
				lines.push_back("position= { " + Util::toString(pos[1]) + ", " +
													Util::toString(pos[0]) + " }");
				lines.push_back("skinConfig = EntitySkins.txt");
				if (object->asset->type != AssetType::STATIC_TILE_DECAL)
					lines.push_back("onCollectionPoints = 1");
				auto skin = qToKString(object->asset->resource->name);
				lines.push_back("skin=" + skin.substring(0, skin.indexOf('.')));
			}
		}

		zipfile[""]["Map.txt"] = linesToBin(lines);
		lines.clear();

		for (size_t x = 0; x < static_cast<size_t>(single->getLayers()[0]->size[0]); ++x) // only one floor supported for now
		{
			Util::String line;
			for (size_t y = 0; y < static_cast<size_t>(single->getLayers()[0]->size[1]); ++y)
			{
				size_t index = y * single->getLayers()[0]->size[0] + x;
				bool haswall = false;
				for (auto& object : single->getLayers()[0]->tiles[index].objects)
				{
					if (object->asset->type == AssetType::STATIC_TILE_DECAL)
					{
						wallindex = getAssetIndex(object->asset, Assets::getObjects()) + Assets::getTiles().size();
						haswall = true;
						break;
					}
				}
				size_t assetindex = getAssetIndex(single->getLayers()[0]->tiles[index].asset, Assets::getTiles());
				if (haswall)
					assetindex = wallindex;
				line += Util::toString(assetindex) + ' ';
			}
			line = line.substring(0, line.getLength() - 1);
			lines.push_back(line);
		}
		zipfile[""]["floor0.txt"] = linesToBin(lines);
		lines.clear();

		lines.push_back("[Skins]");
		for (Asset<Graphics::Texture>* asset : tilesused)
		{
			auto skin = qToKString(asset->resource->name);
			lines.push_back(skin.substring(0, skin.indexOf('.')) + '=');
		}
		for (Asset<Graphics::Texture>* asset : tilesused)
		{
			auto skin = qToKString(asset->resource->name);
			lines.push_back('[' + skin.substring(0, skin.indexOf('.')) + ']');
			lines.push_back("frames=1"); // only 1 frame supported for now
			lines.push_back("fps=1");
			lines.push_back("sheetImage = images/" + skin);
			if (asset->type != AssetType::STATIC_TILE_DECAL)
				lines.push_back("framePivot= { " + Util::toString(asset->resource->rawresource->getDimensions()[0] * 0.5f) + ", " +
					Util::toString(asset->resource->rawresource->getDimensions()[1] * 0.5f) + " }");
			else
				lines.push_back("framePivot= { " + Util::toString(asset->resource->rawresource->getDimensions()[0] * Util::toDecimal<float>(asset->properties["object"]["relativex"])) + ", " +
					Util::toString(asset->resource->rawresource->getDimensions()[1] * Util::toDecimal<float>(asset->properties["object"]["relativey"])) + " }");
			if (asset->type != AssetType::STATIC_TILE_DECAL)
				lines.push_back("dimensions= { " + Util::toString(asset->resource->rawresource->getDimensions()[0]) + ", " +
							Util::toString(asset->resource->rawresource->getDimensions()[1]) + " }");
		}
		zipfile[""]["TileSkins.txt"] = linesToBin(lines);
		lines.clear();

		lines.push_back("[Skins]");
		lines.push_back("exitPortal=");
		lines.push_back("player=");
		for (Asset<Graphics::Texture>* asset : objectsused)
		{
			auto skin = qToKString(asset->resource->name);
			lines.push_back(skin.substring(0, skin.indexOf('.')) + '=');
		}
		for (Asset<Graphics::Texture>* asset : objectsused)
		{
			auto skin = qToKString(asset->resource->name);
			lines.push_back('[' + skin.substring(0, skin.indexOf('.')) + ']');
			lines.push_back("frames=1");
			lines.push_back("fps=1");
			auto dims = asset->resource->rawresource->getDimensions();
			lines.push_back("framePivot= { " +
				Util::toString((asset->properties["object"].keyExists("relativex") ? Util::toDecimal<float>(asset->properties["object"]["relativex"]) : 0.5f) * dims[0]) + ", " +
					Util::toString((asset->properties["object"].keyExists("relativey") ? Util::toDecimal<float>(asset->properties["object"]["relativey"]) : 0.5f) * dims[1]) + " }");
			lines.push_back("sheetDimensions = { 1, 1 }");
			lines.push_back("sheetImage = images/" + qToKString(asset->resource->name));
		}
		lines.push_back("[exitPortal]");
		lines.push_back("frames=1");
		lines.push_back("fps=1");
		lines.push_back("framePivot= { 350.5, 179.5 }");
		lines.push_back("sheetDimensions = { 1, 1 }");
		lines.push_back("sheetImage = images/r_stair_down_y.png");
		lines.push_back("[player]");
		lines.push_back("frames=4");
		lines.push_back("fps=0");
		lines.push_back("[player: 0]");
		lines.push_back("pivot = { 119, 686 }");
		lines.push_back("image = images/Magnus_4.png");
		lines.push_back("[player: 1]");
		lines.push_back("pivot = { 119, 686 }");
		lines.push_back("image = images/Magnus_2.png");
		lines.push_back("[player: 2]");
		lines.push_back("pivot = { 119, 686 }");
		lines.push_back("image = images/Magnus_3.png");
		lines.push_back("[player: 3]");
		lines.push_back("pivot = { 119, 686 }");
		lines.push_back("image = images/Magnus_1.png");

		zipfile[""]["EntitySkins.txt"] = linesToBin(lines);
		lines.clear();

		for (Asset<Graphics::Texture>* asset : tilesused)
			zipfile["images"][qToKString(asset->resource->name)] = fileToBin(asset->resource->path);
		for (Asset<Graphics::Texture>* asset : objectsused)
			zipfile["images"][qToKString(asset->resource->name)] = fileToBin(asset->resource->path);
		zipfile["images"]["Magnus_1.png"] = fileToBin("assets\\resources\\images\\Magnus_1.png"); // hardcoded for now
		zipfile["images"]["Magnus_2.png"] = fileToBin("assets\\resources\\images\\Magnus_2.png");
		zipfile["images"]["Magnus_3.png"] = fileToBin("assets\\resources\\images\\Magnus_3.png");
		zipfile["images"]["Magnus_4.png"] = fileToBin("assets\\resources\\images\\Magnus_4.png");
#endif

		zipfile.zipToFile(kname + ".zip");
	}
	catch (const Util::Exception& e)
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
