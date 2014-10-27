#ifndef OBJECTLISTITEM_H
#define OBJECTLISTITEM_H

#include <QListWidgetItem>
#include <memory>

struct Object;

struct ObjectListItem : public QListWidgetItem
{
	ObjectListItem(std::shared_ptr<Object> object, const QIcon& icon, const QString& text);
	std::shared_ptr<Object> object;
};

#endif
