#ifndef FSM_H__
#define FSM_H__
#include <variant>
#include <optional>
#include "Events.h"

#include <string>

template <typename Derived, typename StateVariant>
class FSM
{
protected:
	StateVariant state_;
public:
	template<typename Event>
	void dispatch(Event&& event)
	{
		Derived& child = static_cast<Derived&>(*this);
		auto new_state = std::visit(
			[&](auto& s) -> std::optional<StateVariant>
			{
				return child.On_Event(s, std::forward<Event>(event));
			}, state_);
		if (new_state)
		{
			state_ = *std::move(new_state);
		}
	}
};

#endif