//
// Purpur Tentakel
// 18.11.2022
//

#include "SceneType.h"

std::string GetStringBySceneType(SceneType sceneType) {
	switch (sceneType) {
		case SceneType::TEST:
			return "TEST";
		case SceneType::LOGO:
			return "LOGO";
		case SceneType::INTRO:
			return "INTRO";
		case SceneType::MAIN_MENU:
			return "MAIN_MENU";
		case SceneType::NEW_GAME_PLAYER:
			return "NEW_GAME_PLAYER";
		case SceneType::NEW_GAME_PARAMETER:
			return "NEW_GAME_PARAMETER";
		case SceneType::SETTINGS:
			return "SETTINGS";
		case SceneType::CREDITS:
			return "CREDITS";
		case SceneType::NONE:
			return "NONE";
		default:
			return "INVALID SCENE";
	}
}