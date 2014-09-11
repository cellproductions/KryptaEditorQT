#ifndef GLPANEL_H
#define GLPANEL_H

#include <QGLWidget>
#include <Graphics/Renderer.h>
#include <Graphics/Canvas.h>

namespace Kryed
{
    class GLPanel : public QGLWidget
    {
        Q_OBJECT
        public:
            GLPanel(QWidget* parent = nullptr);

            inline kry::Graphics::Canvas& getCanvas();
            void updateCanvas();
            void setGrid(bool gridon);
			void resetCamera();

        protected:
            void initializeGL();
            void resizeGL(int w, int h);
            void paintGL();
            void wheelEvent(QWheelEvent* event);
            void mousePressEvent(QMouseEvent* event);
            void mouseReleaseEvent(QMouseEvent* event);
            void mouseMoveEvent(QMouseEvent* event);
            void keyPressEvent(QKeyEvent* event);
			void enterEvent(QEvent* event);
			void leaveEvent(QEvent* event);

        private:
			size_t tileCoordToIndex(const kry::Util::Vector2i& coord);
			kry::Util::Vector2i coordToTileCoord(const kry::Util::Vector2f& coord);
			kry::Util::Vector2f tileCoordToCoord(const kry::Util::Vector2i& coord);
			bool isValidTileCoord(const kry::Util::Vector2i& coord);
			bool isValidIndex(size_t index);

            kry::Graphics::Canvas canvas;
            bool empty;
            bool gridmode;
            bool mouseDown;

            static kry::Graphics::Renderer renderer;
    };


    kry::Graphics::Canvas& GLPanel::getCanvas()
    {
        return canvas;
    }
}

#endif // GLPANEL_H
