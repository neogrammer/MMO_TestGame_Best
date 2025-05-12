#pragma once
#include <string>
class FSMEvent
{
public:
	std::string name{"Invariant"};

	inline void setName(std::string name_) { name = name_; }
	inline std::string getName() { return name; }
	virtual ~FSMEvent() = 0;
};

