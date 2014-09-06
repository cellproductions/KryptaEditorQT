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

        private:
            size_t coordToIndex(const kry::Util::Vector2f& coord);

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
