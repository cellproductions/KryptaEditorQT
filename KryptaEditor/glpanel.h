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
			inline static std::map<Object*, std::vector<kry::Graphics::Sprite>>& getWaypoints();
            void updateCanvas();
			void updateWaypointCanvas();
            void setGrid(bool gridon);
			void resetCamera();
			bool handleToolSwitch(QMouseEvent* event = nullptr, bool switchtopointer = true);
			void handleSelectionEdit();
			void handleSelectionRemove();
			void updateObject(const kry::Util::Vector2i& coord, Tile& tile, Object* object, const kry::Util::String& type, kry::Media::Config& hardproperties);

        protected:
            void initializeGL();
            void resizeGL(int w, int h);
            void paintGL();
            void wheelEvent(QWheelEvent* event);
            void mousePressEvent(QMouseEvent* event);
            void mouseReleaseEvent(QMouseEvent* event);
            void mouseMoveEvent(QMouseEvent* event);
            void keyPressEvent(QKeyEvent* event);
            void keyReleaseEvent(QKeyEvent* event);
			void enterEvent(QEvent* event);
			void leaveEvent(QEvent* event);

        private:
			size_t tileCoordToIndex(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer);
			kry::Util::Vector2i coordToTileCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer);
			kry::Util::Vector2f coordToExpTileCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer);
			kry::Util::Vector2f expTileCoordToCoord(const kry::Util::Vector2f& coord, const std::shared_ptr<Map::Layer>& layer);
			kry::Util::Vector2f tileCoordToCoord(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer);
			bool isValidTileCoord(const kry::Util::Vector2i& coord, const std::shared_ptr<Map::Layer>& layer);
			bool isValidIndex(size_t index, const std::shared_ptr<Map::Layer>& layer);

			ObjectSettingsDialog* objsettingsDialog;
            bool empty;
            bool gridmode;
            bool mouseDown;

			static kry::Graphics::Canvas canvas;
            static kry::Graphics::Renderer renderer;
			static std::map<Object*, std::vector<kry::Graphics::Sprite>> waypoints;
    };


    kry::Graphics::Canvas& GLPanel::getCanvas()
    {
        return canvas;
    }

	std::map<Object*, std::vector<kry::Graphics::Sprite>>& GLPanel::getWaypoints()
	{
		return waypoints;
	}
}

#endif // GLPANEL_H
