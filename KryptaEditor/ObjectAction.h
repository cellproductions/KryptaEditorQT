#ifndef OBJECTACTION_H
#define OBJECTACTION_H

#include <QAction>

struct Object;

class ObjectAction : public QAction
{
		Q_OBJECT
	public:
		explicit ObjectAction(QObject *parent = 0);

		inline void setObject(Object* object);
		inline Object* getObject();

	private:
		Object* object;
};


void ObjectAction::setObject(Object* object)
{
	this->object = object;
}

Object* ObjectAction::getObject()
{
	return object;
}

#endif // OBJECTACTION_H
