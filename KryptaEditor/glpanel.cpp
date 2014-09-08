#include <glew.h>
#include "GLPanel.h"
#include "Map.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Resources.h"
#include "Tool.h"
#include "AssetListItem.h"
#include <Utilities/Math.h>
#include <Graphics/Primitives.h>
#include <QWheelEvent>
#include <QDebug>
#include <QMessageBox>
#include <Windows.h>

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
		//auto dim = tiles[0].asset->resource->rawresource->getDimensions();
		auto dim = Map::getMap()->getCurrentLayer()->tilesize;
		auto halfdim = dim / 2;
		kry::Util::Vector2f startpos = { static_cast<float>(dim[0]), static_cast<float>(-halfdim[1]) };
kry::Util::Vector4f col(0.0f, 1.0f, 0.0f, 1.0f);
int coli = 0;
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
                tile.sprite.rgba = col;
                col[coli] += 0.01f;
                if (col[coli] >= 1.0f)
                {
                    coli += 1;
                    if (coli > 2)
                    {
                        coli = 0;
                        col[0] = 0.0f;
                        col[1] = 0.0f;
                        col[2] = 0.0f;
                    }
                }

                canvas.addSprite(&tile.sprite);
                if (gridmode)
                {
                    // render a grid? iono
                }

				pos[0] += halfdim[0];
				pos[1] += halfdim[1];
            }
        }

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
		canvas.pan = 0.0f;
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
			coordToIndex({(float)event->x(), (float)event->y()});
		}
		else if (event->button() == Qt::RightButton)
		{
			QMenu context(this);
			// add the name of the thing that was clicked, then a separator
			context.addAction("Copy");
			context.addAction("Edit");
			context.addAction("Remove");
			context.exec(event->pos());
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

        if (mouseDown)
        {
            QPoint newpos = event->pos() - dragPos;
            this->canvas.pan += { static_cast<float>(newpos.x()), static_cast<float>(newpos.y()) };
            dragPos = event->pos();
			redraw = true;
        }

		if (Tool<>::getTool()->getType() == ToolType::PAINT)
		{
			follower.position = {static_cast<float>(event->x()), static_cast<float>(event->y())};
			follower.position = canvas.getCoord(follower.position);
			follower.position[0] -= static_cast<int>(follower.position[0]) % (Map::getMap()->getCurrentLayer()->tilesize[0] / 2);
			follower.position[1] -= static_cast<int>(follower.position[1]) % (Map::getMap()->getCurrentLayer()->tilesize[1] / 2);
			if (Tool<PaintData>::getTool()->getData().assetitem != nullptr)
			{
				follower.dimensions = Tool<PaintData>::getTool()->getData().assetitem->asset->resource->rawresource->getDimensions();
				follower.texture = Tool<PaintData>::getTool()->getData().assetitem->asset->resource->rawresource;
			}
			redraw = true;
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

    size_t GLPanel::coordToIndex(const kry::Util::Vector2f& coord)
	{
		auto& size = Map::getMap()->getCurrentLayer()->size;
		kry::Util::Vector2i dim = Map::getMap()->getCurrentLayer()->tiles[0].asset->resource->rawresource->getDimensions();
		kry::Util::Vector2f halfdim = {static_cast<float>(dim[0]) * 0.5f, static_cast<float>(dim[1]) * 0.5f};
		kry::Util::Vector2f screen = canvas.getCoord(coord);

		int x = static_cast<int>((screen[0] / halfdim[0] + screen[1] / halfdim[1]) * 0.5f) - 1;
		int y = static_cast<int>((screen[1] / halfdim[1] - screen[0] / halfdim[0]) * 0.5f) * -1 - 1;

		return y * size[1] + x;
    }
}
