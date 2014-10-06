#ifndef PIVOTVIEW_H
#define PIVOTVIEW_H

#include <QGraphicsView>

class PivotView : public QGraphicsView
{
		Q_OBJECT
	public:
		explicit PivotView(QWidget *parent = 0);
		~PivotView();

		void setup(const QPixmap& pixmap, qreal pivotx, qreal pivoty);
		void reset();
		void setPivoting(bool pivoting);

	private:
		void mouseMoveEvent(QMouseEvent* event);
		void resizeEvent(QResizeEvent* event);

		QGraphicsScene* scene;
		bool pivoting;
};

#endif // PIVOTVIEW_H
