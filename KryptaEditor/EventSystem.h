#ifndef EVENT_H
#define EVENT_H

#include <Utilities/StringTypes.h>
#include <QString>
#include <memory>
#include <vector>

class EventSystem
{
	public:
		struct StaticEvent
		{
			unsigned ID;
			QString name;
		};

		enum class ParamType : unsigned char
		{
			NUMBER,
			ASSET,
			COMPARISON,
			OBJECT_ID
		};

		struct StaticParam
		{
			ParamType type;
			QString name;
		};

		struct StaticAction
		{
			QString name;
			std::vector<StaticParam> params;
		};
		typedef StaticAction StaticCondition;

		inline const std::vector<StaticEvent>& getEvents() const;
		inline const std::vector<StaticCondition>& getConditions() const;
		inline const std::vector<StaticAction>& getActions() const;

		static std::shared_ptr<EventSystem> createSystem();
		inline static std::shared_ptr<EventSystem> getSystem();

	private:
		EventSystem();

		std::vector<StaticEvent> events;
		std::vector<StaticCondition> conditions;
		std::vector<StaticAction> actions;

		static std::shared_ptr<EventSystem> single;
};

struct Event;

struct Param
{
	const EventSystem::StaticParam* param;
	QString value;
};

struct Conditional
{
	struct ConditionPair
	{
		const EventSystem::StaticCondition* condition = nullptr;
		std::vector<Param> params;
	};

	std::vector<ConditionPair> blocks;
	std::vector<QString> connectors;
	Event* owner = nullptr;
};

struct ActionPair
{
	Conditional* owner = nullptr;
	const EventSystem::StaticAction* action = nullptr;
	std::vector<Param> params;
};

struct Event
{
	std::vector<Conditional> conditionals;
	std::vector<ActionPair> actions;
	const EventSystem::StaticEvent* event = nullptr;
};

Event parseEvent(const QString& name, const kry::Util::String& string);


const std::vector<EventSystem::StaticEvent>& EventSystem::getEvents() const
{
	return events;
}

const std::vector<EventSystem::StaticCondition>& EventSystem::getConditions() const
{
	return conditions;
}

const std::vector<EventSystem::StaticAction>& EventSystem::getActions() const
{
	return actions;
}

std::shared_ptr<EventSystem> EventSystem::getSystem()
{
	return single;
}

#endif // EVENT_H
