#include "PivotView.h"
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QDebug>

struct CrossItem : public QGraphicsItem
{
	QBrush white;

	explicit CrossItem(QGraphicsItem* parent = 0) : QGraphicsItem(parent), white(QColor::fromRgb(255, 255, 255))
	{
	}

	CrossItem(QGraphicsItemPrivate& add, QGraphicsItem* parent) : QGraphicsItem(add, parent), white(QColor::fromRgb(255, 255, 255))
	{
	}

	QRectF boundingRect() const
	{
		return QRectF(x() - 5.0, y() - 5.0, 10.0, 10.0);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
	{
		painter->drawLine(x() - 5, y(), x() + 5, y());
		painter->drawLine(x(), y() - 5, x(), y() + 5);
		//painter->setBrush(white);
		//painter->drawArc(x()-1, y()-1, 2, 2, 360, 360);
	}
} crossItem;

qreal scalex = 1.0, scaley = 1.0;

PivotView::PivotView(QWidget *parent) : QGraphicsView(parent), scene(nullptr), pivoting(false)
{
	setMouseTracking(true);

	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene);
}

PivotView::~PivotView()
{
	delete scene;
}

void PivotView::setup(const QPixmap& pixmap, qreal pivotx, qreal pivoty)
{
	clear();
	scene->addPixmap(pixmap);
	scene->addRect(0.0, 0.0, pixmap.width(), pixmap.height());
	crossItem.setPos(pivotx * static_cast<qreal>(pixmap.width()), pivoty * static_cast<qreal>(pixmap.height()));
	scene->addItem(&crossItem);
}

void PivotView::reset()
{
	scene->removeItem(&crossItem);
	pivoting = false;
}

void PivotView::setPivoting(bool pivoting)
{
	this->pivoting = pivoting;
}

void PivotView::clear()
{
	for (auto item : scene->items())
	{
		scene->removeItem(item);
		if (item == &crossItem)
			continue;
		delete item;
	}
}

void PivotView::mouseMoveEvent(QMouseEvent* event)
{
	if (pivoting)
		crossItem.setPos(event->x(), event->y());

	QGraphicsView::mouseMoveEvent(event);
}

void PivotView::resizeEvent(QResizeEvent* event)
{
	qreal x = static_cast<qreal>(event->oldSize().width()) / static_cast<qreal>(event->size().width());
	qreal y = static_cast<qreal>(event->oldSize().height()) / static_cast<qreal>(event->size().height());

	//crossItem.setPos(crossItem.x() * x, crossItem.y() * y);
}
