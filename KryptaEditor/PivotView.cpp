#include "PivotView.h"
#include <QMouseEvent>
#include <QDebug>

CrossItem::CrossItem(QGraphicsItem* parent) : QGraphicsItem(parent), white(QColor::fromRgb(255, 255, 255))
{
}

CrossItem::CrossItem(QGraphicsItemPrivate& add, QGraphicsItem* parent) : QGraphicsItem(add, parent), white(QColor::fromRgb(255, 255, 255))
{
}

QRectF CrossItem::boundingRect() const
{
	return QRectF(x() - 5.0, y() - 5.0, 10.0, 10.0);
}

void CrossItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->drawLine(x() - 5, y(), x() + 5, y());
	painter->drawLine(x(), y() - 5, x(), y() + 5);
	//painter->setBrush(white);
	//painter->drawArc(x()-1, y()-1, 2, 2, 360, 360);
}

qreal scalex = 1.0, scaley = 1.0;

PivotView::PivotView(QWidget *parent) : QGraphicsView(parent), scene(nullptr), crossitem(new CrossItem), pivoting(false)
{
	setMouseTracking(true);

	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	setScene(scene);
}

PivotView::~PivotView()
{
	delete crossitem;
	delete scene;
}

void PivotView::setup(const QPixmap& pixmap, qreal pivotx, qreal pivoty)
{
	clear();
	scene->addPixmap(pixmap);
	scene->addRect(0.0, 0.0, pixmap.width(), pixmap.height());
	boundingbox.setCoords(0, 0, pixmap.width(), pixmap.height());
	crossitem->setPos(pivotx * static_cast<qreal>(pixmap.width()), pivoty * static_cast<qreal>(pixmap.height()));
	scene->addItem(crossitem);
}

void PivotView::reset()
{
	scene->removeItem(crossitem);
	pivoting = false;
}

void PivotView::setPivoting(bool pivoting)
{
	this->pivoting = pivoting;
}

kry::Util::Vector2f PivotView::getPivot() const
{
	return { static_cast<float>(crossitem->x()) / static_cast<float>(boundingbox.width()), 
			static_cast<float>(crossitem->y()) / static_cast<float>(boundingbox.height()) };
}

void PivotView::clear()
{
	for (auto item : scene->items())
	{
		scene->removeItem(item);
		if (item == crossitem)
			continue;
		delete item;
	}
}

void PivotView::mouseMoveEvent(QMouseEvent* event)
{
	if (pivoting)
	{
		auto pos = this->mapToScene(event->x(), event->y());
		qreal x = pos.x() * 0.5f;
		qreal y = pos.y() * 0.5f;
		if (boundingbox.contains(x, y, true))
			crossitem->setPos(x, y);
	}

	QGraphicsView::mouseMoveEvent(event);
}

void PivotView::mousePressEvent(QMouseEvent* event)
{
	pivoting = false;
	if (callback)
	{
		auto pos = this->mapToScene(event->x(), event->y());
		callback(pos.x(), pos.y());
	}

	QGraphicsView::mousePressEvent(event);
}

void PivotView::resizeEvent(QResizeEvent* event)
{
	qreal x = static_cast<qreal>(event->oldSize().width()) / static_cast<qreal>(event->size().width());
	qreal y = static_cast<qreal>(event->oldSize().height()) / static_cast<qreal>(event->size().height());

	//crossItem.setPos(crossItem.x() * x, crossItem.y() * y);
}
