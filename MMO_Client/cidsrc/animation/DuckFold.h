#ifndef DUCK_FOLD_H__
#define DUCK_FOLD_H__
#include "FSM.h"

template<typename Fsm, typename... Events>
void dispatch(Fsm& fsm, Events&&... events)
{
	(fsm.dispatch(std::forward<Events>(events)), ...);
}

#endif