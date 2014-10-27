#include "ObjectListItem.h"


ObjectListItem::ObjectListItem(std::shared_ptr<Object> object, const QIcon& icon, const QString& text) : QListWidgetItem(icon, text), object(object)
{
}
