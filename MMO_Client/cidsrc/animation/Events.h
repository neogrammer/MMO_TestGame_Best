#pragma once
#include <variant>
#include "FSMEvent.h"
struct EventMoved : FSMEvent { EventMoved() : FSMEvent() { setName("Moved"); } };
struct EventStoppedMoving : FSMEvent { EventStoppedMoving() : FSMEvent() { setName("StoppedMoving"); } };
struct EventStoppedShooting : FSMEvent {EventStoppedShooting() : FSMEvent() { setName("StoppedShooting"); } };
struct EventShoot : FSMEvent { EventShoot() : FSMEvent() { setName("Shoot"); } };



using PlayerEventVar = std::variant<EventMoved, EventStoppedMoving, EventStoppedShooting, EventShoot>;
