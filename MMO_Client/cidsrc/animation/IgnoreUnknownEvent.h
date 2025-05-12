#pragma once
#include <optional>

struct IgnoreUnknownEvent
{
	template<typename State,  typename Event>
	auto operator()(const State&, const Event&)
	{
		return std::nullopt;
	}
};