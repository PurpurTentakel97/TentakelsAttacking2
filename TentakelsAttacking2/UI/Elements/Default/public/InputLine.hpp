//
// Purpur Tentakel
// 06.09.2022
//

#pragma once
#include "UIElement.hpp"
#include "Focusable.h"
#include "AppContext.h"
#include "HTextProcessing.h"
#include "HInput.h"
#include <raylib.h>
#include <string>
#include <functional>

/**
 * provides text input.
 */
template <class T>
class InputLine final : public UIElement, public Focusable {
protected:
	bool m_isEnabled = true; ///< contains if the inputline is enabled
	unsigned int m_charLimit; ///< contains the max abount of chars in the inputline
	std::string m_value; ///< contains the current value
	std::string m_oldValue; ///< contains the old value
	std::string m_placeholderText; ///< contains the placeholder text -> gets showed when no value is provided
	Texture* m_texture; ///< contauns the background texture
	double m_backspacePressTime = 0.0; ///< contains the time since the last time backspace got pressed and still hold
	std::function<void()> m_onEnter = []() {}; ///< contains the onEnter lambda gets called then enter is pressed

	/**
	 * checks wether the inputline is still full.
	 * if not the char is edded.
	 * returns if a char was edded.
	 */
	bool AddChar(int key) {
		bool validAdd = m_charLimit > m_value.size();

		if (validAdd) {
			m_value += static_cast<char>(key);

			auto event = PlaySoundEvent(SoundType::TEXT);
			AppContext::GetInstance().eventManager.InvokeEvent(event);
		}

		return validAdd;
	}
	/**
	 * removes a char if the inputline is not empty.
	 * if it is empty: nothing happens.
	 */
	void RemoveChar() {
		if (m_value.size() != 0) {
			m_value.pop_back();

			auto event = PlaySoundEvent(SoundType::TEXT);
			AppContext::GetInstance().eventManager.InvokeEvent(event);
		}
	}
	/**
	 * gets deleted here because it is overloaded for each of the provided datatypes.
	 * checks if a spacific key is a valid key.
	 */
	[[nodiscard]] bool IsValidKey(int key) = delete;

public:
	/**
	 * ctor.
	 * loads texture.
	 */
	InputLine(unsigned int focusID, Vector2 pos, Vector2 size, Alignment alignment,
		Vector2 resolution, unsigned int charLimit)
		: UIElement(pos, size, alignment, resolution), Focusable(focusID), m_charLimit(charLimit) {
		m_texture = AppContext::GetInstance().assetManager.GetTexture(AssetType::GREY);
	}
	InputLine(InputLine const&) = default;
	InputLine(InputLine&&) = default;
	InputLine& operator= (InputLine const&) = default;
	InputLine& operator= (InputLine&&) = default;

	/**
	 * returs if the inputline is enabled.
	 */
	[[nodiscard]] virtual bool IsEnabled() const override {
		return true;
	}

	/**
	 * logic of the inputline.
	 */
	void CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) override {

		UIElement::CheckAndUpdate(mousePosition, appContext);

		if (!m_isEnabled) { return; }

		bool hover = CheckCollisionPointRec(mousePosition, m_collider);
		bool validSelect = !IsFocused() and
			hover and IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

		if (validSelect) {
			auto event = SelectFocusElementEvent(this);
			appContext.eventManager.InvokeEvent(event);
		}

		if (!IsFocused()) { return; }

		bool enter = IsOnlyEnterConfirmInputPressed();
		if (enter) {
			m_onEnter();
		}

		if (IsKeyPressed(KEY_BACKSPACE)) {
			RemoveChar();
			m_backspacePressTime = GetTime();
			return;
		}

		if (IsKeyDown(KEY_BACKSPACE)) {
			if (m_backspacePressTime < GetTime() - 0.5) {
				RemoveChar();
				return;
			}
		}

		while (true) {
			int key = GetCharPressed();

			if (key <= 0) { break; }

			if (!IsValidKey(key)) { continue; }

			if (!AddChar(key)) {
				auto event = ShowMessagePopUpEvent("Max Input", "Maximum number of input values reached");
				appContext.eventManager.InvokeEvent(event);
				break;
			}
		}
	}
	/**
	 * renders the inputline
	 */
	void Render(AppContext const& appContext) override {

		auto constants = appContext.constants.textProcecing;
		// Update here to make shure its after call of HasValueChanced();
		m_oldValue = m_value;

		Rectangle textureRec = {
			0.0f,
			0.0f,
			static_cast<float>(m_texture->width),
			static_cast<float>(m_texture->height)
		};

		DrawTexturePro(
			*m_texture,
			textureRec,
			m_collider,
			Vector2(0.0f, 0.0f),
			0.0f,
			WHITE
		);

		if (m_isEnabled) {
			DrawRectangleLinesEx(
				m_collider,
				2.0f,
				PURPLE
			);
		}

		float posX = m_collider.x + 10.0f;
		float posY = m_collider.y + m_collider.height * 0.1f;
		float fontSize = m_collider.height * 0.8f;
		std::string printableInput;

		if (m_value.size() > 0) {
			printableInput = GetPritableTextInColider(
				m_value,
				fontSize,
				m_collider,
				appContext
			);
			DrawTextEx(
				*(appContext.assetManager.GetFont()),
				printableInput.c_str(),
				Vector2(posX, posY),
				fontSize,
				0,
				WHITE
			);
		}
		else {
			std::string printablePlaceholder = GetPritablePlaceholderTextInColider(
				m_placeholderText,
				fontSize,
				m_collider,
				appContext
			);
			DrawTextEx(
				*(appContext.assetManager.GetFont()),
				printablePlaceholder.c_str(),
				Vector2(posX, posY),
				fontSize,
				0,
				GRAY
			);
		}


		if (IsFocused()) {
			size_t time = static_cast<size_t>(GetTime() * 2.0);
			Vector2 textLength = MeasureTextEx(
				*(appContext.assetManager.GetFont()),
				printableInput.c_str(),
				fontSize,
				0.0f
			);

			if (time % 2 == 0) {
				DrawTextEx(
					*(appContext.assetManager.GetFont()),
					&constants.cursor,
					Vector2(
						posX + constants.cursorOffset + textLength.x,
						posY + m_collider.height * 0.05f
					),
					fontSize,
					0,
					PURPLE
				);
			}
		}
	}

	/**
	 * sets a new placeholder text.
	 * it will be renderd if the inputline is empty.
	 */
	void SetPlaceholderText(std::string placeholderText) {
		m_placeholderText = placeholderText;
	}
	/**
	 * sets a value into the inputline.
	 */
	void SetValue(T value) {
		m_value = std::to_string(value);
	}
	/**
	 * gets deleted here because it is overloaded for each of the provided datatypes.
	 * returns the value.
	 */
	[[nodiscard]] T GetValue() = delete;
	/**
	 * clears the inputline.
	 */
	void Clear() {
		m_value.clear();
	}

	/**
	 * returns the current collider.
	 */
	[[nodiscard]] Rectangle GetCollider() const override {
		return UIElement::GetCollider();
	}
	/**
	 * returns if the inputline has any value.
	 */
	[[nodiscard]] bool HasValue() const {
		return !m_value.empty();
	}
	/**
	 * returns if the value has chanced since the last tick.
	 */
	[[nodiscard]] bool HasValueChanced() const {
		return m_value != m_oldValue;
	}
	/**
	 * sets the onEnter lambda that is called when enter is pressed.
	 */
	void SetOnEnter(std::function<void()> onEnter) {
		m_onEnter = onEnter;
	}

	/**
	 * sets if the inputline is enabled.
	 */
	void SetEnabled(bool isEnabled) {
		m_isEnabled = isEnabled;
	}
};

/**
 * checks if its a valid key for a int inputline.
 */
template<>
inline bool InputLine<int>::IsValidKey(int key) {
	bool valid = key >= 48 and
		key <= 57;

	return valid;
}
/**
 * checks if its a valid key for a float inputline.
 */
template<>
inline bool InputLine<float>::IsValidKey(int key) {
	bool valid = (key >= 48 and key <= 57) // numbers
		or key == 44 // comma
		or key == 46; // dot

	//check for multiple commas/dots
	if ('.' == key or ',' == key) {
		if (m_value.size() == 0) {
			valid = false;
		}

		for (char c : m_value) {
			if ('.' == c or ',' == c) {
				valid = false;
				break;
			}
		}
	}

	return valid;
}
/**
 * checks if its a valid key for a double inputline.
 */
template<>
inline bool InputLine<double>::IsValidKey(int key) {
	bool valid = (key >= 48 and key <= 57) // numbers
		or key == 44 // comma
		or key == 46; // dot

	//check for multiple commas/dots
	if ('.' == key or ',' == key) {
		if (m_value.size() == 0) {
			valid = false;
		}

		for (char c : m_value) {
			if ('.' == c or ',' == c) {
				valid = false;
				break;
			}
		}
	}

	return valid;
}
/**
 * checks if its a valid key for a string inputline.
 */
template<>
inline bool InputLine<std::string>::IsValidKey(int key) {
	bool valid = key >= 32 and
		key <= 126;

	return valid;
}

/**
 * returns the value for a int input line.
 */
template<>
[[nodiscard]] inline int InputLine<int>::GetValue() {
	StripString(m_value);
	if (m_value.size() == 0) {
		return 0;
	}
	return std::stoi(m_value);
}
/**
 * returns the value for a float input line.
 */
template<>
[[nodiscard]] inline float InputLine<float>::GetValue() {
	StripString(m_value);
	if (m_value.size() == 0) {
		return 0.0f;
	}
	for (char& c : m_value) {
		if (c == ',') {
			c = '.';
			break;
		}
	}
	return std::stof(m_value);
}
/**
 * returns the value for a double input line.
 */
template<>
[[nodiscard]] inline double InputLine<double>::GetValue() {
	StripString(m_value);
	if (m_value.size() == 0) {
		return 0.0;
	}
	for (char& c : m_value) {
		if (c == ',') {
			c = '.';
			break;
		}
	}
	return std::stod(m_value);
}
/**
 * returns the value for a string input line.
 */
template<>
[[nodiscard]] inline std::string InputLine<std::string>::GetValue() {
	StripString(m_value);
	return m_value;
}

/**
 * sets the value for a string input line.
 * need of this overload because std::to_string doesnt provides an overloas for an std::string.
 */
template<>
inline void InputLine<std::string>::SetValue(std::string value) {
	m_value = value;
}
