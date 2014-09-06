#include <glew.h>
#include "glpanel.h"
#include "map.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resources.h"
#include <Graphics/Primitives.h>
#include <QWheelEvent>
#include <QDebug>
#include <QMessageBox>
#include <Windows.h>

namespace Kryed
{
    QPoint dragPos;

    kry::Graphics::Renderer GLPanel::renderer;

    GLPanel::GLPanel(QWidget* parent) : QGLWidget(QGLFormat(QGL::FormatOption::SampleBuffers), parent), empty(true), gridmode(true), mouseDown(false)
    {
    }

    void GLPanel::updateCanvas()
    {
        empty = false;

        canvas.clear();
		kry::Util::Vector2f pos;
        auto& size = Map::getMap()->getCurrentLayer()->size;
        auto& tiles = Map::getMap()->getCurrentLayer()->tiles;
        auto dim = tiles[0].asset->resource->rawresource->getDimensions();
        auto halfdim = dim / 2;
		kry::Util::Vector2f startPos = { -(dim[0] * static_cast<float>(size[0]) * 0.5f),
										0.0f};
        float ylimit = halfdim[1] * static_cast<float>(size[1]);
kry::Util::Vector4f col(0.0f, 0.0f, 0.0f, 1.0f);
int coli = 0;
        for (int y = 0; y < size[1]; ++y)
        {
			pos = startPos + (halfdim * y);
            //pos[0] = halfdim[0] * y;
            //pos[1] = ylimit - halfdim[1] * y;

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
                pos[1] -= halfdim[1];
            }
        }
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
        mouseDown = true;
        this->setFocus();
        dragPos = event->pos();

        Ui::MainWindow* mainwin = dynamic_cast<MainWindow*>(this->parent()->parent())->getUI();
        //mainwin->lEnvName =
        coordToIndex({(float)event->x(), (float)event->y()});
        QGLWidget::mousePressEvent(event);
    }

    void GLPanel::mouseReleaseEvent(QMouseEvent* event)
    {
        mouseDown = false;

        QGLWidget::mouseReleaseEvent(event);
    }

    void GLPanel::mouseMoveEvent(QMouseEvent* event)
    {
        if (mouseDown)
        {
            QPoint newpos = event->pos() - dragPos;
            this->canvas.pan += { static_cast<float>(newpos.x()), static_cast<float>(newpos.y()) };
            dragPos = event->pos();
            this->paintGL();
        }

        //QGLWidget::mouseMoveEvent(event);
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

    size_t GLPanel::coordToIndex(const kry::Util::Vector2f& coord)
    {
        kry::Util::Vector2f pos = this->canvas.getCoord(coord);

        auto& size = Map::getMap()->getCurrentLayer()->size;
        auto& tiles = Map::getMap()->getCurrentLayer()->tiles;
        auto dim = tiles[0].asset->resource->rawresource->getDimensions();
        auto halfdim = dim / 2;

        pos[1] = (pos[1] - halfdim[0]) / halfdim[1];
        pos[0] = (pos[0] - halfdim[1]) / halfdim[0];

        qDebug() << pos[0] << pos[1];


        return 0;

        //auto& size = Map::getMap()->getCurrentLayer()->size;
        //auto dim = Map::getMap()->getCurrentLayer()->tiles[0].asset->resource->rawresource->getDimensions();
        //auto pos = this->canvas.getCoord({coord[0], coord[1]});
        //if (pos[0] < 0 || pos[1] < 0)
        //    return -1;
        //kry::Util::Vector2f iso = { pos[0] - pos[1] + dim[0] * 0.5f, (pos[0] + pos[1]) * 0.5f + dim[1] * 0.5f };
        //QMessageBox::information(this, "coords", QString() + QString::number(iso[0]) + ", " + QString::number(iso[1]), QMessageBox::Ok);
        /*float c = cos(-0.7853982f);
        float s = sin(-0.7853982f);
        pos[0] = (pos[0] * c) - (pos[1] * s);
        pos[1] = (pos[0] * s) + (pos[1] * c);*/
        //pos = (size * Map::getMap()->getTiles()[0].asset->resource->getDimensions()) / pos;

        //pos /= Map::getMap()->getTiles()[0].asset->resource->getDimensions();

        //return pos[1] * size[0] + pos[0];
        //pos /= dim;
        //kry::Util::Vector2f iso = { pos[0] - pos[1], (pos[0] + pos[1]) * 0.5f };
        //QMessageBox::information(this, "coords", QString() + QString::number(iso[0]) + ", " + QString::number(iso[1]), QMessageBox::Ok);
        //return pos[1] * size[0] + pos[1];
    }
}
