#ifndef PLAYERANIMFSM_H__
#define PLAYERANIMFSM_H__
#include "DuckFold.h"
#include <optional>
#include "PlayerAnimStates.h"
#include <type_traits>
#include <variant>
#include <iostream>

class FSM_Player : public FSM<FSM_Player, PlayerAnimVar>
{
public:

	//std::optional<PlayerAnimVar> On_Event(IdleState& s, const EventMoved& e) { std::cout << "IdleToRun" << std::endl; return IdleToRunState{}; }
	
	//std::optional<PlayerAnimVar> On_Event(RunningState& s, const EventMoved& e) { std::cout << "IdleToRun" << std::endl; return IdleToRunState{}; }

	//std::optional<PlayerAnimVar> On_Event(ShootingState& s, const EventMoved& e) { std::cout << "IdleToRun" << std::endl; return IdleToRunState{}; }

	//std::optional<PlayerAnimVar> On_Event(RunningAndShootingState& s, const EventMoved& e) { std::cout << "IdleToRun" << std::endl; return IdleToRunState{}; }



	template <typename State, typename Event>
	std::optional<PlayerAnimVar> On_Event(State&, const Event&) { 	return std::nullopt; 	}
	std::string getStateName()
	{
		if (std::holds_alternative<IdleState>(state_)) { return "Idle"; }
		else if (std::holds_alternative<RunningState>(state_)) { return "Running"; }
		else if (std::holds_alternative<ShootingState>(state_)) { return "Shooting"; }
		else if (std::holds_alternative<RunningAndShootingState>(state_)) { return "RunningAndShooting"; }
		else { return "Invariant"; }
	}
};
#endif