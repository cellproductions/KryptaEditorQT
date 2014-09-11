#include <glew.h>
#include "GLPanel.h"
#include "Map.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Resources.h"
#include "Tool.h"
#include "AssetListItem.h"
#include "ObjectAction.h"
#include "Utilities.h"
#include <Utilities/Math.h>
#include <Graphics/Primitives.h>
#include <QWheelEvent>
#include <QDebug>
#include <QMessageBox>
#include <Windows.h>
#include <functional>

namespace Kryed
{
    QPoint dragPos;
	kry::Graphics::Sprite follower;

    kry::Graphics::Renderer GLPanel::renderer;

    GLPanel::GLPanel(QWidget* parent) : QGLWidget(QGLFormat(QGL::FormatOption::SampleBuffers), parent), empty(true), gridmode(true), mouseDown(false)
    {
		this->setMouseTracking(true);
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
		kry::Util::Vector2f startpos = { static_cast<float>(dim[0]), static_cast<float>(-halfdim[1]) };

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

		for (auto& pair : zorder)
			canvas.addSprite(&pair.second->sprite);

		canvas.addSprite(&follower);
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
    }

    void GLPanel::resizeGL(int w, int h)
    {
        this->canvas.dimensions = {static_cast<float>(w), static_cast<float>(h)};
        this->canvas.setResolution({w, h});
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

			Ui::MainWindow* mainwin = dynamic_cast<MainWindow*>(this->parent()->parent())->getUI();
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

						if (Tool<PaintData>::getTool()->getData().assetitem == nullptr)
							break;
						auto asset = Tool<PaintData>::getTool()->getData().assetitem->asset;
						if (asset->type == AssetType::STATIC_TILE)
						{
							Map::getMap()->getCurrentLayer()->tiles[index].asset = asset;
							Map::getMap()->getCurrentLayer()->tiles[index].sprite.texture = asset->resource->rawresource;
							paintGL();
						}
						else
						{
							Object* object = new Object;
							object->sprite.position = follower.position;//tileCoordToCoord(coord);
							object->sprite.texture = asset->resource->rawresource;
							object->sprite.dimensions = asset->resource->rawresource->getDimensions();
							if (asset->type == AssetType::STATIC_ENTITY || asset->type == AssetType::ANIM_ENTITY)
							{
								kry::Util::Vector2f pos;
								pos[0] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativex"]);
								pos[1] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativey"]);
								object->sprite.position -= object->sprite.dimensions * pos;
							}
							object->properties = asset->properties;
							object->asset = asset;
							Map::getMap()->getCurrentLayer()->tiles[index].objects.emplace_back(object);
							updateCanvas();
						}

					}
					break;
			}
		}
		else if (event->button() == Qt::RightButton)
		{
			if (Tool<>::getTool()->getType() == ToolType::PAINT)
			{
				Tool<>::switchTool(ToolType::POINTER);
				follower.texture = nullptr;
				follower.rgba = 0.0f;
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
			ObjectAction* tileaction = new ObjectAction(&context);
			tileaction->setText("Tile: " + kryToQString(tile.asset->properties["global"]["name"]));
			tileaction->setEnabled(false);
			tileaction->setObject(&tile);
			connect(tileaction, &ObjectAction::hovered, [this, tileaction]()
			{
				tileaction->getObject()->sprite.rgba = {1.0f, 1.0f, 0.0f, 1.0f};
				paintGL();
			});
			context.addAction(tileaction);
			auto stack = context.addMenu("Objects");
			for (auto& object : tile.objects)
			{
				ObjectAction* objectaction = new ObjectAction(stack);
				objectaction->setText(kryToQString(object->asset->properties["global"]["name"]));
				objectaction->setEnabled(false);
				objectaction->setObject(object.get());
				connect(objectaction, &ObjectAction::hovered, [this, stack, objectaction]()
				{
					for (QAction* action : stack->actions())
						dynamic_cast<ObjectAction*>(action)->getObject()->sprite.rgba = 1.0f;
					objectaction->getObject()->sprite.rgba = {1.0f, 1.0f, 0.0f, 1.0f};
					paintGL();
				});
				stack->addAction(objectaction);
			}
			connect(stack, &QMenu::aboutToHide, [tileaction, stack]()
			{
				tileaction->getObject()->sprite.rgba = 1.0f;
				for (QAction* action : stack->actions())
					dynamic_cast<ObjectAction*>(action)->getObject()->sprite.rgba = 1.0f;
			});
			context.addSeparator();
			// add the name of the thing that was clicked, then a separator
			context.addAction("Copy");
			context.addAction("Edit");
			context.addAction("Remove");
			context.exec(event->globalPos());
		}
		//QGLWidget::mousePressEvent(event);
    }

    void GLPanel::mouseReleaseEvent(QMouseEvent* event)
    {
        mouseDown = false;

		//QGLWidget::mouseReleaseEvent(event);
    }

    void GLPanel::mouseMoveEvent(QMouseEvent* event)
    {
		bool redraw = false;

		kry::Util::Vector2f canvascoord = {static_cast<float>(event->x()), static_cast<float>(event->y())};
		canvascoord = canvas.getCoord(canvascoord);

		if (mouseDown)
		{
			QPoint newpos = event->pos() - dragPos;
			this->canvas.pan += { static_cast<float>(newpos.x()), static_cast<float>(newpos.y()) };
			dragPos = event->pos();
			redraw = true;
		}

		switch (Tool<>::getTool()->getType())
		{
			case ToolType::POINTER:
				{
					follower.texture = nullptr;
					follower.rgba = 0.0f;
				}
				break;
			case ToolType::PAINT:
				{
					follower.rgba = 1.0f;
					follower.position = canvascoord;
					follower.position[0] -= static_cast<int>(follower.position[0]) % (Map::getMap()->getCurrentLayer()->tilesize[0] / 2);
					follower.position[1] -= static_cast<int>(follower.position[1]) % (Map::getMap()->getCurrentLayer()->tilesize[1] / 2);
					if (Tool<PaintData>::getTool()->getData().assetitem != nullptr)
					{
						auto asset = Tool<PaintData>::getTool()->getData().assetitem->asset;
						if (asset->type == AssetType::STATIC_ENTITY || asset->type == AssetType::ANIM_ENTITY)
							follower.position = canvascoord;
						follower.dimensions = asset->resource->rawresource->getDimensions();
						follower.texture = asset->resource->rawresource;

						if (asset->type == AssetType::STATIC_ENTITY || asset->type == AssetType::ANIM_ENTITY)
						{
							kry::Util::Vector2f pos;
							pos[0] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativex"]);
							pos[1] = kry::Util::toDecimal<float>(asset->properties["entity"]["relativey"]);
							follower.position -= follower.dimensions * pos;
						}
						else
							follower.position -= asset->resource->rawresource->getDimensions() / 2;
					}
					redraw = true;
				}
				break;
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

	void GLPanel::enterEvent(QEvent* event)
	{
		//if (Tool<>::getTool()->getType() == ToolType::PAINT)
		//	this->setCursor(QCursor(Qt::BlankCursor));
	}

	void GLPanel::leaveEvent(QEvent* event)
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

		int x = static_cast<int>((screen[0] / halfdim[0] + screen[1] / halfdim[1]) * 0.5f) - 1;
		int y = static_cast<int>((screen[1] / halfdim[1] - screen[0] / halfdim[0]) * 0.5f) * -1 - 1;

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
