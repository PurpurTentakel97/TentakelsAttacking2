//
// Purpur Tentakel
// 31.08.2022
//

#pragma once

#include "AssetType.h"
#include <raylib.h>
#include <array>
#include <unordered_map>
#include <string>
#include <vector>

class AssetManager {
private:
	std::array<std::string, 6> m_files = {
		"Assets/btn_f_default.png",
		"Assets/grey.png",
		"Assets/grey_50.png",
		"Assets/exclamation_mark.png",
		"Assets/question_mark.png",
		"Assets/logo.png",
	};
	std::unordered_map<AssetType, Texture2D> m_assets;
	std::vector<std::string> m_title;
	Font m_font;

	void LoadTitle();
	void LoadFont();

public:
	AssetManager();
	~AssetManager();

	[[nodiscard]] Texture* GetTexture(AssetType assetType);
	std::vector<std::string>* GetTitle();
	[[nodiscard]] Font const* GetFont() const;
};