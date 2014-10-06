#include "Map.h"
#include "PrjSetupDialog.h"
#include "LayerOptionsItem.h"
#include "Resources.h"
#include "Utilities.h"
#include "GLPanel.h"
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
		unsigned index = 0;
		for (Tile& tile : layer->tiles)
			tile.properties["global"]["id"] = kry::Util::toString(static_cast<unsigned>(i) * 10000u + (index++));

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

std::shared_ptr<Map> Map::loadFromFile(const QString& path, kry::Media::Config& prjsettings) /** #TODO(change) actually read the id attributes and set them properly for tiles/objects */
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
			prjsettings["player"]["speed"] = qToKString(element.text);
			reader.readNext();
			element = readElement(reader);
			prjsettings["player"]["fov"] = qToKString(element.text);
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

void Map::saveToFile(const QString& name, kry::Media::Config& prjsettings)
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
		writer.writeStartElement("player");
		{
			writer.writeTextElement("layer", kryToQString(prjsettings["player"]["layer"]));
			writer.writeTextElement("tilex", kryToQString(prjsettings["player"]["tilex"]));
			writer.writeTextElement("tiley", kryToQString(prjsettings["player"]["tiley"]));
			writer.writeTextElement("speed", kryToQString(prjsettings["player"]["speed"]));
			writer.writeTextElement("fov", kryToQString(prjsettings["player"]["fov"]));
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

void Map::exportToFile(const QString& name, kry::Media::Config& prjconfig)
{
	using namespace kry;

	try
	{
		Media::Zip zipfile;

		auto kname = qToKString(name);

		std::vector<Util::String> lines;

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
		lines.push_back("player=0"); /** #TODO(change) all hardcoded rubbish. no player needed (always will be a player). game does not need an exit (an objective could describe that easily) */
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

		/** #TODO(change) mention to luka to always do y before x */
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
		zipfile[""]["floor0.txt"] = linesToBin(lines); /** #TODO(change) change all of these .txt types to something much more representitive of what they hold */
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
