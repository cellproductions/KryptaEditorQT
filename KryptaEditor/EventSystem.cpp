#include "EventSystem.h"
#include "Utilities.h"
#include <Utilities/BasicString.h>
#include <QDebug>

std::shared_ptr<EventSystem> EventSystem::single;

std::shared_ptr<EventSystem> EventSystem::createSystem()
{
	single.reset(new EventSystem);

	return single;
}

EventSystem::EventSystem()
{
	events.push_back({ 0, "onTouch" });
	events.push_back({ 1, "onGiveItem" });

	conditions.push_back({ "health", { { ParamType::COMPARISON, "comparator" }, { ParamType::NUMBER, "amount" } } });
	conditions.push_back({ "instigator", { { ParamType::OBJECT_ID, "id" } } });

	actions.push_back({ "addScore", { { ParamType::NUMBER, "amount" } } });
	actions.push_back({ "playSound", { { ParamType::ASSET, "soundasset" } } }); // change "file" to "asset"?
	actions.push_back({ "doDamage", { { ParamType::NUMBER, "amount" } } });
}

Event parseEvent(const QString& name, const kry::Util::String& string)
{
	using namespace kry;

	if (name.isEmpty())
		return Event();

	Event event;
	for (const EventSystem::StaticEvent& e : EventSystem::getSystem()->getEvents())
	{
		if (e.name == name)
		{
			event.event = &e;
			break;
		}
	}

	Util::String parsed = string.trim();
	if (parsed.isEmpty())
		return event;
	parsed = parsed.substring(1, parsed.getLength() - 2);

	bool createnew = true;
	unsigned level = 0;
	for (Util::String& token : parsed.explode(' '))
	{
		if (token == "{")
			++level;
		else if (token == "}")
		{
			--level;
			createnew = true; // done with this conditional, add a new one
		}
		else
		{
			if (level == 0)
			{
				if (createnew)
				{
					Conditional newconditional;
					newconditional.owner = &event;
					event.conditionals.push_back(newconditional);
					createnew = false;
				}
				Conditional& last = *event.conditionals.rbegin();

				if (token == "OR" || token == "AND")
					last.connectors.push_back(kryToQString(token));
				else
				{
					Conditional::ConditionPair pair;
					QString name;
					if (token.contains('('))
						name = kryToQString(token.substring(0, token.indexOf('(')));
					else
						continue;

					for (const EventSystem::StaticCondition& c : EventSystem::getSystem()->getConditions())
					{
						if (name == c.name)
						{
							pair.condition = &c;
							break;
						}
					}

					token = token.substring(token.indexOf('('));
					token[0] = ','; // replace brackets with commas
					token[token.getLength() - 1] = ',';
					unsigned index = 0;
					for (Util::String& value : token.explode(','))
					{
						Param param {const_cast<const EventSystem::StaticParam*>(&pair.condition->params[index]), kryToQString(value)};
						pair.params.push_back(param);
						++index;
					}
					last.blocks.push_back(pair);
				}
			}
			else if (level == 1)
			{
				ActionPair action;
				action.owner = event.conditionals.begin()->blocks.empty() ? nullptr : &event.conditionals[event.conditionals.size() - 1];

				QString name = kryToQString(token.substring(0, token.indexOf('(')));
				for (const EventSystem::StaticAction& a : EventSystem::getSystem()->getActions())
				{
					if (name == a.name)
					{
						action.action = &a;
						break;
					}
				}

				token = token.substring(token.indexOf('('));
				token[0] = ','; // replace brackets with commas
				token[token.getLength() - 1] = ',';
				unsigned index = 0;
				for (Util::String& value : token.explode(','))
				{
					action.params.push_back({ &action.action->params[index], kryToQString(value) });
					++index;
				}
				event.actions.push_back(action);
			}
		}
	}

	return event;
}
