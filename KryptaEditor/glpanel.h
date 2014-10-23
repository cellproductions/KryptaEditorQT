#ifndef GLPANEL_H
#define GLPANEL_H

#include <QGLWidget>
#include <Graphics/Renderer.h>
#include <Graphics/Canvas.h>
#include <Map.h>

class ObjectSettingsDialog;
struct Object;

namespace Kryed
{
    class GLPanel : public QGLWidget
    {
        Q_OBJECT
        public:
            GLPanel(QWidget* parent = nullptr);

			inline static kry::Graphics::Canvas& getCanvas();
			inline static kry::Graphics::Canvas& getWaypointCanvas();
			inline static std::map<Object*, std::vector<kry::Graphics::Sprite>>& getWaypoints();
            void updateCanvas();
			void updateWaypointCanvas();
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
			size_t tileCoordToIndex(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer);
			kry::Util::Vector2i coordToTileCoord(const kry::Util::Vector2f& coord);
			kry::Util::Vector2f coordToExpTileCoord(const kry::Util::Vector2f& coord);
			kry::Util::Vector2f tileCoordToCoord(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer);
			bool isValidTileCoord(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer);
			bool isValidIndex(size_t index, const std::shared_ptr<Map::Layer>& layer);

			ObjectSettingsDialog* objsettingsDialog;
            bool empty;
            bool gridmode;
            bool mouseDown;

			static kry::Graphics::Canvas canvas;
			static kry::Graphics::Canvas waypointcanvas;
            static kry::Graphics::Renderer renderer;
			static std::map<Object*, std::vector<kry::Graphics::Sprite>> waypoints;
    };


    kry::Graphics::Canvas& GLPanel::getCanvas()
    {
        return canvas;
    }

    kry::Graphics::Canvas& GLPanel::getWaypointCanvas()
    {
        return waypointcanvas;
    }

	std::map<Object*, std::vector<kry::Graphics::Sprite>>& GLPanel::getWaypoints()
	{
		return waypoints;
	}
}

#endif // GLPANEL_H
