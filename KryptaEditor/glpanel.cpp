#include <glew.h>
#include "GLPanel.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Resources.h"
#include "Tool.h"
#include "AssetListItem.h"
#include "ObjectAction.h"
#include "ObjectSettingsDialog.h"
#include "Utilities.h"
#include <Utilities/Math.h>
#include <Utilities/MD5.h>
#include <Graphics/Primitives.h>
#include <Utilities\Math.h>
#include <QWheelEvent>
#include <QCheckBox>
#include <QMessageBox>
#include <QDebug>
#include <Windows.h>
#include <functional>
#include <iostream>

namespace Kryed /** #TODO(change) remove the qDebugs from here */
{
    QPoint dragPos;
	Object follower;
	// SELECTION TOOL
	QPoint lastClick;
	kry::Graphics::Quad selection;

	kry::Graphics::Canvas GLPanel::canvas;
    kry::Graphics::Renderer GLPanel::renderer;
	std::map<Object*, std::vector<kry::Graphics::Sprite>> GLPanel::waypoints;

	GLPanel::GLPanel(QWidget* parent) : QGLWidget(QGLFormat(QGL::FormatOption::SampleBuffers), parent), objsettingsDialog(new ObjectSettingsDialog(this)), empty(true),
		gridmode(true), mouseDown(false)
    {
		this->setMouseTracking(true);
		follower.asset = nullptr;
		selection.rgba = 0.0f;
    }

    void GLPanel::updateCanvas()
    {
        empty = false;

        canvas.clear();
		kry::Util::Vector2f pos;
        auto& size = Map::getMap()->getCurrentLayer()->size;
		auto& tiles = Map::getMap()->getCurrentLayer()->tiles;
		auto dim = Map::getMap()->getCurrentLayer()->tilesize;
		auto halfdim = dim / 2;
		kry::Util::Vector2f startpos = { 0.0f, static_cast<float>(-halfdim[1]) };
		//kry::Util::Vector2f startpos = { static_cast<float>(dim[0]), static_cast<float>(-halfdim[1]) };
		//kry::Util::Vector2f startpos = { static_cast<float>(0.0f), static_cast<float>(dim[1] * (size[1] / 2)) };

		std::map<kry::Util::Vector3f, Object*> zorder;
        for (int y = 0; y < size[1]; ++y)
        {
			pos = { static_cast<float>(halfdim[0] * y), static_cast<float>(-(halfdim[1] * y)) };
			pos = startpos + pos;

            for (int x = 0; x < size[0]; ++x)
            {
                auto& tile = tiles[y * size[0] + x];
                tile.sprite.dimensions = dim;
                tile.sprite.position = pos;
				if (tile.hardproperties["floor"]["skin"] == "-1")
					tile.sprite.texture = Resources::getEditorTexture(EditorResource::MISSING_TILE).get()->rawresource;
				else
					tile.sprite.texture = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(tile.hardproperties["floor"]["skin"])].get()->rawresource;
				tile.sprite.dimensions = tile.sprite.texture->getDimensions();

				zorder.insert(std::pair<kry::Util::Vector3f, Object*>({pos[0], pos[1], 0.0f}, &tile));
				for (auto& object : tile.objects)
				{
					auto& p = object->sprite.position;
					auto parent = Assets::getParentType(object->properties["global"]["hardtype"]);
					if (parent == "entity")
					{
						if (object->hardproperties[parent]["skinIdle"] == "-1")
							object->sprite.texture = Resources::getEditorTexture(EditorResource::MISSING_TILE).get()->rawresource;
						else
							object->sprite.texture = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(object->hardproperties[parent]["skinIdle"])].get()->rawresource;
					}
					else // wall object
					{
						if (object->hardproperties[parent]["skin"] == "-1")
							object->sprite.texture = Resources::getEditorTexture(EditorResource::MISSING_TILE).get()->rawresource;
						else
							object->sprite.texture = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(object->hardproperties[parent]["skin"])].get()->rawresource;
					}
					object->sprite.dimensions = object->sprite.texture->getDimensions();
					
					zorder.insert(std::pair<kry::Util::Vector3f, Object*>({p[0], p[1], 1.0f},
																		  object.get()));
				}

                if (gridmode)
                {
                    // render a grid? iono
                }

				pos[0] += halfdim[0];
				pos[1] += halfdim[1];
            }
        }
		//if (follower.asset != nullptr)
		//	zorder.insert(std::pair<kry::Util::Vector3f, Object*>({follower.sprite.position[0], follower.sprite.position[1], (follower.asset->type == AssetType::STATIC_TILE ? 0.0f : 1.0f)}, &follower));

		for (auto& pair : zorder)
			canvas.addSprite(&pair.second->sprite);

		
		if (Tool<>::getTool()->getType() == ToolType::WAYPOINT)
		{
			for (auto& pair : waypoints)
				for (auto& sprite : pair.second)
					canvas.addSprite(&sprite);
			canvas.addSprite(&follower.sprite);
		}
		else if (Tool<>::getTool()->getType() == ToolType::SELECTION)
			canvas.addQuad(&selection);
		else
			canvas.addSprite(&follower.sprite); /** #TODO(change) make it so that the follower also follows the zorder rules */

		paintGL();
    }

    void GLPanel::setGrid(bool gridon)
    {
        gridmode = gridon;
        updateCanvas();
    }

	void GLPanel::resetCamera()
	{
		canvas.pan = { -(static_cast<float>(Map::getMap()->getCurrentLayer()->size[0] *
					   Map::getMap()->getCurrentLayer()->tilesize[0]) * 0.5f), 0.0f };
		paintGL();
	}

	bool GLPanel::handleToolSwitch(QMouseEvent* event, bool switchtopointer)
	{
		if (!Map::getMap())
		{
			if (switchtopointer)
				dynamic_cast<MainWindow*>(this->parent()->parent())->clearAndSwitchTool();
			return true;
		}

		if (Tool<>::getTool()->getType() == ToolType::PAINT || Tool<>::getTool()->getType() == ToolType::PAINT_COPY)
		{
			if (switchtopointer)
				dynamic_cast<MainWindow*>(this->parent()->parent())->clearAndSwitchTool();
			follower.sprite.texture = nullptr;
			follower.sprite.rgba = 0.0f;
			paintGL();
			return true;
		}
		else if (Tool<>::getTool()->getType() == ToolType::WAYPOINT)
		{
			WaypointData& data = Tool<WaypointData>::getTool()->getData();
			if (data.object == nullptr) // just a safety check
			{
				if (switchtopointer)
					dynamic_cast<MainWindow*>(this->parent()->parent())->clearAndSwitchTool();
				follower.sprite.texture = nullptr;
				follower.sprite.rgba = 0.0f;
				updateCanvas();
				return true;
			}
			if (event != nullptr)
			{
				auto cursorpos = canvas.getCoord({(float)event->x(), (float)event->y()});

				kry::Util::Vector2f pivot = { 0.09f, 1.0f };
				auto dims = Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource->getDimensions();
				unsigned index = 0;
				for (auto& waypoint : data.waypoints)
				{
					auto spritepoint = waypoint.position + dims * pivot;
					if (kry::Util::boxPointIntersect(cursorpos, spritepoint, dims))
					{
						auto object = waypoint.owner;
						QMenu context(this);
						context.addAction("Owner: " + kryToQString(object->properties["global"]["id"]));
						QAction* removeaction = new QAction("Remove", &context);
						connect(removeaction, &QAction::triggered, [this, &context, &data, index](bool checked)
						{
							if (index == 0 || index == data.waypoints.size() - 1)
							{
								data.looping = false;
								for (auto& sprite : getWaypoints()[data.object])
									sprite.texture = Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource;
							}
							data.waypoints.erase(data.waypoints.begin() + index);
							getWaypoints()[data.object].erase(getWaypoints()[data.object].begin() + index);
							updateCanvas();
							context.close();
						});
						context.addAction(removeaction);
						context.exec(event->globalPos());

						break;
					}
					++index;
				}
			}
			auto type = data.object->properties["global"]["hardtype"];
			data.object->hardproperties[type]["loopPath"] = data.looping ? kry::Util::String("true") : kry::Util::String("false");
			kry::Util::String path;
			for (auto& waypoint : data.waypoints)
				path += waypoint.position.toString() + ", ";
			data.object->hardproperties[type]["path"] = path.trim();
			if (switchtopointer)
				dynamic_cast<MainWindow*>(this->parent()->parent())->clearAndSwitchTool();
			follower.sprite.texture = nullptr;
			follower.sprite.rgba = 0.0f;
			updateCanvas();

			return true;
		}
		else if (Tool<>::getTool()->getType() == ToolType::SELECTION)
		{
			for (auto object : Tool<SelectionData>::getTool()->getData().objects)
				object->sprite.rgba = 1.0f;
			Tool<SelectionData>::getTool()->getData().objects.clear();
			if (switchtopointer)
				dynamic_cast<MainWindow*>(this->parent()->parent())->clearAndSwitchTool();
			paintGL();
			return true;
		}
		return false;
	}

	void GLPanel::handleSelectionEdit()
	{
		if (!Map::getMap() || Tool<SelectionData>::getTool()->getData().objects.empty())
			return;
		auto& objects = Tool<SelectionData>::getTool()->getData().objects;
		if (objsettingsDialog->showDialog("Group Settings", objects, true) == DialogResult::OK)
		{
			auto results = objsettingsDialog->getAllProperties();
			if (objects.empty())
				return;
			for (auto object : objects)
			{
				ObjectProperties tmp;
				tmp.type = object->properties["global"]["hardtype"];
				auto& result = *results.find(tmp);
				auto stranim = Assets::getParentType(tmp.type) == "entity" ? result.hardtypesettings["entity"]["skinIdle"] : result.hardtypesettings["floor"]["skin"];
				assert(!stranim.isEmpty());
				
				auto anim = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(stranim)];
				auto name = anim->properties[0]["Skins"]["name"];
				auto strpivot = anim->properties[0][name]["framePivot"];
				kry::Util::Vector2f pivot(0.5f, 0.5f);
				if (!strpivot.isEmpty())
					pivot = kry::Util::Vector2f::Vector(strpivot);

				auto pos = object->sprite.position + pivot;
				auto coord = coordToTileCoord(pos, Map::getMap()->getCurrentLayer());
				auto tileindex = tileCoordToIndex(coord, Map::getMap()->getCurrentLayer());
				auto& tile = Map::getMap()->getCurrentLayer()->tiles[tileindex];
				updateObject(coord, tile, object, result.type, result.hardtypesettings);
				object->hardproperties = result.hardtypesettings;
			}
			updateCanvas();
		}
	}

	void GLPanel::updateObject(const kry::Util::Vector2i& coord, Tile& tile, Object* object, const kry::Util::String& type, kry::Media::Config& hardproperties)
	{
		auto parent = Assets::getParentType(type);
		if (parent == "entity")
		{
			/** #TODO(incomplete) update its position first if its position has changed */
			auto floor = hardproperties[parent]["floor"];
			if (!floor.isEmpty() && floor != object->hardproperties[parent]["floor"])
			{
				size_t floorid = kry::Util::toUIntegral<size_t>(floor);
				if (floorid >= 0 && floorid < Map::getMap()->getLayers().size())
				{
					auto layer = Map::getMap()->getLayers()[floorid];
					auto switchLayer = [&tile, object, &layer](int newindex)
					{
						std::shared_ptr<Object> tmp;
						auto end = tile.objects.end();
						unsigned i = 0;
						for (auto itr = tile.objects.begin(); itr != end; ++itr)
						{
							if (object == itr->get())
							{
								tmp = *itr;
								tile.objects.erase(itr);
								break;
							}
							++i;
						}
						if (i >= layer->tiles[newindex].objects.size())
							layer->tiles[newindex].objects.push_back(tmp);
						else
							layer->tiles[newindex].objects.insert(layer->tiles[newindex].objects.begin() + i, tmp);
					};
					if (isValidTileCoord(coord, layer))
						switchLayer(tileCoordToIndex(coord, layer));
					else
					{
						auto newcoord = coord;
						if (newcoord[0] >= layer->size[0])
							newcoord[0] = layer->size[0] - 1;
						if (newcoord[1] >= layer->size[1])
							newcoord[1] = layer->size[1] - 1;

						switchLayer(tileCoordToIndex(newcoord, layer));
						object->sprite.position = tileCoordToCoord(newcoord, layer);
						/** #TODO(incomplete) actually set the objects sprite position to the new pos */
					}
				}
			}
			if (hardproperties[type].keyExists("path"))
			{
				if (hardproperties[type]["path"] != object->hardproperties[type]["path"])
					if (getWaypoints().find(object) != getWaypoints().end())
						getWaypoints().erase(object);
			}
		}
		else // floor
		{

		}
	}

    void GLPanel::initializeGL()
    {
		glewInit();

        renderer.addCanvas(&canvas);
    }

    void GLPanel::resizeGL(int w, int h)
    {
		this->canvas.dimensions = { static_cast<float>(w), static_cast<float>(h) };
		this->canvas.setResolution({ w, h });
    }

    void GLPanel::paintGL()
    {
        if (empty)
            return;

        auto prevcontext = wglGetCurrentContext();
        auto prevdc = wglGetCurrentDC();

        this->makeCurrent();
        auto context = wglGetCurrentContext();
        auto dc = wglGetCurrentDC();

        glClear(GL_COLOR_BUFFER_BIT);
        renderer.render(dc, context, {this->size().width(), this->size().height()});

        wglMakeCurrent(prevdc, prevcontext);
    }

    void GLPanel::wheelEvent(QWheelEvent* event)
    {
        if (empty)
            return;

        auto oldpos = this->canvas.getCoord({static_cast<float>(event->x()), static_cast<float>(event->y())});
        if (event->delta() > 0)
        {
            this->canvas.scale *= kry::Util::Vector2f(1.1f);
            if (this->canvas.scale[0] > 2.0f)
                this->canvas.scale = 2.0f;
        }
        else
		{
            this->canvas.scale *= kry::Util::Vector2f(0.9f);
			if (this->canvas.scale[0] < 0.025f)
				this->canvas.scale = 0.025f;
        }
        auto newpos = this->canvas.getCoord({static_cast<float>(event->x()), static_cast<float>(event->y())});
        this->canvas.pan += this->canvas.scale * (newpos - oldpos);

        this->paintGL();
    }

    void GLPanel::mousePressEvent(QMouseEvent* event)
    {
		if (empty)
			return;
		this->setFocus();
		dragPos = event->pos();
		if (Tool<>::getTool()->getType() == ToolType::SELECTION)
		{
			lastClick = event->pos();
			updateCanvas();
		}
    }

	void GLPanel::mouseReleaseEvent(QMouseEvent* event)
    {
		if (!mouseDown)
		{
			if (empty)
				return;
			if (event->button() == Qt::LeftButton)
			{
				auto coord = this->coordToTileCoord(canvas.getCoord({(float)event->x(), (float)event->y()}), Map::getMap()->getCurrentLayer());

				switch (Tool<>::getTool()->getType())
				{
					case ToolType::PAINT:
						{
							if (!isValidTileCoord(coord, Map::getMap()->getCurrentLayer()))
								break;
							size_t index = this->tileCoordToIndex(coord, Map::getMap()->getCurrentLayer());
							if (!isValidIndex(index, Map::getMap()->getCurrentLayer()))
								break;

							if (!Tool<PaintData>::getTool()->getData().objectasset)
								break;

							auto objectasset = Tool<PaintData>::getTool()->getData().objectasset;
							if (objectasset->asset->type == AssetType::TILE)
							{
								Tile& tile = Map::getMap()->getCurrentLayer()->tiles[index];
								auto id = tile.properties["global"]["id"];
								tile.asset = objectasset->asset;
								tile.sprite.texture = follower.sprite.texture;
								tile.properties = objectasset->asset->properties;
								tile.hardproperties = objectasset->hardproperties;
								/*
								auto type = objectasset->asset->properties["global"]["hardtype"];
								auto parenttype = Assets::getParentType(type);
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
									tile.hardproperties[type][key] = "";
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[parenttype].getKeyNames())
									tile.hardproperties[parenttype][key] = "";
								unsigned index = 0;
								for (auto& resource : Resources::getAnimations())
								{
									if (resource.get() == asset->resource)
										break;
									++index;
								}
								if (parenttype == "entity")
								{
									tile.hardproperties[parenttype]["skinIdle"] = kry::Util::toString(index);
									tile.hardproperties[parenttype]["floor"] = kry::Util::toString(Map::getMap()->getCurrentLayer()->index);
								}
								else
									tile.hardproperties[parenttype]["skin"] = kry::Util::toString(index);
									*/
							}
							else
							{
								Object* object = new Object;
								object->sprite.position = follower.sprite.position;
								object->sprite.texture = follower.sprite.texture;
								object->sprite.dimensions = follower.sprite.texture->getDimensions();

								object->asset = objectasset->asset;
								object->properties = objectasset->asset->properties;
								object->hardproperties = objectasset->hardproperties;
								/*
								auto type = object->properties["global"]["hardtype"];
								auto parenttype = Assets::getParentType(type);
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
									object->hardproperties[type][key] = "";
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[parenttype].getKeyNames())
									object->hardproperties[parenttype][key] = "";
								unsigned animindex = 0;
								for (auto& resource : Resources::getAnimations())
								{
									if (resource.get() == asset->resource)
										break;
									++animindex;
								}
								auto parent = Assets::getParentType(object->properties["global"]["hardtype"]);
								if (parent == "entity")
								{
									object->hardproperties[parent]["skinIdle"] = kry::Util::toString(animindex);
									object->hardproperties[parent]["floor"] = kry::Util::toString(Map::getMap()->getCurrentLayer()->index);
								}
								else if (parent == "floor")
									object->hardproperties[parent]["skin"] = kry::Util::toString(animindex);
									*/
								//for (size_t i = 0; i < EventSystem::getSystem()->getEvents().size(); ++i)
								//	object->events.push_back(parseEvent(EventSystem::getSystem()->getEvents()[i].name, object->properties["events"][kry::Util::toString(i)]));

								object->properties["global"]["id"] = kry::Util::toString(Object::increment++);
								object->properties["global"]["posx"] = kry::Util::toString(follower.sprite.position[0]); /** #TODO(change) this is for map save/load */
								object->properties["global"]["posy"] = kry::Util::toString(follower.sprite.position[1]);
								if (Assets::getParentType(object->properties["global"]["hardtype"]) == "entity")
								{
									try
									{
										kry::Util::Vector2f rel;
										rel[0] = kry::Util::toDecimal<float>(objectasset->asset->properties["global"]["relativex"]);
										rel[1] = kry::Util::toDecimal<float>(objectasset->asset->properties["global"]["relativey"]);
										auto pos = coordToExpTileCoord(object->sprite.position + rel * object->sprite.dimensions, Map::getMap()->getCurrentLayer());
										object->hardproperties["entity"]["position"] = "{ " + kry::Util::toString(pos[0]) + ", " + kry::Util::toString(pos[1]) + " }";
									}
									catch (const kry::Util::Exception& e)
									{
										QMessageBox::information(nullptr, "Internal Error", QStringLiteral("Object: ") + e.what(), QMessageBox::Ok);
										QString message;
										Tool<>::switchTool(ToolType::POINTER, message);
										dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusMain()->setText(message);
										follower.sprite.texture = nullptr;
										follower.sprite.rgba = 0.0f;
									}
								}
								/** #TODO(incomplete) also add this to map loading */

								Map::getMap()->getCurrentLayer()->tiles[index].objects.emplace_back(object);
								updateCanvas();
							}
						}
						break;
					case ToolType::PAINT_COPY:
						{
							if (!isValidTileCoord(coord, Map::getMap()->getCurrentLayer()))
								break;
							size_t index = this->tileCoordToIndex(coord, Map::getMap()->getCurrentLayer());
							if (!isValidIndex(index, Map::getMap()->getCurrentLayer()))
								break;

							if (Tool<CopyObjectData>::getTool()->getData().asset == nullptr)
								break;

							auto asset = Tool<CopyObjectData>::getTool()->getData().asset;
							if (asset->type == AssetType::TILE)
							{
								Tile& tile = Map::getMap()->getCurrentLayer()->tiles[index];
								auto id = tile.properties["global"]["id"];
								tile.asset = asset;
								tile.sprite.texture = follower.sprite.texture;
								Tile* o = reinterpret_cast<Tile*>(Tool<CopyObjectData>::getTool()->getData().object);
								tile.properties = o->properties;
								tile.hardproperties = o->hardproperties;
								tile.properties["global"]["id"] = id;
								paintGL();
							}
							else
							{
								Object* object = new Object;
								object->sprite.position = follower.sprite.position;
								object->sprite.texture = follower.sprite.texture;
								object->sprite.dimensions = follower.sprite.texture->getDimensions();
							
								Object* o = Tool<CopyObjectData>::getTool()->getData().object;
								object->properties = o->properties;
								object->hardproperties = o->hardproperties;
								object->waypoints = o->waypoints;
								//object->events = o->events;
								object->asset = o->asset;

								asset = object->asset;object->properties["global"]["id"] = kry::Util::toString(Object::increment++);
								object->properties["global"]["posx"] = kry::Util::toString(follower.sprite.position[0]); /** #TODO(change) this is for map save/load */
								object->properties["global"]["posy"] = kry::Util::toString(follower.sprite.position[1]);
								if (Assets::getParentType(object->properties["global"]["hardtype"]) == "entity")
								{
									try
									{
										kry::Util::Vector2f rel;
										rel[0] = kry::Util::toDecimal<float>(asset->properties["global"]["relativex"]);
										rel[1] = kry::Util::toDecimal<float>(asset->properties["global"]["relativey"]);
										auto pos = coordToExpTileCoord(object->sprite.position + rel * object->sprite.dimensions, Map::getMap()->getCurrentLayer());
										object->hardproperties["entity"]["position"] = "{ " + kry::Util::toString(pos[0]) + ", " + kry::Util::toString(pos[1]) + " }";
									}
									catch (const kry::Util::Exception& e)
									{
										QMessageBox::information(nullptr, "Internal Error", QStringLiteral("Object: ") + e.what(), QMessageBox::Ok);
										QString message;
										Tool<>::switchTool(ToolType::POINTER, message);
										dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusMain()->setText(message);
										follower.sprite.texture = nullptr;
										follower.sprite.rgba = 0.0f;
									}
								}
								/** #TODO(incomplete) also add this to map loading */

								Map::getMap()->getCurrentLayer()->tiles[index].objects.emplace_back(object);
								updateCanvas();
							}
						}
						break;
					case ToolType::WAYPOINT:
						{
							if (!isValidTileCoord(coord, Map::getMap()->getCurrentLayer()))
								break;
							size_t index = this->tileCoordToIndex(coord, Map::getMap()->getCurrentLayer());
							if (!isValidIndex(index, Map::getMap()->getCurrentLayer()))
								break;

							if (Tool<WaypointData>::getTool()->getData().object == nullptr)
								return;

							WaypointData& data = Tool<WaypointData>::getTool()->getData();
							auto cursorpos = canvas.getCoord({(float)event->x(), (float)event->y()});
							if (data.waypoints.size() > 1)
							{
								kry::Util::Vector2f pivot = { 0.09f, 1.0f };
								auto startpoint = data.waypoints[0].position + follower.sprite.dimensions * pivot;
								if (kry::Util::boxPointIntersect(cursorpos, startpoint, follower.sprite.dimensions))
								{
									data.looping = true;
									for (auto& sprite : waypoints[data.object])
										sprite.texture = Resources::getEditorTexture(EditorResource::FLAG_GREEN)->rawresource;
									updateCanvas();
								}
							}
							if (!data.looping)
							{
								kry::Graphics::Sprite sprite = follower.sprite;
								waypoints[data.object].push_back(sprite);
								data.waypoints.push_back({ canvas.getCoord({ static_cast<float>(event->x()), static_cast<float>(event->y()) }), data.object });
								updateCanvas();
							}
						}
						break;
					case ToolType::SELECTION:
						{
							if (!isValidTileCoord(coord, Map::getMap()->getCurrentLayer()))
								break;
							size_t index = this->tileCoordToIndex(coord, Map::getMap()->getCurrentLayer());
							if (!isValidIndex(index, Map::getMap()->getCurrentLayer()))
								break;
							
							auto layer = Map::getMap()->getCurrentLayer();
							auto& data = Tool<SelectionData>::getTool()->getData();
							auto& objects = Tool<SelectionData>::getTool()->getData().objects;

							auto addRemove = [&objects](Object* object, bool checked)
							{
								if (checked)
								{
									objects.insert(object);
									object->sprite.rgba = { 0.0f, 1.0f, 0.0f, 1.0f };
								}
								else
								{
									objects.erase(object);
									object->sprite.rgba = 1.0f;
								}
							};

							Tile& tile = layer->tiles[index];
							if (tile.objects.empty())
								addRemove(&tile, objects.find(&tile) == objects.end());
							else
							{
								QMenu menu(this);
								ObjectAction tileaction(&menu);
								tileaction.setObject(&tile);
								QCheckBox tilecheck(kryToQString(tile.properties["global"]["id"] + ':' + tile.properties["global"]["name"]), &menu);
								tileaction.setDefaultWidget(&tilecheck);
								menu.addAction(&tileaction);
								tilecheck.setChecked(objects.find(&tile) != objects.end());
								connect(&tilecheck, &QCheckBox::clicked, [this, &tileaction, &objects, &addRemove](bool checked)
								{
									addRemove(tileaction.getObject(), checked);
									paintGL();
								});
								menu.addSeparator();
								for (auto object : tile.objects)
								{
									ObjectAction* objectaction = new ObjectAction(&menu);
									QCheckBox* objectcheck = new QCheckBox(kryToQString(object->properties["global"]["id"] + ':' + object->properties["global"]["name"]), &menu);
									objectaction->setDefaultWidget(objectcheck);
									objectaction->setObject(object.get());
									menu.addAction(objectaction);
									objectcheck->setChecked(objects.find(object.get()) != objects.end());
									connect(objectcheck, &QCheckBox::clicked, [this, objectaction, &objects, &addRemove](bool checked)
									{
										addRemove(objectaction->getObject(), checked);
										paintGL();
									});
								}
								menu.exec(event->globalPos());
							}
							auto item = dynamic_cast<MainWindow*>(this->parent()->parent())->getToolbarItems()[0];
							dynamic_cast<QLabel*>(item.widget)->setText("Objects selected: " + QString::number(objects.size()));
							paintGL();
						}
						break;
				}
			}
			else if (event->button() == Qt::RightButton)
			{
				if (handleToolSwitch(event))
					return;

				auto coord = coordToTileCoord(canvas.getCoord({static_cast<float>(event->x()), static_cast<float>(event->y())}), Map::getMap()->getCurrentLayer());
				if (!isValidTileCoord(coord, Map::getMap()->getCurrentLayer()))
					return;
				size_t index = tileCoordToIndex(coord, Map::getMap()->getCurrentLayer());
				if (!isValidIndex(index, Map::getMap()->getCurrentLayer()))
					return;

				Tile& tile = Map::getMap()->getCurrentLayer()->tiles[index];

				QMenu context(this);
				context.addAction(QString::number(coord[0]) + "x" + QString::number(coord[1]));
				QAction* copyaction = new QAction("Copy", &context);
				QAction* editaction = new QAction("Edit", &context);
				QAction* removeaction = new QAction("Remove", &context);

				ObjectAction* tileaction = new ObjectAction(&context);
				QCheckBox* tilecheck = new QCheckBox(&context);
				tileaction->setDefaultWidget(tilecheck);
				tilecheck->setText(kryToQString("Tile: " + tile.properties["global"]["id"] + ':' + tile.asset->properties["global"]["name"]));
				tileaction->setCheckable(true);
				tileaction->setObject(&tile);
				context.addAction(tileaction);
				auto stack = context.addMenu("Objects");
				connect(tileaction, &ObjectAction::hovered, [this, tileaction]()
				{
					tileaction->getObject()->sprite.rgba = {0.0f, 1.0f, 0.0f, 1.0f};
					paintGL();
				});
				connect(tilecheck, &QCheckBox::clicked, [stack, editaction, removeaction](bool checked)
				{
					if (checked)
						for (QAction* action : stack->actions())
							dynamic_cast<QCheckBox*>(dynamic_cast<ObjectAction*>(action)->defaultWidget())->setChecked(false);

					removeaction->setEnabled(!checked);
				});
				for (auto& object : tile.objects)
				{
					ObjectAction* objectaction = new ObjectAction(stack);
					QCheckBox* objectcheck = new QCheckBox(&context);
					objectaction->setDefaultWidget(objectcheck);
					objectcheck->setText(kryToQString(object->properties["global"]["id"] + ':' + object->asset->properties["global"]["name"]));
					objectaction->setCheckable(true);
					objectaction->setObject(object.get());
					connect(objectaction, &ObjectAction::hovered, [this, stack, objectaction, tileaction]()
					{
						tileaction->getObject()->sprite.rgba = 1.0f;
						for (QAction* action : stack->actions())
							dynamic_cast<ObjectAction*>(action)->getObject()->sprite.rgba = 1.0f;
						objectaction->getObject()->sprite.rgba = {0.0f, 1.0f, 0.0f, 1.0f};
						paintGL();
					});
					connect(objectcheck, &QCheckBox::clicked, [objectcheck, tileaction, stack](bool checked)
					{
						if (checked)
						{
							dynamic_cast<QCheckBox*>(dynamic_cast<ObjectAction*>(tileaction)->defaultWidget())->setChecked(false);
							for (QAction* action : stack->actions())
								dynamic_cast<QCheckBox*>(dynamic_cast<ObjectAction*>(action)->defaultWidget())->setChecked(false);
							objectcheck->setChecked(true);
						}
					});
					stack->addAction(objectaction);
				}
				connect(stack, &QMenu::aboutToHide, [this, tileaction, stack]()
				{
					tileaction->getObject()->sprite.rgba = 1.0f;
					for (QAction* action : stack->actions())
						dynamic_cast<ObjectAction*>(action)->getObject()->sprite.rgba = 1.0f;
					paintGL();
				});
				context.addSeparator();

				auto findSelected = [&context, tileaction, stack]()
				{
					ObjectAction* toremove = nullptr;
					if (dynamic_cast<QCheckBox*>(dynamic_cast<ObjectAction*>(tileaction)->defaultWidget())->isChecked())
						toremove = tileaction;
					else
					{
						for (QAction* action : stack->actions())
						{
							if (dynamic_cast<QCheckBox*>(dynamic_cast<ObjectAction*>(action)->defaultWidget())->isChecked())
							{
								toremove = dynamic_cast<ObjectAction*>(action);
								break;
							}
						}
					}

					return toremove;
				};

				// add the name of the thing that was clicked, then a separator
				connect(copyaction, &QAction::triggered, [this, &findSelected, tileaction, event](bool)
				{
					auto toedit = findSelected();
					if (toedit != nullptr)
					{
						QString message;
						Tool<>::switchTool(ToolType::PAINT_COPY, message);
						CopyObjectData data;
						data.size = 1;
						data.asset = toedit->getObject()->asset;
						data.object = toedit->getObject();
						Tool<CopyObjectData>::getTool()->setData(data);
						dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusMain()->setText(message);
					}
				});
				connect(editaction, &QAction::triggered, [this, &findSelected, tileaction, index, &tile, coord](bool)
				{
					auto toedit = findSelected();
					if (toedit != nullptr)
					{
						std::set<Object*> tmp = { toedit->getObject() };
						if (objsettingsDialog->showDialog("ID: " + toedit->getObject()->properties["global"]["id"] + " - " + toedit->getObject()->properties["global"]["name"],
															tmp) == DialogResult::OK)
						{
							auto object = toedit->getObject();
							auto results = objsettingsDialog->getAllProperties();
							auto properties = (*results.begin()).settings;
							auto hardproperties = (*results.begin()).hardtypesettings;
							updateObject(coord, tile, object, properties["global"]["hardtype"], hardproperties);
							object->properties = properties;
							object->hardproperties = hardproperties;
							updateCanvas();
						}
					}
				});
				connect(removeaction, &QAction::triggered, [this, &findSelected, tileaction, stack, index](bool)
				{
					auto toremove = findSelected();
					if (toremove != nullptr)
					{
						if (toremove == tileaction)
							QMessageBox::information(this, "Invalid Action", "You cannot remove a tile.", QMessageBox::Ok);
						else
						{
							auto removeitr = Map::getMap()->getCurrentLayer()->tiles[index].objects.begin();
							auto end = Map::getMap()->getCurrentLayer()->tiles[index].objects.end();
							for (; removeitr != end; ++removeitr)
								if ((*removeitr).get() == toremove->getObject())
									break;
							Map::getMap()->getCurrentLayer()->tiles[index].objects.erase(removeitr);
							stack->removeAction(tileaction);

							updateCanvas();
						}
					}
				});
				context.addAction(copyaction);
				context.addAction(editaction);
				context.addAction(removeaction);

				context.exec(event->globalPos());
				tile.sprite.rgba = 1.0f;
				for (auto object : tile.objects)
					object->sprite.rgba = 1.0f;
			}
		}
		else // mouse is down
		{
			if (Tool<>::getTool()->getType() == ToolType::SELECTION)
			{
				auto layer = Map::getMap()->getCurrentLayer();
				auto& data = Tool<SelectionData>::getTool()->getData();
				auto& objects = Tool<SelectionData>::getTool()->getData().objects;

				auto addRemove = [&objects](Object* object, bool checked)
				{
					if (checked)
					{
						objects.insert(object);
						object->sprite.rgba = { 0.0f, 1.0f, 0.0f, 1.0f };
					}
					else
					{
						objects.erase(object);
						object->sprite.rgba = 1.0f;
					}
				};

				if (data.ctrldown)
				{
					// go through all the objects in the selection and do the same to them as below
					for (auto& tile : layer->tiles)
					{
						auto pivot = getObjectPivot(&tile);
						if (kry::Util::boxPointIntersect(tile.sprite.position + pivot, selection.position, selection.dimensions))
							addRemove(&tile, objects.find(&tile) == objects.end());
						for (auto& object : tile.objects)
						{
							pivot = getObjectPivot(object.get());
							if (kry::Util::boxPointIntersect(object->sprite.position + pivot, selection.position, selection.dimensions))
								addRemove(object.get(), objects.find(object.get()) == objects.end());
						}
					}
				}
			}
		}
        mouseDown = false;
    }

    void GLPanel::mouseMoveEvent(QMouseEvent* event)
    {
		if (!Map::getMap())
			return;

		bool redraw = false;

		if (event->buttons().testFlag(Qt::LeftButton))
		{
			mouseDown = true;
			bool pan = true;
			if (Tool<>::getTool()->getType() == ToolType::SELECTION)
			{
				auto& data = Tool<SelectionData>::getTool()->getData();
				pan = !data.ctrldown;
			}
			if (pan)
			{
				QPoint newpos = event->pos() - dragPos;
				this->canvas.pan += { static_cast<float>(newpos.x()), static_cast<float>(newpos.y()) };
				dragPos = event->pos();
				redraw = true;
			}
		}

		kry::Util::Vector2f canvascoord = {static_cast<float>(event->x()), static_cast<float>(event->y())};
		{
			auto coord = canvas.getCoord(canvascoord);
			dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusPos()->setText(QString::number(coord[0]) + 'x' + QString::number(coord[1]));
			coord = coordToTileCoord(coord, Map::getMap()->getCurrentLayer());
			dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusTile()->setText(QString::number(coord[0]) + 'x' + QString::number(coord[1]));
		}

		size_t tileindex = tileCoordToIndex(coordToTileCoord(canvas.getCoord(canvascoord), Map::getMap()->getCurrentLayer()), Map::getMap()->getCurrentLayer());
		if (isValidIndex(tileindex, Map::getMap()->getCurrentLayer()))
		{
			kry::Util::Vector2f tilecoord = Map::getMap()->getCurrentLayer()->tiles[tileindex].sprite.position;
			switch (Tool<>::getTool()->getType())
			{
				case ToolType::POINTER:
					{
						follower.sprite.texture = nullptr;
						follower.sprite.rgba = 0.0f;
					}
					break;
				case ToolType::PAINT:
					{
						follower.sprite.rgba = 1.0f;
						follower.sprite.position = tilecoord;
						if (Tool<PaintData>::getTool()->getData().objectasset)
						{
							auto objectasset = Tool<PaintData>::getTool()->getData().objectasset;
							auto parent = Assets::getParentType(objectasset->properties["global"]["hardtype"]);
							auto skin = parent == "entity" ? kry::Util::String("skinIdle") : kry::Util::String("skin");
							assert(!objectasset->hardproperties[parent][skin].isEmpty());
							follower.sprite.texture = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(objectasset->hardproperties[parent][skin])]->rawresource;
							follower.sprite.dimensions = follower.sprite.texture->getDimensions();
							follower.asset = objectasset->asset;

							//if (asset->type == AssetType::ENTITY)
							{
								bool snap = true;
								if (objectasset->asset->properties["global"]["gridsnap"] == "false")
									snap = false;

								kry::Util::Vector2f pos = 0.5f;
								pos[0] = kry::Util::toDecimal<float>(objectasset->asset->properties["global"]["relativex"]); /** #TODO(incomplete) use pivot here */
								pos[1] = kry::Util::toDecimal<float>(objectasset->asset->properties["global"]["relativey"]);
								pos = follower.sprite.dimensions * pos; // texture pivot
								if (snap)
									follower.sprite.position += (Map::getMap()->getCurrentLayer()->tilesize / 2) - pos;
								else
									follower.sprite.position = canvas.getCoord(canvascoord) - pos;
							}
						}
						redraw = true;
					}
					break;
				case ToolType::PAINT_COPY:
					{
						follower.sprite.rgba = 1.0f;
						follower.sprite.position = tilecoord;
						if (Tool<CopyObjectData>::getTool()->getData().object != nullptr)
						{
							auto object = Tool<CopyObjectData>::getTool()->getData().object;
							auto parent = Assets::getParentType(object->properties["global"]["hardtype"]);
							kry::Util::String value = parent == "entity" ? object->hardproperties[parent]["skinIdle"] : object->hardproperties[parent]["skin"];
							assert(!value.isEmpty());
							if (value != "-1")
								follower.sprite.texture = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(value)]->rawresource;
							else
								follower.sprite.texture = Resources::getEditorTexture(EditorResource::MISSING_TILE)->rawresource;

							follower.sprite.dimensions = follower.sprite.texture->getDimensions();
							follower.asset = Tool<CopyObjectData>::getTool()->getData().asset;

							bool snap = true;
							if (follower.asset->properties["global"]["gridsnap"] == "false")
								snap = false;

							kry::Util::Vector2f pos = 0.5f;
							pos[0] = kry::Util::toDecimal<float>(follower.asset->properties["global"]["relativex"]); /** #TODO(change) should probably use the pivot from the animation? */
							pos[1] = kry::Util::toDecimal<float>(follower.asset->properties["global"]["relativey"]);
							pos = follower.sprite.dimensions * pos; // texture pivot
							if (snap)
								follower.sprite.position += (Map::getMap()->getCurrentLayer()->tilesize / 2) - pos;
							else
								follower.sprite.position = canvas.getCoord(canvascoord) - pos;
						}
						redraw = true;
					}
					break;
				case ToolType::WAYPOINT:
					{
						if (Tool<WaypointData>::getTool()->getData().object == nullptr)
							break;
						follower.sprite.texture = Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource;
						follower.sprite.dimensions = Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource->getDimensions();
						follower.sprite.rgba = 1.0f;
						kry::Util::Vector2f pos = { 0.09f, 1.0f };
						follower.sprite.position = canvas.getCoord(canvascoord) - follower.sprite.dimensions * pos;
						for (auto& pair : waypoints)
						{
							if (pair.first == Tool<WaypointData>::getTool()->getData().object)
							{
								for (auto& sprite : pair.second)
									sprite.rgba = 1.0f;
								continue;
							}
							for (auto& sprite : pair.second)
							{
								sprite.rgba = { 0.5f, 0.5f, 0.5f, 1.0f }; /** #TODO(note) compiler bug? removing the braces for this loop reveales the bug */
							}
						}
						redraw = true;
					}
					break;
				case ToolType::SELECTION:
					{
						auto& data = Tool<SelectionData>::getTool()->getData();
						if (mouseDown && data.ctrldown)
						{
							kry::Util::Vector2f startpos = canvas.getCoord({ static_cast<float>(lastClick.x()), static_cast<float>(lastClick.y()) });
							kry::Util::Vector2f currpos = canvas.getCoord({ static_cast<float>(event->pos().x()), static_cast<float>(event->pos().y()) });
							selection.rgba = 0.5f;
							selection.position = std::min(startpos, currpos);
							selection.dimensions = std::max(selection.position, currpos) - std::min(selection.position, currpos);
							dragPos = event->pos();
						}
						else
							selection.rgba = 0.0f;
						redraw = true;
					}
					break;
			}
		}

		if (redraw)
			this->paintGL();
		QGLWidget::mouseMoveEvent(event);
    }

    void GLPanel::keyPressEvent(QKeyEvent* event)
    {
        switch(event->key())
        {
            case Qt::Key_Right:
				this->canvas.pan[0] -= 10 * canvas.scale[0]; /** #TODO(change) these scales need changing (add some scroll speed in the settings too) */
                this->paintGL();
                break;
            case Qt::Key_Left:
                this->canvas.pan[0] += 10 * canvas.scale[0];
                this->paintGL();
                break;
            case Qt::Key_Up:
                this->canvas.pan[1] += 10 * canvas.scale[1];
                this->paintGL();
                break;
            case Qt::Key_Down:
                this->canvas.pan[1] -= 10 * canvas.scale[1];
                this->paintGL();
                break;
        }
		if (Tool<>::getTool()->getType() == ToolType::SELECTION)
		{
			auto& data = Tool<SelectionData>::getTool()->getData();
			if (event->key() == Qt::Key_Control)
				data.ctrldown = true;
		}
    }

    void GLPanel::keyReleaseEvent(QKeyEvent* event)
    {
		if (Tool<>::getTool()->getType() == ToolType::SELECTION)
		{
			auto& data = Tool<SelectionData>::getTool()->getData(); 
			if (event->key() == Qt::Key_Control)
				data.ctrldown = false;
		}
    }

	void GLPanel::enterEvent(QEvent*)
	{
		//if (Tool<>::getTool()->getType() == ToolType::PAINT)
		//	this->setCursor(QCursor(Qt::BlankCursor));
	}

	void GLPanel::leaveEvent(QEvent*)
	{
		//if (Tool<>::getTool()->getType() == ToolType::PAINT)
		//	this->setCursor(QCursor(Qt::ArrowCursor));
	}

	size_t GLPanel::tileCoordToIndex(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer)
	{
		return coord[1] * layer->size[1] + coord[0];
    }

	kry::Util::Vector2i GLPanel::coordToTileCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer)
	{
		kry::Util::Vector2f dim = layer->tilesize;
		kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};
		//kry::Util::Vector2f screen = canvas.getCoord(coord);

		int x = (coord[0] / halfdim[0] + coord[1] / halfdim[1]) * 0.5f;
		int y = ((coord[1] / halfdim[1] - coord[0] / halfdim[0]) * 0.5f) * -1;

		return {x, y};
	}

	kry::Util::Vector2f GLPanel::coordToExpTileCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer)
	{
		kry::Util::Vector2f dim = layer->tilesize;
		kry::Util::Vector2f halfdim = { static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f };

		float x = (coord[0] / halfdim[0] + coord[1] / halfdim[1]) * 0.5f;  // black magic goin on here?
		float y = (coord[1] / halfdim[1] - coord[0] / halfdim[0]) * 0.5f * -1;

		return {x, y};
	}

	kry::Util::Vector2f GLPanel::expTileCoordToCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer)
	{
		kry::Util::Vector2f dim = layer->tilesize;
		kry::Util::Vector2f halfdim = { static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f };

		kry::Util::Vector2f tilecoord(static_cast<int>(coord[0]), static_cast<int>(coord[1]));
		auto scalecoord = coord;
		scalecoord[0] -= static_cast<float>(tilecoord[0]);
		scalecoord[1] -= static_cast<float>(tilecoord[1]);
		/** #TODO(incomplete) finish this */
		
		return coord;
	}

	kry::Util::Vector2f GLPanel::tileCoordToCoord(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer)
	{
		kry::Util::Vector2f dim = layer->tilesize;
		kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};

		float x = (coord[0] * halfdim[0] - coord[1] * halfdim[0]) * 0.5f; /** #TODO(incomplete) fix this */
		float y = (coord[0] * halfdim[1] + coord[1] * halfdim[1]) * 0.5f; /** #TODO(complete) */
		//float x = (coord[1] * halfdim[1] - coord[0] * halfdim[0]) * 2.0f + 1.0f;
		//float y = ((coord[1] * halfdim[1] + coord[0] * halfdim[0]) * 2.0f) * -1.0f + 1.0f;
		/*
		kry::Util::Vector2f pos = {coord[0] + 1.0f, coord[1] * -1.0f - 1.0f};
		float x = (pos[0] - pos[1]) * halfdim[0];
		float y = (pos[0] + pos[1]) * halfdim[1] - halfdim[1];*/

		return {x, y};
	}

	bool GLPanel::isValidTileCoord(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer)
	{
		kry::Util::Vector2f size = layer->size;

		return (coord[0] >= 0 && coord[0] < size[0]) && (coord[1] >= 0 && coord[1] < size[1]);
	}

	bool GLPanel::isValidIndex(size_t index, const std::shared_ptr<Map::Layer>& layer)
	{
		return index >= 0 && index < layer->tiles.size();
	}
}
