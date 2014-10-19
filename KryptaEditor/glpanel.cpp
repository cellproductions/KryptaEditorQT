#include <glew.h>
#include "GLPanel.h"
#include "Map.h"
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

	kry::Graphics::Canvas GLPanel::canvas;
	kry::Graphics::Canvas GLPanel::waypointcanvas;
    kry::Graphics::Renderer GLPanel::renderer;
	std::map<Object*, std::vector<kry::Graphics::Sprite>> GLPanel::waypoints;

	GLPanel::GLPanel(QWidget* parent) : QGLWidget(QGLFormat(QGL::FormatOption::SampleBuffers), parent), objsettingsDialog(new ObjectSettingsDialog(this)), empty(true),
		gridmode(true), mouseDown(false)
    {
		this->setMouseTracking(true);
		follower.asset = nullptr;
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
				tile.sprite.texture = tile.asset->resource->rawresource;

				zorder.insert(std::pair<kry::Util::Vector3f, Object*>({pos[0], pos[1], 0.0f}, &tile));
				for (auto& object : tile.objects)
				{
					auto& p = object->sprite.position;
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

		if (Tool<>::getTool()->getType() != ToolType::WAYPOINT)
			canvas.addSprite(&follower.sprite); /** #TODO(change) make it so that the follower also follows the zorder rules */

		paintGL();
    }

	void GLPanel::updateWaypointCanvas()
	{
		waypointcanvas.clear();
		for (auto& pair : waypoints)
			for (auto& sprite : pair.second)
				waypointcanvas.addSprite(&sprite);
		waypointcanvas.addSprite(&follower.sprite);

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

    void GLPanel::initializeGL()
    {
		glewInit();

        renderer.addCanvas(&canvas);
		renderer.addCanvas(&waypointcanvas);
		waypointcanvas.rgba[3] = 0.0f;
    }

    void GLPanel::resizeGL(int w, int h)
    {
		this->canvas.dimensions = { static_cast<float>(w), static_cast<float>(h) };
		this->canvas.setResolution({ w, h });
		this->waypointcanvas.dimensions = this->canvas.dimensions;
		this->waypointcanvas.setResolution({ w, h });
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
		if (event->button() == Qt::LeftButton)
		{
			mouseDown = true;
			this->setFocus();
			dragPos = event->pos();

			auto coord = this->coordToTileCoord({(float)event->x(), (float)event->y()});

			switch (Tool<>::getTool()->getType())
			{
				case ToolType::PAINT:
					{
						if (!isValidTileCoord(coord))
							break;
						size_t index = this->tileCoordToIndex(coord);
						if (!isValidIndex(index))
							break;

						if (Tool<PaintData>::getTool()->getData().asset == nullptr)
							break;

						auto asset = Tool<PaintData>::getTool()->getData().asset;
						if (asset->type == AssetType::STATIC_TILE)
						{
							Tile& tile = Map::getMap()->getCurrentLayer()->tiles[index];
							auto id = tile.properties["global"]["id"];
							tile.asset = asset;
							tile.sprite.texture = asset->resource->rawresource;
							if (Tool<CopyObjectData>::getTool()->getData().object != nullptr)
							{
								Tile* o = reinterpret_cast<Tile*>(Tool<CopyObjectData>::getTool()->getData().object);
								tile.properties = o->properties;
								tile.hardproperties = o->hardproperties;
							}
							else
							{
								Map::getMap()->getCurrentLayer()->tiles[index].properties = asset->properties;
								auto type = asset->properties["global"]["hardtype"];
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
									tile.hardproperties[type][key] = "";
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["all"].getKeyNames())
									tile.hardproperties["all"][key] = "";
								for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["floor"].getKeyNames())
									tile.hardproperties["floor"][key] = "";
								unsigned index = 0;
								for (auto& resource : Resources::getAnimations())
								{
									if (resource.get() == asset->resource)
										break;
									++index;
								}
								tile.hardproperties["all"]["skinIdle"] = kry::Util::toString(index);
							}
							tile.properties["global"]["id"] = id;
							paintGL();
						}
						else
						{
							
							Object* object = new Object;
							object->sprite.position = follower.sprite.position;
							object->sprite.texture = asset->resource->rawresource;
							object->sprite.dimensions = asset->resource->rawresource->getDimensions();
							if (asset->type == AssetType::STATIC_ENTITY || asset->type == AssetType::ANIM_ENTITY)
							{
								try
								{
									kry::Util::Vector2f pos;
									pos[0] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativex"]);
									pos[1] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativey"]);
									object->sprite.position = canvas.getCoord({static_cast<float>(event->x()), static_cast<float>(event->y())}) - object->sprite.dimensions * pos;
								}
								catch (const kry::Util::Exception& e)
								{
									QMessageBox::information(nullptr, "Internal Error", QStringLiteral("Entity: ") + e.what(), QMessageBox::Ok);
								}
							}
							if (Tool<CopyObjectData>::getTool()->getData().object != nullptr)
							{
								Object* o = Tool<CopyObjectData>::getTool()->getData().object;
								object->properties = o->properties;
								object->hardproperties = o->hardproperties;
								object->waypoints = o->waypoints;
								object->events = o->events;
								object->asset = o->asset;
								asset = object->asset;
							}
							else
							{
								object->asset = asset;
								object->properties = asset->properties;
								kry::Util::String section;
								if (object->properties.sectionExists("object"))
									section = "object";
								else if (object->properties.sectionExists("entity"))
									section = "entity";
								if (object->properties["global"].keyExists("hardtype"))
								{
									auto type = object->properties["global"]["hardtype"];
									for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
										object->hardproperties[type][key] = "";
									for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["all"].getKeyNames())
										object->hardproperties["all"][key] = "";
									for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["floor"].getKeyNames())
										object->hardproperties["floor"][key] = "";
									unsigned index = 0;
									for (auto& resource : Resources::getAnimations())
									{
										if (resource.get() == asset->resource)
											break;
										++index;
									}
									object->hardproperties["all"]["skinIdle"] = kry::Util::toString(index);
								}
								for (size_t i = 0; i < EventSystem::getSystem()->getEvents().size(); ++i)
									object->events.push_back(parseEvent(EventSystem::getSystem()->getEvents()[i].name, object->properties["events"][kry::Util::toString(i)]));
							}
							object->properties["global"]["id"] = kry::Util::toString(Object::increment++);
							kry::Util::String section;
							if (object->properties.sectionExists("object"))
								section = "object";
							else if (object->properties.sectionExists("entity"))
								section = "entity";
							object->properties[section]["posx"] = kry::Util::toString(follower.sprite.position[0]);
							object->properties[section]["posy"] = kry::Util::toString(follower.sprite.position[1]);
							try
							{
								kry::Util::Vector2f rel;
								rel[0] = kry::Util::toDecimal<float>(asset->properties[section]["relativex"]);
								rel[1] = kry::Util::toDecimal<float>(asset->properties[section]["relativey"]);
								auto pos = coordToExpTileCoord(object->sprite.position + rel * object->sprite.dimensions);
								object->hardproperties["all"]["position"] = "{ " + kry::Util::toString(pos[0]) + ", " + kry::Util::toString(pos[1]) + " }";
							}
							catch (const kry::Util::Exception& e)
							{
								QMessageBox::information(nullptr, "Internal Error", QStringLiteral("Object: ") + e.what(), QMessageBox::Ok);
							}
							/** #TODO(incomplete) also add this to map loading */

							Map::getMap()->getCurrentLayer()->tiles[index].objects.emplace_back(object);
							updateCanvas();
						}
					}
					break;
				case ToolType::WAYPOINT:
					{
							qDebug() << "waypoint click";
						if (!isValidTileCoord(coord))
							break;
						size_t index = this->tileCoordToIndex(coord);
						if (!isValidIndex(index))
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
								
							qDebug() << "looped";
								data.looping = true;
								for (auto& sprite : waypoints[data.object])
									sprite.texture = Resources::getEditorTexture(EditorResource::FLAG_GREEN)->rawresource;
								updateWaypointCanvas();
							}
						}
						if (!data.looping)
						{
							qDebug() << "added";
							kry::Graphics::Sprite sprite = follower.sprite;
							waypoints[data.object].push_back(sprite);
							updateWaypointCanvas();
						}
							qDebug() << "done";
					}
					break;
			}
		}
		else if (event->button() == Qt::RightButton)
		{
			if (Tool<>::getTool()->getType() == ToolType::PAINT)
			{
				Tool<>::switchTool(ToolType::POINTER);
				follower.sprite.texture = nullptr;
				follower.sprite.rgba = 0.0f;
				//updateCanvas();
				paintGL();
				return;
			}
			else if (Tool<>::getTool()->getType() == ToolType::WAYPOINT)
			{
				WaypointData& data = Tool<WaypointData>::getTool()->getData();
				if (data.object == nullptr) // just a safety check
					return;
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
							updateWaypointCanvas();
							context.close();
						});
						context.addAction(removeaction);
						context.exec(event->globalPos());

						return;
					}
					++index;
				}

				auto type = data.object->properties["global"]["hardtype"];
				data.object->hardproperties[type]["loopPath"] = data.looping ? kry::Util::String("true") : kry::Util::String("false");
				kry::Util::String path;
				for (auto& waypoint : data.waypoints)
					path += waypoint.position.toString() + ", ";
				data.object->hardproperties[type]["path"] = path.trim();
				Tool<>::switchTool(ToolType::POINTER);
				follower.sprite.texture = nullptr;
				follower.sprite.rgba = 0.0f;
				waypointcanvas.rgba[3] = 0.0f;
				//updateCanvas();
				paintGL();

				return;
			}

			auto coord = this->coordToTileCoord({(float)event->x(), (float)event->y()});
			if (!isValidTileCoord(coord))
				return;
			size_t index = this->tileCoordToIndex(coord);
			if (!isValidIndex(index))
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
			connect(copyaction, &QAction::triggered, [this, &findSelected, tileaction, index, event](bool)
			{
				auto toedit = findSelected();
				if (toedit != nullptr)
				{
					auto value = Tool<CopyObjectData>::getTool()->getType() == ToolType::PAINT ? Tool<PaintData>::getTool()->getData().size : 1;
					Tool<>::switchTool(ToolType::PAINT);
					CopyObjectData data;
					data.size = value;
					data.asset = toedit->getObject()->asset;
					data.object = toedit->getObject();
					Tool<CopyObjectData>::getTool()->setData(data);
				}
			});
			connect(editaction, &QAction::triggered, [this, &findSelected, tileaction, index](bool)
			{
				auto toedit = findSelected();
				if (toedit != nullptr)
				{
					//if (toedit == tileaction)
					//	QMessageBox::information(this, "Invalid Action", "You cannot edit a tile (incomplete).", QMessageBox::Ok);
					//else
					//{
						if (objsettingsDialog->showDialog(toedit->getObject()->properties["global"]["id"] + " - " + toedit->getObject()->properties["global"]["name"],
														  toedit->getObject()) == DialogResult::OK)
						{
							toedit->getObject()->properties = objsettingsDialog->getSettings();
							toedit->getObject()->hardproperties = objsettingsDialog->getHardTypeSettings();
						}
					//}
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
		}
		//QGLWidget::mousePressEvent(event);
    }

	void GLPanel::mouseReleaseEvent(QMouseEvent*)
    {
        mouseDown = false;

		//QGLWidget::mouseReleaseEvent(event);
    }

    void GLPanel::mouseMoveEvent(QMouseEvent* event)
    {
		if (!Map::getMap())
			return;

		bool redraw = false;

		if (mouseDown)
		{
			QPoint newpos = event->pos() - dragPos;
			this->canvas.pan += { static_cast<float>(newpos.x()), static_cast<float>(newpos.y()) };
			dragPos = event->pos();
			redraw = true;
		}

		kry::Util::Vector2f canvascoord = {static_cast<float>(event->x()), static_cast<float>(event->y())};
		{
			auto coord = canvas.getCoord(canvascoord);
			dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusPos()->setText(QString::number(coord[0]) + 'x' + QString::number(coord[1]));
			coord = coordToTileCoord(canvascoord);
			dynamic_cast<MainWindow*>(this->parent()->parent())->getStatusTile()->setText(QString::number(coord[0]) + 'x' + QString::number(coord[1]));
		}

		size_t tileindex = tileCoordToIndex(coordToTileCoord(canvascoord));
		if (isValidIndex(tileindex))
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
						if (Tool<PaintData>::getTool()->getData().asset != nullptr)
						{
							auto asset = Tool<PaintData>::getTool()->getData().asset;
							follower.sprite.dimensions = asset->resource->rawresource->getDimensions();
							follower.sprite.texture = asset->resource->rawresource;
							follower.asset = asset;

							if (asset->type == AssetType::STATIC_ENTITY || asset->type == AssetType::ANIM_ENTITY)
							{
								kry::Util::Vector2f pos;
								pos[0] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativex"]);
								pos[1] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativey"]);
								follower.sprite.position = canvas.getCoord(canvascoord) - follower.sprite.dimensions * pos;
							}
							else if (asset->type == AssetType::STATIC_TILE_DECAL ||asset->type == AssetType::ANIM_TILE_DECAL)
							{
								kry::Util::Vector2f pos = 0.5f;
								if (asset->properties["object"].keyExists("relativex"))
									pos[0] = kry::Util::toDecimal<float>(asset->properties["object"]["relativex"]);
								if (asset->properties["object"].keyExists("relativey"))
									pos[1] = kry::Util::toDecimal<float>(asset->properties["object"]["relativey"]);
								follower.sprite.position += (Map::getMap()->getCurrentLayer()->tilesize / 2) - follower.sprite.dimensions * pos;
							}
							else if (asset->type == AssetType::STATIC_TILE_OBJECT ||asset->type == AssetType::ANIM_TILE_OBJECT)
							{
								bool snap = true;
								if (asset->properties.sectionExists("object"))
									if (asset->properties["object"].keyExists("gridsnap"))
										if (asset->properties["object"]["gridsnap"] == "false")
											snap = false;

								kry::Util::Vector2f pos = 0.5f;
								if (asset->properties["object"].keyExists("relativex"))
									pos[0] = kry::Util::toDecimal<float>(asset->properties["object"]["relativex"]);
								if (asset->properties["object"].keyExists("relativey"))
									pos[1] = kry::Util::toDecimal<float>(asset->properties["object"]["relativey"]);
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
				case ToolType::WAYPOINT:
					{
						waypointcanvas.rgba[3] = 0.0f;
						follower.sprite.texture = Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource;
						follower.sprite.dimensions = Resources::getEditorTexture(EditorResource::FLAG_RED)->rawresource->getDimensions();
						follower.sprite.rgba = 1.0f;
						kry::Util::Vector2f pos = { 0.09f, 1.0f };
						follower.sprite.position = canvas.getCoord(canvascoord) - follower.sprite.dimensions * pos;

						for (auto& pair : waypoints)
						{
							if (pair.first == Tool<WaypointData>::getTool()->getData().object)
							{
								for (auto& sprite : pair.second) /** #TODO(note) compiler bug? removing the braces for this loop reveales the bug */
								{
									sprite.rgba = 1.0f;
								}
								continue;
							}
							for (auto& sprite : pair.second)
							{
								sprite.rgba = { 0.5f, 0.5f, 0.5f, 1.0f };
							}
						}

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
                this->canvas.pan[0] -= 10 * canvas.scale[0];
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

	size_t GLPanel::tileCoordToIndex(const kry::Util::Vector2i& coord)
	{
		auto& size = Map::getMap()->getCurrentLayer()->size;

		return coord[1] * size[1] + coord[0];
    }

	kry::Util::Vector2i GLPanel::coordToTileCoord(const kry::Util::Vector2f& coord)
	{
		kry::Util::Vector2i dim = Map::getMap()->getCurrentLayer()->tiles[0].asset->resource->rawresource->getDimensions();
		kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};
		kry::Util::Vector2f screen = canvas.getCoord(coord);

		int x = static_cast<int>((screen[0] / halfdim[0] + screen[1] / halfdim[1]) * 0.5f);
		int y = static_cast<int>((screen[1] / halfdim[1] - screen[0] / halfdim[0]) * 0.5f) * -1;

		return {x, y};
	}

	kry::Util::Vector2f GLPanel::coordToExpTileCoord(const kry::Util::Vector2f& coord)
	{
		kry::Util::Vector2i dim = Map::getMap()->getCurrentLayer()->tiles[0].asset->resource->rawresource->getDimensions();
		kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};

		float x = (coord[0] / halfdim[0] + coord[1] / halfdim[1]) * 0.5f;  // black magic goin on here?
		float y = (coord[1] / halfdim[1] - coord[0] / halfdim[0]) * 0.5f * -1;

		return {x, y};
	}

	kry::Util::Vector2f GLPanel::tileCoordToCoord(const kry::Util::Vector2i& coord)
	{
		kry::Util::Vector2i dim = Map::getMap()->getCurrentLayer()->tiles[0].asset->resource->rawresource->getDimensions();
		kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};

		//float x = (coord[1] * halfdim[1] - coord[0] * halfdim[0]) * 2.0f + 1.0f;
		//float y = ((coord[1] * halfdim[1] + coord[0] * halfdim[0]) * 2.0f) * -1.0f + 1.0f;
		kry::Util::Vector2f pos = {coord[0] + 1.0f, coord[1] * -1.0f - 1.0f};
		float x = (pos[0] - pos[1]) * halfdim[0];
		float y = (pos[0] + pos[1]) * halfdim[1] - halfdim[1];

		return {x, y};
	}

	bool GLPanel::isValidTileCoord(const kry::Util::Vector2i& coord)
	{
		auto& size = Map::getMap()->getCurrentLayer()->size;

		return (coord[0] >= 0 && coord[0] < size[0]) && (coord[1] >= 0 && coord[1] < size[1]);
	}

	bool GLPanel::isValidIndex(size_t index)
	{
		return index >= 0 && index < Map::getMap()->getCurrentLayer()->tiles.size();
	}
}
