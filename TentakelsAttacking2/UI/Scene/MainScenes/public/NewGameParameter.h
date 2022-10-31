//
// Purpur Tentakel
// 31.10.2022
//

#pragma once
#include "Scene.h"

class NewGameParameterScene : public Scene {
private:
	void Initialize(Vector2 resolution);

	void SetValue(int value, int ID) const;

public:
	NewGameParameterScene(Vector2 resolution);
};