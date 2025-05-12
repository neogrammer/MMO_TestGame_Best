#pragma once
#include "IgnoreUnknownEvent.h"
#include <variant>
#include "Events.h"





struct IdleState {};
struct RunningState {};
struct ShootingState {};
struct RunningAndShootingState {};



using PlayerAnimVar = std::variant<IdleState, RunningState, ShootingState, RunningAndShootingState>;
//using AnimVariant = PlayerAnimVar;
