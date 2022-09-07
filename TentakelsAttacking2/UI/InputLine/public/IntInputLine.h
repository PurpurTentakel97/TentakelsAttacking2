//
// Purpur Tentakel
// 06.09.2022
//

#pragma once
#include "InputLine.h"

class IntInputLine : public InputLine {
public:
	using InputLine::InputLine;
	[[nodiscard]] bool IsValidKey(int key) override;
};
