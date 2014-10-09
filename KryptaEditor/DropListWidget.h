#ifndef DROPLISTWIDGET_H
#define DROPLISTWIDGET_H

#include <QListWidget>

class DropListWidget : public QListWidget
{
		Q_OBJECT
	public:
		explicit DropListWidget(QWidget *parent = 0);

	signals:


	protected:
		virtual void dragEnterEvent(QDragEnterEvent* event);
		virtual void dropEvent(QDropEvent* event);
};

#endif // DROPLISTWIDGET_H
