#include "DropListWidget.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

DropListWidget::DropListWidget(QWidget *parent) : QListWidget(parent)
{
}

void DropListWidget::dragEnterEvent(QDragEnterEvent* event)
{

}

void DropListWidget::dropEvent(QDropEvent* event)
{
	/** #TODO(incomplete) maybe add image dropping here */
}
