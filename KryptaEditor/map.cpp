#include "Map.h"
#include "PrjSetupDialog.h"
#include "LayerOptionsItem.h"
#include "Resources.h"
#include "Utilities.h"
#include "GLPanel.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "EnvBrowserDialog.h"
#include "ui_EnvBrowserDialog.h"
#include "ObjectListItem.h"
#include "ItemManagerDialog.h"
#include <System/Filesystem.h>
#include <Utilities/StringConvert.h>
#include <Media/Zip.h>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <set>
#include <fstream>

#define KRY_DEVELOPER

unsigned Object::increment(0);

struct TextElement
{
	QString name;
	QString text;
	std::vector<QString> attrVal;
};

std::shared_ptr<Map> Map::single;
QString Map::projectname;

kry::Util::Vector2f getObjectPivot(Object* object)
{
	auto type = object->properties["global"]["hardtype"];
	auto stranim = Assets::getParentType(type) == "entity" ? object->hardproperties["entity"]["skinIdle"] : object->hardproperties["floor"]["skin"];
	assert(!stranim.isEmpty());
				
	auto anim = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(stranim)];
	auto name = anim->properties[0]["Skins"]["name"];
	auto strpivot = anim->properties[0][name]["framePivot"];
	kry::Util::Vector2f pivot(Map::getMap()->getCurrentLayer()->tilesize[0] * 0.5f, Map::getMap()->getCurrentLayer()->tilesize[1] * 0.5f);
	if (!strpivot.isEmpty())
		pivot = kry::Util::Vector2f::Vector(strpivot);
	return pivot;
}

std::shared_ptr<Object> getObjectByID(const kry::Util::String& id)
{
	for (auto layer : Map::getMap()->getLayers())
		for (auto& tile : layer->tiles)
			for (auto obj : tile.objects)
				if (obj->properties["global"]["id"] == id)
					return obj;
	return std::shared_ptr<Object>();
}

Tile& getObjectOwner(Object* object, std::shared_ptr<Map::Layer> layer)
{
	auto pivot = getObjectPivot(object);
	auto tilecoord = Kryed::GLPanel::coordToTileCoord(object->sprite.position + pivot, layer);
	auto tileindex = Kryed::GLPanel::tileCoordToIndex(tilecoord, layer);
	return layer->tiles[tileindex];
}

void Map::resetMap()
{
    layers.clear();
	getItems().clear();
	getCurrentLayer().reset();
    name.clear();
}

Map::Layer* Map::createLayer(const Tile& defaulttile, const QSize& size, const QString& description, unsigned id)
{
	Layer* layer = new Layer { std::move(std::vector<Tile>(size.width() * size.height(), defaulttile)), description,
								{size.width(), size.height()}, defaulttile.asset->resource->rawresource->getDimensions(), static_cast<unsigned>(id) };
	unsigned resourceindex = 0;
	for (auto& resource : Resources::getAnimations())
	{
		if (resource.get() == defaulttile.asset->resource)
			break;
		++resourceindex;
	}
	auto dim = layer->tilesize;
	auto halfdim = dim / 2;
	kry::Util::Vector2f startpos = { 0.0f, static_cast<float>(-halfdim[1]) };
	kry::Util::Vector2f pos;
	unsigned index = 0;
	for (int y = 0; y < layer->size[1]; ++y)
	{
		pos = { static_cast<float>(halfdim[0] * y), static_cast<float>(-(halfdim[1] * y)) };
		pos = startpos + pos;

		for (int x = 0; x < layer->size[0]; ++x)
		{
			auto& tile = layer->tiles[y * layer->size[0] + x];
			tile.sprite.position = pos;
			tile.properties = defaulttile.asset->properties;
			tile.properties["global"]["id"] = kry::Util::toString(static_cast<unsigned>(id) * 10000u + (index++));
			auto type = tile.properties["global"]["hardtype"];
			for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["floor"].getKeyNames())
				tile.hardproperties["floor"][key] = "";
			for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
				tile.hardproperties[type][key] = "";
			tile.hardproperties["floor"]["skin"] = kry::Util::toString(resourceindex);
			tile.hardproperties["floor"]["heuristic"] = type == "solid" ? kry::Util::String("1") : kry::Util::String("0");
			tile.hardproperties["floor"]["sortDepth"] = type == "wall" ? kry::Util::String("1") : kry::Util::String("0");
			tile.hardproperties["floor"]["sortPivotOffset"] = "{ 0, 0 }";

			pos[0] += halfdim[0];
			pos[1] += halfdim[1];
		}
	}
	return layer;
}

std::shared_ptr<Map> Map::createMap(const QString& name, const Tile& defaulttile, QListWidget* layerList)
{
	single.reset(new Map(name, layerList->count()));

    for (int i = 0; i < layerList->count(); ++i)
    {
		if (layerList->item(i)->type() != LayerOptionsItem::LayerOptionType)
            continue;
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(layerList->item(i));
		Layer* layer = createLayer(defaulttile, QSize(option->getWidth(), option->getHeight()), option->getDescription(), i);

        single->layers.emplace_back(layer);
    }

	Item* item = new Item;
	item->name = "Example";
	item->properties["item"]["type"] = "lootitem";
	item->properties["item"]["inventorySkin"] = "0";
	item->properties["lootitem"];
	single->getItems().insert(std::pair<kry::Util::String, std::shared_ptr<Item>>(kry::Util::toString(single->getItems().size()), std::make_shared<Item>(*item)));

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
	for (auto attr : reader.attributes())
		element.attrVal.push_back(attr.value().toString());
	reader.readNext();
	element.text = reader.text().toString();
	if (reader.isEndElement())
		return element;
	reader.readNext();
	return element;
}

TextElement readNameAttr(QXmlStreamReader& reader)
{
	TextElement element;
	reader.readNext();
	element.name = reader.name().toString();
	if (reader.isEndElement())
		return element;
	for (auto attr : reader.attributes())
		element.attrVal.push_back(attr.value().toString());
	return element;
}

std::shared_ptr<Map> Map::loadFromFile(MainWindow* window, const QString& path, kry::Media::Config& prjsettings)
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
// MAP
	reader.readNext(); // map
	element = readElement(reader);
	auto prjname = element.text;
	element = readElement(reader);
	unsigned objectidcount = element.text.toUInt();
	element = readElement(reader);
	unsigned itemidcount = element.text.toUInt();
	Object::increment = objectidcount;
	ItemManagerDialog::setIDCount(itemidcount);
	reader.readNext(); // end map
// ANIMATIONS
	element = readNameAttr(reader); // animations
	unsigned count = element.attrVal[0].toUInt();
	for (unsigned i = 0; i < count; ++i)
	{
		reader.readNext(); // anim
		element = readElement(reader);
		auto name = element.text;
		element = readElement(reader);
		auto path = element.text;
		auto animation = Animation<>::createDefaultAnimation(qToKString(path));
		for (unsigned dircount = 0; dircount < Animation<>::MAX_DIRECTION_COUNT; ++dircount) // properties for each dir
		{
			element = readNameAttr(reader); // properties
			unsigned dir = element.attrVal[0].toUInt();
			unsigned sections = element.attrVal[1].toUInt();
			for (unsigned j = 0; j < sections; ++j)
			{
				element = readNameAttr(reader); // section
				auto section = qToKString(element.name);
				unsigned keys = element.attrVal[0].toUInt();
				for (unsigned k = 0; k < keys; ++k)
				{
					element = readElement(reader); // key/value
					animation->properties[dir][section][qToKString(element.name)] = qToKString(element.text);
				}
				reader.readNext(); // end section
			}
			reader.readNext(); // end properties
		}
		for (unsigned dircount = 0; dircount < Animation<>::MAX_DIRECTION_COUNT; ++dircount) // frames for each dir
		{
			element = readNameAttr(reader); // frames
			unsigned dir = element.attrVal[0].toUInt();
			unsigned framecount = element.attrVal[1].toUInt();
			for (unsigned n = 0; n < framecount; ++n)
			{
				element = readNameAttr(reader); // frame
				unsigned sections = element.attrVal[0].toUInt();
				for (unsigned j = 0; j < sections; ++j)
				{
					element = readNameAttr(reader); // section
					auto section = qToKString(element.name);
					unsigned keys = element.attrVal[0].toUInt();
					for (unsigned k = 0; k < keys; ++k)
					{
						element = readElement(reader); // key/value
						animation->frames[dir][n][section][qToKString(element.name)] = qToKString(element.text);
					}
					reader.readNext(); // end section
				}
				reader.readNext(); // end frame
			}
			reader.readNext(); // end frames
		}
		reader.readNext(); // end anim
		if (i >= Resources::getAnimations().size())
			const_cast<std::vector<std::shared_ptr<Animation<>>>&>(Resources::getAnimations()).emplace_back(animation);
		else
			const_cast<std::vector<std::shared_ptr<Animation<>>>&>(Resources::getAnimations())[i].reset(animation);
	}
	reader.readNext(); // end animations
// SOUNDS
	element = readNameAttr(reader); // sounds
	count = element.attrVal[0].toUInt();
	for (unsigned i = 0; i < count; ++i)
	{
		reader.readNext(); // sound
		{
			element = readElement(reader);
			auto name = element.text;
			element = readElement(reader);
			auto path = element.text;
			Resource<kry::Audio::Buffer>* resource = createDefaultSound(qToKString(path));
			if (i >= Resources::getSounds().size())
				const_cast<std::vector<std::shared_ptr<Resource<kry::Audio::Buffer>>>&>(Resources::getSounds()).emplace_back(resource);
			else
				const_cast<std::vector<std::shared_ptr<Resource<kry::Audio::Buffer>>>&>(Resources::getSounds())[i].reset(resource);
		}
		reader.readNext(); // end sound
	}
	reader.readNext(); // end sounds
// MUSIC
	element = readNameAttr(reader); // musics
	count = element.attrVal[0].toUInt();
	for (unsigned i = 0; i < count; ++i)
	{
		reader.readNext(); // music
		{
			element = readElement(reader);
			auto name = element.text;
			element = readElement(reader);
			auto path = element.text;
			Resource<kry::Audio::Source>* resource = createDefaultMusic(qToKString(path));
			if (i >= Resources::getMusic().size())
				const_cast<std::vector<std::shared_ptr<Resource<kry::Audio::Source>>>&>(Resources::getMusic()).emplace_back(resource);
			else
				const_cast<std::vector<std::shared_ptr<Resource<kry::Audio::Source>>>&>(Resources::getMusic())[i].reset(resource);
		}
		reader.readNext(); // end music
	}
	reader.readNext(); // end musics
// SET UP ASSETS
	Resources::reassignAnimations(const_cast<std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>&>(Assets::getTiles()));
	Resources::reassignAnimations(const_cast<std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>&>(Assets::getEntities()));
	Resources::reassignSounds(const_cast<std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>&>(Assets::getSounds()));
	Resources::reassignMusic(const_cast<std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>&>(Assets::getMusic()));
// ENV BROWSER
	element = readNameAttr(reader); // env browser
	count = element.attrVal[0].toUInt();
	for (unsigned i = 0; i < count; ++i)
	{
		reader.readNext(); // env
		{
			element = readElement(reader);
			auto name = element.text; 
			element = readElement(reader);
			auto path = element.text;
			element = readElement(reader);
			auto type = qToKString(element.text);
			auto asset = Assets::getTileByHardtype(type);
			kry::Media::Config properties;
			kry::Media::Config hardproperties;
			element = readNameAttr(reader); // properties
			unsigned sections = element.attrVal[0].toUInt();
			for (unsigned j = 0; j < sections; ++j)
			{
				element = readNameAttr(reader); // section
				auto section = qToKString(element.name);
				unsigned keys = element.attrVal[0].toUInt();
				for (unsigned k = 0; k < keys; ++k)
				{
					element = readElement(reader); // key/value
					properties[section][qToKString(element.name)] = qToKString(element.text);
				}
				reader.readNext(); // end section
			}
			reader.readNext(); // end properties
			element = readNameAttr(reader); // hardproperties
			sections = element.attrVal[0].toUInt();
			for (unsigned j = 0; j < sections; ++j)
			{
				element = readNameAttr(reader); // section
				auto section = qToKString(element.name);
				unsigned keys = element.attrVal[0].toUInt();
				for (unsigned k = 0; k < keys; ++k)
				{
					element = readElement(reader); // key/value
					hardproperties[section][qToKString(element.name)] = qToKString(element.text);
				}
				reader.readNext(); // end section
			}
			reader.readNext(); // end hardproperties
			auto item = EnvBr::createListItem(asset.get(), path, name);
			window->getEnvBrowser()->getUI()->lbIcons->addItem(item);
			window->getEnvBrowser()->overrideFirstLoad(false);
		}
		reader.readNext(); // end env
	}
	reader.readNext(); // end env browser
// PROJECT SETTINGS
	element = readNameAttr(reader); // project settings
	count = element.attrVal[0].toUInt();
	for (unsigned i = 0; i < count; ++i)
	{
		element = readNameAttr(reader); // section
		auto section = qToKString(element.name);
		unsigned keys = element.attrVal[0].toUInt();
		for (unsigned k = 0; k < keys; ++k)
		{
			element = readElement(reader); // key/value
			prjsettings[section][qToKString(element.name)] = qToKString(element.text);
		}
		reader.readNext(); // end section
	}
	reader.readNext(); // end project settings
// FLOORS
	element = readNameAttr(reader); // floors
	count = element.attrVal[0].toUInt();
	single.reset(new Map(prjname.left(prjname.lastIndexOf('.')), count)); // reset the map
	single->setProjectName(prjname);
	for (unsigned i = 0; i < count; ++i)
	{
		element = readNameAttr(reader); // floor
		auto id = element.attrVal[0];
		{
			element = readElement(reader); // name
			auto description = element.text;
			element = readElement(reader); // width
			auto width = element.text;
			element = readElement(reader); // height
			auto height = element.text;
			element = readElement(reader); // tilewidth
			auto tilewidth = element.text;
			element = readElement(reader); // tileheight
			auto tileheight = element.text;

			Tile defaulttile;
			defaulttile.asset = Assets::getTiles()[0].get(); /** #TODO(bug) there might not be any assets */
			auto layer = createLayer(defaulttile, QSize(width.toInt(), height.toInt()), description, i);
			single->layers.emplace_back(layer);
		}
		reader.readNext(); // end floor
	}
	reader.readNext(); // end floors
// ITEMS
	element = readNameAttr(reader); // items
	count = element.attrVal[0].toUInt();
	for (unsigned i = 0; i < count; ++i)
	{
		element = readNameAttr(reader); // item
		auto id = element.attrVal[0];
		{
			element = readElement(reader); // name
			auto name = element.text;
			Item* item = new Item;
			item->name = name;
			element = readNameAttr(reader); // properties
			unsigned sections = element.attrVal[0].toUInt();
			for (unsigned j = 0; j < sections; ++j)
			{
				element = readNameAttr(reader); // section
				auto section = qToKString(element.name);
				unsigned keys = element.attrVal[0].toUInt();
				for (unsigned k = 0; k < keys; ++k)
				{
					element = readElement(reader); // key/value
					item->properties[section][qToKString(element.name)] = qToKString(element.text);
				}
				reader.readNext(); // end section
			}
			reader.readNext(); // end properties
			single->getItems().insert(std::pair<Util::String, std::shared_ptr<Item>>(Util::toString(i), std::make_shared<Item>(*item)));
		}
		reader.readNext(); // end item
	}
	reader.readNext(); // end items
// TILE DATA
	std::map<Object*, std::set<QString>> spawnids;
	reader.readNext(); // tiledata
	{
		for (size_t i = 0; i < single->layers.size(); ++i)
		{
			std::shared_ptr<Layer>& layer = single->layers[i];
			for (size_t j = 0; j < layer->tiles.size(); ++j)
			{
				Tile& tile = layer->tiles[j];
// TILE
				element = readNameAttr(reader); // tile
				{
					element = readElement(reader); // layer id
					element = readElement(reader); // type
					tile.asset = Assets::getTileByHardtype(qToKString(element.text)).get();
					element = readElement(reader); // object count
					auto objcount = element.text.toUInt();
					tile.objects.reserve(objcount);
					element = readElement(reader); // position
					tile.sprite.position = Util::Vector2f::Vector(qToKString(element.text));
					element = readNameAttr(reader); // properties
					unsigned sections = element.attrVal[0].toUInt();
					for (unsigned k = 0; k < sections; ++k)
					{
						element = readNameAttr(reader); // section
						auto section = qToKString(element.name);
						unsigned keys = element.attrVal[0].toUInt();
						for (unsigned n = 0; n < keys; ++n)
						{
							element = readElement(reader); // key/value
							tile.properties[section][qToKString(element.name)] = qToKString(element.text);
						}
						reader.readNext(); // end section
					}
					reader.readNext(); // end properties
					element = readNameAttr(reader); // hardproperties
					sections = element.attrVal[0].toUInt();
					for (unsigned k = 0; k < sections; ++k)
					{
						element = readNameAttr(reader); // section
						auto section = qToKString(element.name);
						unsigned keys = element.attrVal[0].toUInt();
						for (unsigned n = 0; n < keys; ++n)
						{
							element = readElement(reader); // key/value
							tile.hardproperties[section][qToKString(element.name)] = qToKString(element.text);
						}
						reader.readNext(); // end section
					}
					reader.readNext(); // end hardproperties
// OBJECTS
					reader.readNext(); // objects
					{
						for (unsigned k = 0; k < objcount; ++k)
						{
							element = readNameAttr(reader); // object
							{
								Object* object = new Object;
								element = readElement(reader); // tile id
								element = readElement(reader); // type
								object->asset = (element.text == "wall" ? Assets::getTileByHardtype(qToKString(element.text)) : Assets::getEntityByHardtype(qToKString(element.text))).get();
								element = readElement(reader); // position
								object->sprite.position = Util::Vector2f::Vector(qToKString(element.text));
								element = readNameAttr(reader); // properties
								sections = element.attrVal[0].toUInt();
								for (unsigned n = 0; n < sections; ++n)
								{
									element = readNameAttr(reader); // section
									auto section = qToKString(element.name);
									unsigned keys = element.attrVal[0].toUInt();
									for (unsigned m = 0; m < keys; ++m)
									{
										element = readElement(reader); // key/value
										object->properties[section][qToKString(element.name)] = qToKString(element.text);
									}
									reader.readNext(); // end section
								}
								reader.readNext(); // end properties
								element = readNameAttr(reader); // hardproperties
								sections = element.attrVal[0].toUInt();
								for (unsigned n = 0; n < sections; ++n)
								{
									element = readNameAttr(reader); // section
									auto section = qToKString(element.name);
									unsigned keys = element.attrVal[0].toUInt();
									for (unsigned m = 0; m < keys; ++m)
									{
										element = readElement(reader); // key/value
										object->hardproperties[section][qToKString(element.name)] = qToKString(element.text);
									}
									reader.readNext(); // end section
								}
								reader.readNext(); // end hardproperties
								element = readNameAttr(reader); // waypoints
								unsigned wcount = element.attrVal[0].toUInt();
								for (int n = 0; n < wcount; ++n)
								{
									kry::Graphics::Sprite way;
									element = readElement(reader); // position
									way.position = Util::Vector2f::Vector(qToKString(element.text));
									element = readElement(reader); // flag
									way.texture = Resources::getEditorTexture(element.text == "FLAG_RED" ? EditorResource::FLAG_RED : EditorResource::FLAG_GREEN)->rawresource;
									way.dimensions = way.texture->getDimensions();
									object->waypoints.push_back(way);
								}
								window->getUI()->glWidget->getWaypoints().insert(std::pair<Object*, std::vector<Graphics::Sprite>>(object, object->waypoints));
								reader.readNext(); // end waypoints
								element = readNameAttr(reader); // spawns
								wcount = element.attrVal[0].toUInt();
								for (int n = 0; n < wcount; ++n)
								{
									element = readNameAttr(reader); // spawn
									auto spawnid = element.attrVal[0];
									auto isnew = element.attrVal[1] == "true";
									if (isnew)
									{
										element = readElement(reader); // asset
										Object* obj = new Object;
										obj->asset = Assets::getEntityByHardtype(qToKString(element.text)).get();
										element = readNameAttr(reader); // properties
										sections = element.attrVal[0].toUInt();
										for (unsigned n = 0; n < sections; ++n)
										{
											element = readNameAttr(reader); // section
											auto section = qToKString(element.name);
											unsigned keys = element.attrVal[0].toUInt();
											for (unsigned m = 0; m < keys; ++m)
											{
												element = readElement(reader); // key/value
												obj->properties[section][qToKString(element.name)] = qToKString(element.text);
											}
											reader.readNext(); // end section
										}
										reader.readNext(); // end properties
										element = readNameAttr(reader); // hardproperties
										sections = element.attrVal[0].toUInt();
										for (unsigned n = 0; n < sections; ++n)
										{
											element = readNameAttr(reader); // section
											auto section = qToKString(element.name);
											unsigned keys = element.attrVal[0].toUInt();
											for (unsigned m = 0; m < keys; ++m)
											{
												element = readElement(reader); // key/value
												obj->hardproperties[section][qToKString(element.name)] = qToKString(element.text);
											}
											reader.readNext(); // end section
										}
										reader.readNext(); // end hardproperties
										object->spawns.emplace_back(obj);
									}
									else
									{
										auto found = spawnids.find(object);
										if (found == spawnids.end())
											spawnids[object].insert(spawnid);
										else
											found->second.insert(spawnid);
									}
									reader.readNext(); // end spawn
								}
								reader.readNext(); // end spawns
								tile.objects.emplace_back(object);
							}
							reader.readNext(); // end object
						}
					}
					reader.readNext(); // end objects
				}
				reader.readNext(); // end tile
			}
		}
	}
	reader.readNext(); // end tiledata
	for (auto pair : spawnids)
		for (auto id : pair.second)
			pair.first->spawns.push_back(getObjectByID(qToKString(id))); // add existing spawn entities to their owners

	file.close();

	single->currentLayer = single->layers[0];
	return single;
}

void Map::saveToFile(MainWindow* window, const QString& name, kry::Media::Config& prjsettings)
{
	using namespace kry;
	window->getStatusMain()->setText("Saving project...");

	QFile file(name);
	file.open(QIODevice::WriteOnly);

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(false);
	writer.writeStartDocument();

	writer.writeStartElement("projectfile");
	{
		writer.writeStartElement("map");
		{
			writer.writeTextElement("name", name.left(name.indexOf('.')));
			writer.writeTextElement("objectid", QString::number(Object::increment));
			writer.writeTextElement("itemid", QString::number(ItemManagerDialog::getIDCount()));
		}
		writer.writeEndElement();
// ANIMATIONS
		writer.writeStartElement("animations");
		writer.writeAttribute("count", QString::number(Resources::getAnimations().size()));
		{
			unsigned animindex = 0;
			for (auto anim : Resources::getAnimations())
			{
				writer.writeStartElement("anim" + QString::number(animindex));
				{
					writer.writeTextElement("name", anim->name);
					writer.writeTextElement("path", anim->path);
					for (int i = 0; i < Animation<>::MAX_DIRECTION_COUNT; ++i)
					{
						writer.writeStartElement("properties");
						writer.writeAttribute("direction", QString::number(i));
						writer.writeAttribute("sections", QString::number(anim->properties[i].sectionExists("") ? anim->properties[i].getSectionNames().size() - 1 : anim->properties[i].getSectionNames().size()));
						{
							for (auto& section : anim->properties[i].getSectionNames())
							{
								if (section.isEmpty())
									continue;
								writer.writeStartElement(kryToQString(section));
								writer.writeAttribute("keys", QString::number(anim->properties[i][section].getKeyNames().size()));
								{
									for (auto& key : anim->properties[i][section].getKeyNames())
										writer.writeTextElement(kryToQString(key), kryToQString(anim->properties[i][section][key]));
								}
								writer.writeEndElement();
							}
						}
						writer.writeEndElement();
					}
					for (int i = 0; i < Animation<>::MAX_DIRECTION_COUNT; ++i)
					{
						writer.writeStartElement("frames");
						writer.writeAttribute("direction", QString::number(i));
						writer.writeAttribute("framecount", QString::number(anim->frames[i].size()));
						{
							for (auto& frame : anim->frames[i])
							{
								writer.writeStartElement("frame");
								writer.writeAttribute("sections", QString::number(frame.sectionExists("") ? frame.getSectionNames().size() - 1 : frame.getSectionNames().size()));
								{
									for (auto& section : frame.getSectionNames())
									{
										if (section.isEmpty())
											continue;
										writer.writeStartElement(kryToQString(section));
										writer.writeAttribute("keys", QString::number(frame[section].getKeyNames().size()));
										{
											for (auto& key : frame[section].getKeyNames())
												writer.writeTextElement(kryToQString(key), kryToQString(frame[section][key]));
										}
										writer.writeEndElement();
									}
								}
								writer.writeEndElement();
							}
						}
						writer.writeEndElement();
					}
				}
				writer.writeEndElement();
				++animindex;
			}
		}
		writer.writeEndElement();
// SOUNDS
		writer.writeStartElement("sounds");
		writer.writeAttribute("count", QString::number(Resources::getSounds().size()));
		{
			unsigned soundindex = 0;
			for (auto sound : Resources::getSounds())
			{
				writer.writeStartElement("sound" + QString::number(soundindex));
				{
					writer.writeTextElement("name", sound->name);
					writer.writeTextElement("path", sound->path);
				}
				writer.writeEndElement();
				++soundindex;
			}
		}
		writer.writeEndElement();
// MUSIC
		writer.writeStartElement("music");
		writer.writeAttribute("count", QString::number(Resources::getMusic().size()));
		{
			unsigned musicindex = 0;
			for (auto music : Resources::getMusic())
			{
				writer.writeStartElement("music" + QString::number(musicindex));
				{
					writer.writeTextElement("name", music->name);
					writer.writeTextElement("path", music->path);
				}
				writer.writeEndElement();
				++musicindex;
			}
		}
		writer.writeEndElement();
// ENV BROWSER ITEMS /** #TODO(incomplete) if i add an add button to ent browser as well, do one for that too */
		writer.writeStartElement("envbrowser");
		writer.writeAttribute("count", QString::number(window->getEnvBrowser()->getUI()->lbIcons->count()));
		{
			auto icons = window->getEnvBrowser()->getUI()->lbIcons;
			for (int i = 0; i < icons->count(); ++i)
			{
				auto item = dynamic_cast<ObjectListItem*>(icons->item(i));
				writer.writeStartElement("env" + QString::number(i));
				{
					writer.writeTextElement("text", item->text());
					writer.writeTextElement("iconpath", item->path);
					writer.writeTextElement("type", kryToQString(item->object->asset->properties["global"]["hardtype"]));
					writer.writeStartElement("properties");
					writer.writeAttribute("sections", QString::number(item->object->properties.sectionExists("") ? item->object->properties.getSectionNames().size() - 1 : item->object->properties.getSectionNames().size()));
					{
						for (auto& section : item->object->properties.getSectionNames())
						{
							if (section.isEmpty())
								continue;
							writer.writeStartElement(kryToQString(section));
							writer.writeAttribute("keys", QString::number(item->object->properties[section].getKeyNames().size()));
							{
								for (auto& key : item->object->properties[section].getKeyNames())
									writer.writeTextElement(kryToQString(key), kryToQString(item->object->properties[section][key]));
							}
							writer.writeEndElement();
						}
					}
					writer.writeEndElement();
					writer.writeStartElement("hardproperties");
					writer.writeAttribute("sections", QString::number(item->object->hardproperties.sectionExists("") ? item->object->hardproperties.getSectionNames().size() - 1 : item->object->hardproperties.getSectionNames().size()));
					{
						for (auto& section : item->object->hardproperties.getSectionNames())
						{
							if (section.isEmpty())
								continue;
							writer.writeStartElement(kryToQString(section));
							writer.writeAttribute("keys", QString::number(item->object->hardproperties[section].getKeyNames().size()));
							{
								for (auto& key : item->object->hardproperties[section].getKeyNames())
									writer.writeTextElement(kryToQString(key), kryToQString(item->object->hardproperties[section][key]));
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
// PROJECT SETTINGS
		writer.writeStartElement("projectsettings");
		writer.writeAttribute("sections", QString::number(prjsettings.sectionExists("") ? prjsettings.getSectionNames().size() - 1 : prjsettings.getSectionNames().size()));
		{
			for (auto& section : prjsettings.getSectionNames())
			{
				if (section.isEmpty())
					continue;
				writer.writeStartElement(kryToQString(section));
				writer.writeAttribute("keys", QString::number(prjsettings[section].getKeyNames().size()));
				{
					for (auto& key : prjsettings[section].getKeyNames())
						writer.writeTextElement(kryToQString(key), kryToQString(prjsettings[section][key]));
				}
				writer.writeEndElement();
			}
		}
		writer.writeEndElement();
// FLOORS
		writer.writeStartElement("floors");
		writer.writeAttribute("count", QString::number(single->layers.size()));
		{
			for (size_t i = 0; i < single->layers.size(); ++i)
			{
				std::shared_ptr<Layer>& layer = single->layers[i];
				writer.writeStartElement("floor");
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
// ITEMS
		writer.writeStartElement("items");
		writer.writeAttribute("count", QString::number(single->getItems().size()));
		{
			unsigned itemindex = 0;
			for (auto item : single->items)
			{
				writer.writeStartElement("item" + QString::number(itemindex));
				writer.writeAttribute("id", kryToQString(item.first));
				{
					writer.writeTextElement("name", item.second->name);
					writer.writeStartElement("properties");
					writer.writeAttribute("sections", QString::number(item.second->properties.sectionExists("") ? item.second->properties.getSectionNames().size() - 1 : item.second->properties.getSectionNames().size()));
					{
						for (auto& section : item.second->properties.getSectionNames())
						{
							if (section.isEmpty())
								continue;
							writer.writeStartElement(kryToQString(section));
							writer.writeAttribute("keys", QString::number(item.second->properties[section].getKeyNames().size()));
							{
								for (auto& key : item.second->properties[section].getKeyNames())
									writer.writeTextElement(kryToQString(key), kryToQString(item.second->properties[section][key]));
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
// TILE DATA
		writer.writeStartElement("tiledata");
		{
			for (size_t i = 0; i < single->layers.size(); ++i)
			{
				std::shared_ptr<Layer>& layer = single->layers[i];

				for (size_t j = 0; j < layer->tiles.size(); ++j)
				{
					Tile& tile = layer->tiles[j];
// TILE
					writer.writeStartElement("tile");
					writer.writeAttribute("id", kryToQString(tile.properties["global"]["id"]));
					{
						writer.writeTextElement("layerid", QString::number(i));
						writer.writeTextElement("asset", kryToQString(tile.asset->properties["global"]["hardtype"]));
						writer.writeTextElement("objectcount", QString::number(tile.objects.size()));
						writer.writeTextElement("position", kryToQString(tile.sprite.position.toString()));
						writer.writeStartElement("properties");
						writer.writeAttribute("sections", QString::number(tile.properties.sectionExists("") ? tile.properties.getSectionNames().size() - 1 : tile.properties.getSectionNames().size()));
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
						writer.writeAttribute("sections", QString::number(tile.hardproperties.sectionExists("") ? tile.hardproperties.getSectionNames().size() - 1 : tile.hardproperties.getSectionNames().size()));
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
// TILE OBJECTS
						writer.writeStartElement("objects");
						{
							for (size_t n = 0; n < tile.objects.size(); ++n)
							{
								std::shared_ptr<Object>& object = tile.objects[n];
								writer.writeStartElement("object");
								writer.writeAttribute("id", kryToQString(object->properties["global"]["id"]));
								{
									writer.writeTextElement("tileid", kryToQString(tile.properties["global"]["id"]));
									writer.writeTextElement("asset", kryToQString(object->asset->properties["global"]["hardtype"]));
									writer.writeTextElement("position", kryToQString(object->sprite.position.toString()));
									writer.writeStartElement("properties");
									writer.writeAttribute("sections", QString::number(object->properties.sectionExists("") ? object->properties.getSectionNames().size() - 1 : object->properties.getSectionNames().size()));
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
									writer.writeAttribute("sections", QString::number(object->hardproperties.sectionExists("") ? object->hardproperties.getSectionNames().size() - 1 : object->hardproperties.getSectionNames().size()));
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
									writer.writeStartElement("waypoints");
									writer.writeAttribute("count", QString::number(object->waypoints.size()));
									{
										for (auto& waypoint : object->waypoints)
										{
											writer.writeStartElement("waypoint");
											{
												writer.writeTextElement("position", kryToQString(waypoint.position.toString()));
												writer.writeTextElement("texture", waypoint.texture == Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource ? QString("FLAG_RED") : QString("FLAG_GREEN"));
											}
											writer.writeEndElement();
										}
									}
									writer.writeEndElement();
									writer.writeStartElement("spawns");
									writer.writeAttribute("count", QString::number(object->spawns.size()));
									{
										for (size_t n = 0; n < object->spawns.size(); ++n)
										{
											std::shared_ptr<Object> obj = object->spawns[n];
											writer.writeStartElement("spawn");
											writer.writeAttribute("id", kryToQString(obj->properties["global"]["id"]));
											bool isnew = kryToQString(obj->hardproperties["entity"]["floor"]).isEmpty();
											writer.writeAttribute("new", isnew ? QString("true") : QString("false"));
											if (isnew)
											{
												writer.writeTextElement("asset", kryToQString(obj->asset->properties["global"]["hardtype"]));
												writer.writeStartElement("properties");
												writer.writeAttribute("sections", QString::number(obj->properties.sectionExists("") ? obj->properties.getSectionNames().size() - 1 : obj->properties.getSectionNames().size()));
												{
													for (auto& section : obj->properties.getSectionNames())
													{
														if (section.isEmpty())
															continue;
														writer.writeStartElement(kryToQString(section));
														writer.writeAttribute("keys", QString::number(obj->properties[section].getKeyNames().size()));
														{
															for (auto& key : obj->properties[section].getKeyNames())
																writer.writeTextElement(kryToQString(key), kryToQString(obj->properties[section][key]));
														}
														writer.writeEndElement();
													}
												}
												writer.writeEndElement();
												writer.writeStartElement("hardproperties");
												writer.writeAttribute("sections", QString::number(obj->hardproperties.sectionExists("") ? obj->hardproperties.getSectionNames().size() - 1 : obj->hardproperties.getSectionNames().size()));
												{
													for (auto& section : obj->hardproperties.getSectionNames())
													{
														if (section.isEmpty())
															continue;
														writer.writeStartElement(kryToQString(section));
														writer.writeAttribute("keys", QString::number(obj->hardproperties[section].getKeyNames().size()));
														{
															for (auto& key : obj->hardproperties[section].getKeyNames())
																writer.writeTextElement(kryToQString(key), kryToQString(obj->hardproperties[section][key]));
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
	
	window->getStatusMain()->setText("Project saved successfully!");
}

size_t getAnimIndex(Resource<kry::Graphics::Texture>* anim)
{
	size_t index = 0;
	for ( ; index < Resources::getAnimations().size(); ++index)
		if (Resources::getAnimations()[index].get() == anim)
			return index;
	return ~0;
}

kry::Util::Vector2f coordToExpTileCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer)
{
	kry::Util::Vector2f dim = layer->tilesize;
	kry::Util::Vector2f halfdim = { static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f };

	float x = (coord[0] / halfdim[0] + coord[1] / halfdim[1]) * 0.5f;
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

kry::Util::String getIdFromFrame(const kry::Util::String& framename)
{
	assert(!framename.isEmpty());
	unsigned index = framename.getLength() - 1;
	assert(index != 0); // there should be characters before the number
	while (index != 0 && framename[index] >= '0' && framename[index] <= '9')
		--index;
	assert(index != framename.getLength() - 1); // there should be at least 1 digit on the end
	return framename.substring(index + 1);
}

kry::Util::String getPathFilename(const kry::Util::String& path)
{
	auto newpath = path;
	if (!newpath.isEmpty())
	{
		if (newpath.contains('/'))
			newpath = newpath.substring(newpath.lastIndexOf('/') + 1);
		else if (newpath.contains('\\'))
			newpath = newpath.substring(newpath.lastIndexOf('\\') + 1);
	}
	return newpath;
}

struct ExpTile
{
	Tile* tile;
	Object* wall;
};

bool operator==(const ExpTile& left, const ExpTile& right)
{
	bool result = true;
	if (left.wall != nullptr || right.wall != nullptr)
	{
		if (left.wall != nullptr && right.wall != nullptr)
			result = left.wall->sprite.texture == right.wall->sprite.texture;
		else
			result = false;
	}
	return (left.tile->hardproperties["floor"]["skin"] == right.tile->hardproperties["floor"]["skin"]) && result;
}

struct UsedAnim
{
	std::shared_ptr<Animation<>> animation;
	kry::Util::String skinfile;
	unsigned directions;
	size_t animid;
};

bool operator==(const UsedAnim& left, const UsedAnim& right)
{
	return left.animid == right.animid && left.directions == right.directions;
}

bool compareAnims(const UsedAnim& left, const UsedAnim& right)
{
	return left == right;
}

void Map::exportToFile(MainWindow* window, const QString& name, kry::Media::Config& prjconfig) /** #TODO(note) if a key is empty, dont export it (let the game crash or print error, etc.) */
{
	using namespace kry;	/** #TODO(note) remember to convert positions (and positions to ExpPositions in arrays) */
	window->getStatusMain()->setText("Exporting map...");
	try
	{
		Media::Zip zipfile;

		auto kname = qToKString(name);
		
		bool doesMissingExist = false;
		std::map<Util::String, std::vector<UsedAnim>> objectanimsused;
		typedef decltype(objectanimsused) AnimObjectMap;
		std::set<Util::String> soundsused;
		std::set<Util::String> musicused;

		std::vector<Util::String> lines;
// MAP SETTINGS
		lines.push_back("[Settings]");
		lines.push_back("name = " + qToKString(single->getName()));
		auto iconpath = prjconfig["project"]["iconImage"];
		if (!iconpath.isEmpty())
		{
			iconpath = getPathFilename(iconpath);
			lines.push_back("iconImage = images/" + iconpath);
			zipfile["images"][iconpath] = fileToBin(kryToQString(prjconfig["project"]["iconImage"]));
		}
#if defined(KRY_DEVELOPER)
		lines.push_back("checksum = ce114e4501d2f4e2dcea3e17b546f339");
#else
		lines.push_back("checksum = "); /** #TODO(incomplete) load this at the end somehow */
#endif
		lines.push_back("fogOfWar = " + boolToKBool(prjconfig["project"]["fogOfWar"]));
		lines.push_back("revealOfWar = " + boolToKBool(prjconfig["project"]["revealOfWar"]));
		lines.push_back("fogTint = " + prjconfig["project"]["fogTint"]);
		lines.push_back("fogThroughWalls = " + boolToKBool(prjconfig["project"]["fogThroughWalls"]));
		lines.push_back("fogTillLastWall = " + boolToKBool(prjconfig["project"]["fogTillLastWall"]));
		lines.push_back("tileDimensions = " + prjconfig["project"]["tileDimensions"]);
		lines.push_back("floorFadeTime = " + prjconfig["project"]["floorFadeTime"]);
		lines.push_back("cameraScale = " + prjconfig["project"]["cameraScale"]);
		lines.push_back("deathFadeTime = " + prjconfig["project"]["deathFadeTime"]);
		kry::Util::String gameskins = "GameSkins.txt";
		lines.push_back("overlaySkinConfig = " + gameskins);
		bool hasskin = false;
		if (!prjconfig["project"]["gameOverSkin"].isEmpty() && prjconfig["project"]["gameOverSkin"] != "-1")
		{
			doesMissingExist = true;
			auto index = Util::toUIntegral<size_t>(prjconfig["project"]["gameOverSkin"]);
			std::pair<Util::String, std::vector<UsedAnim>> pair;
			pair.first = gameskins;
			pair.second.push_back({ Resources::getAnimations()[index], gameskins, 1, index });
			AnimObjectMap::iterator found = objectanimsused.find(gameskins);
			if (found == objectanimsused.end())
				objectanimsused.insert(pair);
			else
			{
				auto& foundanims = found->second;
				if (std::find(foundanims.begin(), foundanims.end(), *pair.second.begin()) == foundanims.end())
					foundanims.push_back(*pair.second.begin());
			}
			lines.push_back("gameOverSkin = " + prjconfig["project"]["gameOverSkin"]);
			hasskin = true;
		}
		if (!prjconfig["project"]["lifeSkin"].isEmpty() && prjconfig["project"]["lifeSkin"] != "-1")
		{
			doesMissingExist = true;
			auto index = Util::toUIntegral<size_t>(prjconfig["project"]["lifeSkin"]);
			std::pair<Util::String, std::vector<UsedAnim>> pair;
			pair.first = gameskins;
			pair.second.push_back({ Resources::getAnimations()[index], gameskins, 1, index });
			AnimObjectMap::iterator found = objectanimsused.find(gameskins);
			if (found == objectanimsused.end())
				objectanimsused.insert(pair);
			else
			{
				auto& foundanims = found->second;
				if (std::find(foundanims.begin(), foundanims.end(), *pair.second.begin()) == foundanims.end())
					foundanims.push_back(*pair.second.begin());
			}
			lines.push_back("lifeSkin = " + prjconfig["project"]["lifeSkin"]);
			hasskin = true;
		}
		if (hasskin)
			lines.push_back("overlaySkinConfig = " + gameskins);
		prjconfig["project"]["soundtrackSize"] = Util::toString(Assets::getMusic().size()); /** #TODO(change) would prefer this not to be here, but meh for now */
		lines.push_back("soundtrackSize = " + prjconfig["project"]["soundtrackSize"]);
		auto soundtracksize = Util::toUIntegral<unsigned>(prjconfig["project"]["soundtrackSize"]);
		for (unsigned i = 0; i < soundtracksize; ++i)
		{
			auto music = Assets::getMusic()[i];
			lines.push_back("soundtrack" + Util::toString(i) + "=sounds/" + getPathFilename(qToKString(music->resource->path)));
			musicused.insert(Util::toString(i));
		}
		lines.push_back("randomizeSoundtrack = " + boolToKBool(prjconfig["project"]["randomizeSoundtrack"]));
		lines.push_back("inventoryTextSize = " + boolToKBool(prjconfig["project"]["inventoryTextSize"]));
		lines.push_back("inventoryIconDimensions = " + boolToKBool(prjconfig["project"]["inventoryIconDimensions"]));
		lines.push_back("inventoryIconGap = " + boolToKBool(prjconfig["project"]["inventoryIconGap"]));
// ITEM DECLARATIONS
		lines.push_back("[Items]");
		for (auto itempair : Map::getMap()->getItems())
			lines.push_back("item" + itempair.first + '=' + itempair.first);
// ITEM DEFINITIONS
		auto writeItemProps = [&lines, &objectanimsused, &doesMissingExist, &soundsused, &musicused](Item* item, const kry::Util::String& section, const kry::Util::String& skinsfile)
		{
			for (auto& key : item->properties[section].getKeyNames())
			{
				auto value = item->properties[section][key];
				auto widgettype = const_cast<Media::Config&>(Assets::getHardTypes())[section][key];
				if (widgettype == "ANIM_ID" || widgettype == "ANIM_ID_FORCE")
				{
					if (widgettype == "ANIM_ID")
					{
						if (value != "-1")
						{
							doesMissingExist = true;
							auto index = Util::toUIntegral<size_t>(value);
							std::pair<Util::String, std::vector<UsedAnim>> pair;
							pair.first = skinsfile;
							pair.second.push_back({ Resources::getAnimations()[index], skinsfile, 1, index });
							AnimObjectMap::iterator found = objectanimsused.find(skinsfile);
							if (found == objectanimsused.end())
								objectanimsused.insert(pair);
							else
							{
								auto& foundanims = found->second;
								if (std::find(foundanims.begin(), foundanims.end(), *pair.second.begin()) == foundanims.end())
									foundanims.push_back(*pair.second.begin());
							}
						}
						else
							value = "";
					}
					else
					{
						doesMissingExist = true;
						auto index = Util::toUIntegral<size_t>(value);
						std::pair<Util::String, std::vector<UsedAnim>> pair;
						pair.first = skinsfile;
						pair.second.push_back({ Resources::getAnimations()[index], skinsfile, 1, index });
						AnimObjectMap::iterator found = objectanimsused.find(skinsfile);
						if (found == objectanimsused.end())
							objectanimsused.insert(pair);
						else
						{
							auto& foundanims = found->second;
							if (std::find(foundanims.begin(), foundanims.end(), *pair.second.begin()) == foundanims.end())
								foundanims.push_back(*pair.second.begin());
						}
					}
				}
				else if (widgettype == "SOUND_ID")
				{
					if (value != "-1")
					{
						soundsused.insert(value);
						auto path = Resources::getSounds()[Util::toUIntegral<size_t>(value)]->path;
						value = "sounds/" + getPathFilename(qToKString(path));
					}
					else
						value = "";
				}
				else if (widgettype == "MUSIC_ID")
				{
					if (value != "-1")
					{
						musicused.insert(value);
						auto path = Resources::getMusic()[Util::toUIntegral<size_t>(value)]->path;
						value = "sounds/" + getPathFilename(qToKString(path));
					}
					else
						value = "";
				}
				else if (widgettype == "BOOL")
					value = boolToKBool(value);
				else if (widgettype == "ITEM_TYPE")
					value = value.substring(0, value.indexOf("item"));
				if (!value.isEmpty() || const_cast<kry::Media::Config&>(Assets::getRequiredKeys())[section].keyExists(key))
					lines.push_back(key + '=' + value);
			}
		};
		for (auto itempair : Map::getMap()->getItems())
		{
			lines.push_back("[item" + itempair.first + ']');
			auto skinsfile = Util::String("1ItemSkins.txt");
			auto type = itempair.second->properties["item"]["type"];
			writeItemProps(itempair.second.get(), "item", skinsfile);
			writeItemProps(itempair.second.get(), type, skinsfile);
		}
// FLOORS/FLOOR SETTINGS
		lines.push_back("[Floors]");
		for (auto& floor : Map::getMap()->getLayers())
			lines.push_back("floor" + Util::toString(floor->index) + '=' + Util::toString(floor->index));
		
		for (auto& floor : Map::getMap()->getLayers())
		{
			lines.push_back("[floor" + Util::toString(floor->index) + ']');
			lines.push_back("dimensions= { " + Util::toString(floor->size[1]) + ", " + Util::toString(floor->size[0]) + " }");
			lines.push_back("floorBinary=floors/floor" + Util::toString(floor->index) + ".txt");
		}
// ENTITY/TILE COLLECTION
		std::vector<Object*> objects;
		std::vector<ExpTile> tilesused;
		unsigned count = 0;
		for (auto& floor : Map::getMap()->getLayers())
		{
			for (auto& tile : floor->tiles)
			{
				ExpTile exptile { &tile, nullptr };
				ExpTile exptilewall { &tile, nullptr };
				for (auto& object : tile.objects)
				{
					if (object->properties["global"]["hardtype"] == "wall")
						exptilewall.wall = object.get(); /** #TODO(note) cheeky as, only takes the last wall from the tile, might change this one day */
					else
					{
						objects.push_back(object.get());
						std::sort(objects.begin(), objects.end(), [](Object* left, Object* right) -> bool { return left->properties["global"]["id"] < right->properties["global"]["id"]; });
					}
				}
				if (std::find(tilesused.rbegin(), tilesused.rend(), exptile) == tilesused.rend())
					tilesused.push_back(exptile); // add the tile itself
				if (std::find(tilesused.rbegin(), tilesused.rend(), exptilewall) == tilesused.rend())
					tilesused.push_back(exptilewall); // add the tile and the wall (if any)
			}
		}
// ENTITY DECLARATIONS 
		count = 0;
		lines.push_back("[Entities]");
		lines.push_back("player=-1");
		for (auto object : objects)
			lines.push_back("entity" + Util::toString(count++) + '=' + object->properties["global"]["id"]);
// ENTITY SETTINGS /** #TODO(incomplete) path positions need to be converted to expPositions (either here or in object settingss) */
		{
			Util::String playersection = "player";
			lines.push_back("[player]");
			lines.push_back("type=player");
			auto playerskinsfile = prjconfig["entity"]["directions"] + "EntitySkins.txt";
			prjconfig["entity"]["skinConfig"] = playerskinsfile;
			auto playerdirections = Util::toUIntegral<unsigned>(prjconfig["entity"]["directions"]);
			auto writeObjectProps = [&lines, &objectanimsused, &doesMissingExist, &soundsused, &musicused](kry::Media::Config& hardproperties, const kry::Util::String& section, const kry::Util::String& skinsfile, unsigned directions)
			{
				for (auto& key : hardproperties[section].getKeyNames())
				{
					auto widgettype = const_cast<Media::Config&>(Assets::getHardTypes())[section][key];
					auto value = hardproperties[section][key];
					if (!value.isEmpty())
					{
						if (widgettype == "ANIM_ID" || widgettype == "ANIM_ID_FORCE")
						{
							if (widgettype == "ANIM_ID")
							{
								if (value != "-1")
								{
									doesMissingExist = true;
									auto index = Util::toUIntegral<size_t>(value);
									std::pair<Util::String, std::vector<UsedAnim>> pair;
									pair.first = skinsfile;
									pair.second.push_back({ Resources::getAnimations()[index], skinsfile, directions, index });
									AnimObjectMap::iterator found = objectanimsused.find(skinsfile);
									if (found == objectanimsused.end())
										objectanimsused.insert(pair);
									else
									{
										auto& foundanims = found->second;
										if (std::find(foundanims.begin(), foundanims.end(), *pair.second.begin()) == foundanims.end())
											foundanims.push_back(*pair.second.begin());
									}
								}
								else
									value = "";
							}
							else
							{
								doesMissingExist = true;
								auto index = Util::toUIntegral<size_t>(value);
								std::pair<Util::String, std::vector<UsedAnim>> pair;
								pair.first = skinsfile;
								pair.second.push_back({ Resources::getAnimations()[index], skinsfile, directions, index });
								AnimObjectMap::iterator found = objectanimsused.find(skinsfile);
								if (found == objectanimsused.end())
									objectanimsused.insert(pair);
								else
								{
									auto& foundanims = found->second;
									if (std::find(foundanims.begin(), foundanims.end(), *pair.second.begin()) == foundanims.end())
										foundanims.push_back(*pair.second.begin());
								}
							}
						}
						else if (widgettype == "BOOL")
							value = boolToKBool(value);
						else if (widgettype == "VEC_2_POS")
						{
							/** #TODO(note) cheap. only so happens that this widgettype only appears in entities. otherwise, this would crash for tiles */
							auto type = Assets::isParentType(section) ? section : Assets::getParentType(section);
							auto layerindex = kry::Util::toUIntegral<size_t>(hardproperties[type]["floor"]);
							if (layerindex >= getMap()->getLayers().size())
								layerindex = 0;
							value = coordToExpTileCoord(kry::Util::Vector2f::Vector(value), getMap()->getLayers()[layerindex]).toString();
						}
						else if (widgettype == "SOUND_ID")
						{
							if (value != "-1")
							{
								soundsused.insert(value);
								auto path = Resources::getSounds()[Util::toUIntegral<size_t>(value)]->path;
								value = "sounds/" + getPathFilename(qToKString(path));
							}
							else
								value = "";
						}
						else if (widgettype == "MUSIC_ID")
						{
							if (value != "-1")
							{
								musicused.insert(value);
								auto path = Resources::getMusic()[Util::toUIntegral<size_t>(value)]->path;
								value = "sounds/" + getPathFilename(qToKString(path));
							}
							else
								value = "";
						}
						else if (widgettype == "ITEM_ID")
						{
							if (value == "-1")
								value = "";
						}
						if (!value.isEmpty() || const_cast<kry::Media::Config&>(Assets::getRequiredKeys())[section].keyExists(key))
							lines.push_back(key + '=' + value);
					}
					else
					{
						if (widgettype == "ITEM_ARR" || widgettype == "ENTITY_GROUP_ARR" || widgettype == "ENTITY_ARR" || widgettype == "VEC_2_POS_ARR" || 
							const_cast<kry::Media::Config&>(Assets::getRequiredKeys())[section].keyExists(key))
							lines.push_back(key + '=' + value);
					}
				}
			};
			writeObjectProps(prjconfig, "entity", playerskinsfile, playerdirections);
			writeObjectProps(prjconfig, "player", playerskinsfile, playerdirections);

			count = 0;
			for (auto& object : objects)
			{
				auto section = object->properties["global"]["hardtype"];
				lines.push_back("[entity" + Util::toString(count++) + ']');
				lines.push_back("type=" + (section == "itementity" ? Util::String("item") : section));
				auto skinsfile = object->hardproperties["entity"]["directions"] + "EntitySkins.txt";
				object->hardproperties["entity"]["skinConfig"] = skinsfile;
				auto directions = Util::toUIntegral<unsigned>(object->hardproperties["entity"]["directions"]);
				writeObjectProps(object->hardproperties, "entity", skinsfile, directions);
				writeObjectProps(object->hardproperties, section, skinsfile, directions);
			}
// TILE/WALL DECLARATIONS
			lines.push_back("[Tiles]");
			count = 0;
			for (auto& exptile : tilesused)
			{
				lines.push_back("tile" + Util::toString(count) + '=' + Util::toString(count));
				++count;
			}
// TILE/WALL SETTINGS 
			count = 0;
			for (auto& exptile : tilesused)
			{
				lines.push_back("[tile" + Util::toString(count++) + ']');
				Object* object = exptile.wall != nullptr ? exptile.wall : exptile.tile;
				auto type = object->properties["global"]["hardtype"];
				lines.push_back("type=" + type);
				Util::String skinsfile = gameskins;
				object->hardproperties["floor"]["skinConfig"] = skinsfile;
				if (type == "wall")
				{
					ExpTile checkexptile { exptile.tile, nullptr };
					unsigned expindex = 0;
					for (auto& exptile : tilesused)
					{
						if (exptile == checkexptile)
							break;
						++expindex;
					}
					assert(expindex != tilesused.size());
					object->hardproperties[type]["floorTile"] = Util::toString(expindex);
				}
				writeObjectProps(object->hardproperties, "floor", skinsfile, 1);
				writeObjectProps(object->hardproperties, type, skinsfile, 1);
			}
		}

		zipfile[""]["Map.txt"] = linesToBin(lines);
		lines.clear();
// FLOOR FILES
		count = 0;
		for (auto& layer : Map::getMap()->getLayers())
		{
			for (size_t x = 0; x < static_cast<size_t>(layer->size[0]); ++x)
			{
				Util::String line;
				for (size_t y = 0; y < static_cast<size_t>(layer->size[1]); ++y)
				{
					size_t index = y * layer->size[0] + x;
					ExpTile checkexptile { &layer->tiles[index], nullptr };
					for (auto& object : layer->tiles[index].objects)
					{
						if (object->properties["global"]["hardtype"] == "wall")
						{
							checkexptile.wall = object.get();
							break;
						}
					}
					unsigned expindex = 0;
					for (auto& exptile : tilesused)
					{
						if (exptile == checkexptile)
							break;
						++expindex;
					}
					assert(expindex != tilesused.size());
					line += Util::toString(expindex) + ' ';
				}
				line = line.substring(0, line.getLength() - 1);
				lines.push_back(line);
			}

			zipfile["floors"]["floor" + Util::toString(count++) + ".txt"] = linesToBin(lines);
			lines.clear();
		}

// SKIN FILES /** #TODO(change) allow -1s for anim_ids, just set the id of the missing tile skin to -1, also add a missing tile skin (hardcode) */
		auto writeAnim = [&lines](UsedAnim& anim, const kry::Util::String& strid, unsigned dirindex)
		{
			auto name = anim.animation->properties[dirindex]["Skins"]["name"];
			lines.push_back('[' + strid + ']');
			for (auto& key : anim.animation->properties[dirindex][name].getKeyNames()) // actual animation sheet
			{
				auto value = anim.animation->properties[dirindex][name][key];
				if (!value.isEmpty())
				{
					if (key == "sheetImage")
					{
						if (value.contains('/'))
							value = value.substring(value.lastIndexOf('/') + 1);
						value = "images/" + value;
					}
					lines.push_back(key + '=' + value);
				}
			}
			unsigned framenum = 0;
			for (auto& frame : anim.animation->frames[dirindex]) // frames
			{
				auto section = frame["Skins"]["name"];

				bool empty = true;
				for (auto& key : frame[section].getKeyNames())
				{
					auto ie = frame[section][key].trim().isEmpty();
					if (!ie)
					{
						empty = false;
						break;
					}
				}
				if (!empty)
				{
					lines.push_back('[' + strid + ": " + Util::toString(framenum) + ']');
					for (auto& key : frame[section].getKeyNames())
					{
						auto value = frame[name][key];
						bool ie = value.isEmpty();
						if (!ie)
						{
							if (key == "image")
							{
								if (value.contains('/'))
									value = value.substring(value.lastIndexOf('/') + 1);
								value = "images/" + value;
							}
							lines.push_back(key + '=' + value);
						}
					}
				}
				++framenum;
			}
		};
		for (auto& pair : objectanimsused)
		{
			auto skinsfile = pair.first;
			lines.push_back("[Skins]");
// SKIN DECLARATIONS
			char dirs = skinsfile[0];
			for (auto& anim : pair.second)
			{
				if (anim.skinfile != gameskins)
				{
					for (char i = 0; i < dirs - '0'; ++i)
						lines.push_back((Util::toString(anim.animid) + char((dirs - (dirs - '0')) + i)) + '=');
				}
				else
					lines.push_back(Util::toString(anim.animid) + '=');
			}
// SKIN DEFINITIONS
			for (auto& anim : pair.second)
			{
				auto strid = Util::toString(anim.animid);
				if (anim.skinfile != gameskins)
				{
					for (char i = 0; i < dirs - '0'; ++i)
						writeAnim(const_cast<UsedAnim&>(anim), (strid + char((dirs - (dirs - '0')) + i)), static_cast<unsigned>(dirs == '4' ? (i * 2) : i));
				}
				else
					writeAnim(const_cast<UsedAnim&>(anim), strid, 0);
			}
			zipfile[""][skinsfile] = linesToBin(lines);
			lines.clear();
		}
// IMAGE FILE COPIES
		std::set<size_t> animindicies;
		for (auto& pair : objectanimsused)
			for (auto& anim : pair.second)
				animindicies.insert(anim.animid);
		for (auto index : animindicies)
		{
			auto animation = Resources::getAnimations()[index];
			for (unsigned int i = 0; i < Animation<>::MAX_DIRECTION_COUNT; ++i)
			{
				auto name = animation->properties[i]["Skins"]["name"];
				auto simage = animation->properties[i][name]["sheetImage"];
				if (!simage.isEmpty())
				{
					auto filename = simage;
					if (filename.contains('/'))
						filename = filename.substring(filename.lastIndexOf('/') + 1);
					zipfile["images"][filename] = fileToBin(kryToQString(simage));
				}
				for (auto& frame : animation->frames[i])
				{
					auto fname = frame["Skins"]["name"];
					auto fimage = frame[name]["image"];
					if (!fimage.isEmpty())
					{
						auto filename = fimage;
						if (filename.contains('/'))
							filename = filename.substring(filename.lastIndexOf('/') + 1);
						zipfile["images"][filename] = fileToBin(kryToQString(fimage));
					}
				}
			}
		}
		for (auto strindex : soundsused)
		{
			auto index = kry::Util::toUIntegral<size_t>(strindex);
			zipfile["sounds"][getPathFilename(qToKString(Resources::getSounds()[index]->path))] = fileToBin(Resources::getAnimations()[index]->path);
		}
		for (auto strindex : musicused)
		{
			auto index = kry::Util::toUIntegral<size_t>(strindex);
			zipfile["music"][getPathFilename(qToKString(Resources::getMusic()[index]->path))] = fileToBin(Resources::getMusic()[index]->path);
		}

// WRITE ZIP TO FILE
		zipfile.zipToFile(kname + ".zip");
		window->getStatusMain()->setText("Map exported successfully!");
	}
	catch (const Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Export Failure", e.what(), QMessageBox::Ok);
		window->getStatusMain()->setText("Map failed to export.");
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
