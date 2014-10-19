#ifndef PIVOTVIEW_H
#define PIVOTVIEW_H

#include <Utilities\Vector.h>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <functional>

struct CrossItem : public QGraphicsItem
{
	QBrush white;

	explicit CrossItem(QGraphicsItem* parent = 0);
	CrossItem(QGraphicsItemPrivate& add, QGraphicsItem* parent);

	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
};

class PivotView : public QGraphicsView
{
		Q_OBJECT
	public:
		explicit PivotView(QWidget *parent = 0);
		~PivotView();

		void setup(const QPixmap& pixmap, qreal pivotx, qreal pivoty);
		void reset();
		void setPivoting(bool pivoting);
		kry::Util::Vector2f getPivot() const;
		inline void setClickCallback(const std::function<void(float x, float y)>& callback);
		void clear();

	private:
		void mouseMoveEvent(QMouseEvent* event);
		void mousePressEvent(QMouseEvent* event);
		void resizeEvent(QResizeEvent* event);
		
		std::function<void(float x, float y)> callback;
		QRect boundingbox;
		QGraphicsScene* scene;
		CrossItem* crossitem;
		bool pivoting;
};

void PivotView::setClickCallback(const std::function<void(float x, float y)>& callback)
{
	this->callback = callback;
}

#endif // PIVOTVIEW_H
