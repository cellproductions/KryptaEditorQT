#ifndef EVENTBUTTONITEM_H
#define EVENTBUTTONITEM_H

#include "EventSystem.h"
#include <QPushButton>

class EventButtonItem : public QPushButton
{
		Q_OBJECT
	public:
		EventButtonItem(QString text, QWidget* parent = 0);

		inline void setEvent(const Event& event);
		inline Event& getEvent();

	private:
		Event event;
};


void EventButtonItem::setEvent(const Event& event)
{
	this->event = event;
}

Event& EventButtonItem::getEvent()
{
	return event;
}

#endif // EVENTBUTTONITEM_H
