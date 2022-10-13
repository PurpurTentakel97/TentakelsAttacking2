//
// Purpur Tentakel
// 05.10.2022
//

#include "ColorPicker.h"
#include "AppContext.h"
#include "ColorPickerCell.h"
#include "HGeneral.h"
#include <cmath>


void ColorPicker::Initialise(Vector2 resolution) {
	AppContext& appContext = AppContext::GetInstance();

	auto colors = appContext.playerCollection.GetAllColors();

	double countX_D = std::ceil(std::sqrt(colors.size()));
	m_countX = static_cast<size_t>(countX_D);
	m_countY = static_cast<size_t>(std::ceil(colors.size() / countX_D));

	for (int row = 0; row < m_countY; ++row) {
		for (int column = 0; column < m_countX; ++column) {

			size_t index = GetIndexFromRowAndColumn(row, column, m_countX);
			// start					offset
			float posX = (1.0f / (2 * m_countX)) + (1.0f / m_countX * column);
			float posY = (1.0f / (2 * m_countY)) + (1.0f / m_countY * row);
			float sizeX = (0.8f / m_countX);
			float sizeY = (0.7f / m_countY);

			Color color = colors.size() > index ? colors.at(index) : BLANK;

			m_cells.push_back(std::make_unique<ColorPickerCell>(
				static_cast<unsigned int>(index + 1),
				GetElementPosition(m_pos, m_size, posX, posY),
				GetElementSize(m_size, sizeX, sizeY),
				Alignment::MID_MID,
				resolution,
				color,
				this
				));
		}
	}
}
void ColorPicker::SetUsedColors(AppContext const& appContext) {
	auto colors = appContext.playerCollection.GetColors();

	for (auto& c : m_cells) {
		bool sameColor = false;
		for (auto& [ID, color] : colors) {
			if (c->GetColor() == color) {
				sameColor = true;
			}
		}

		c->SetEnabled(!sameColor);
	}
}
void ColorPicker::SetColorFromFocus() {
	if (!m_isNestedFocus) {
		return;
	}

	for (auto& c : m_cells) {
		if (c->IsFocused()) {
			m_currentColorCell = c.get();
			return;
		}
	}
}
void ColorPicker::CheckforValidColor(AppContext const& appContext) {
	if (!m_currentColorCell) {
		return;
	}
	if (m_currentColorCell->IsEnabled()) {
		return;
	}

	Color color = appContext.playerCollection.GetPossibleColor();
	SetColor(color);

}

ColorPicker::ColorPicker(unsigned int ID, Vector2 pos, Vector2 size,
	Alignment alignment, Vector2 resolution)
	: Focusable(ID), UIElement(pos, size, alignment) {

	m_colider = GetAlignedCollider(m_pos, m_size, alignment, resolution);
	m_backGround = AppContext::GetInstance().assetManager.GetTexture(AssetType::GREY);
	Initialise(resolution);
}
ColorPicker::~ColorPicker() {
	if (m_isNestedFocus) {
		auto event = DeleteFocusLayerEvent();
		AppContext::GetInstance().eventManager.InvokeEvent(event);
	}
}

Color ColorPicker::GetColor() const {
	if (m_currentColorCell) {
		return m_currentColorCell->GetColor();
	}
	return BLANK;
}
bool ColorPicker::HasColorChanced() const {
	return m_currentColorCell->GetColor() !=
		m_previousColorCell->GetColor();
}
bool ColorPicker::SetInitialColor(Color color) {
	bool set = SetColor(color);
	m_previousColorCell = m_currentColorCell;
	return set;
}
bool ColorPicker::SetColor(Color color) {
	for (auto& c : m_cells) {
		if (c->GetColor() == color) {
			if (!c->IsEnabled()) {
				m_currentColorCell = nullptr;
				return false;
			}

			m_currentColorCell = c.get();
			if (m_isNestedFocus) {
				auto event = SelectFocusElementEvent(c.get());
				AppContext::GetInstance().eventManager.InvokeEvent(event);
			}
			return true;
		}
	}

	return false;
}

void ColorPicker::SetOnEnter(std::function<void()> onEnter) {
	m_onEnter = onEnter;
}

void ColorPicker::SetCellFocuses(AppContext const& appContext) {
	if (m_isNestedFocus) {
		return;
	}

	{
		auto event = NewFocusLayerEvent();
		appContext.eventManager.InvokeEvent(event);
	}

	for (auto& c : m_cells) {
		auto event = NewFocusElementEvent(c.get());
		appContext.eventManager.InvokeEvent(event);
	}

	if (m_currentColorCell) {
		auto event = SelectFocusElementEvent(m_currentColorCell);
		appContext.eventManager.InvokeEvent(event);
	}
	else {
		auto event = SelectFocusElementEvent(m_cells.at(0).get());
		appContext.eventManager.InvokeEvent(event);
	}

	m_isNestedFocus = true;
}
void ColorPicker::SetEnabled(bool enabled, Color color) {
	for (auto& c : m_cells) {
		if (c->GetColor() == color) {
			c->SetEnabled(enabled);
			return;
		}
	}
}
bool ColorPicker::IsEnabled() const {
	return true;
}
Rectangle ColorPicker::GetCollider() const {
	return m_colider;
}

void ColorPicker::CheckAndUpdate(Vector2 const& mousePosition,
	AppContext const& appContext) {
	if (IsKeyPressed(KEY_ESCAPE)) {
		if (m_isNestedFocus) {
			auto event = DeleteFocusLayerEvent();
			AppContext::GetInstance().eventManager.InvokeEvent(event);
			m_isNestedFocus = false;
		}
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (CheckCollisionPointRec(mousePosition, m_colider)) {
			auto event = SelectFocusElementEvent(this);
			appContext.eventManager.InvokeEvent(event);
		}
	}

	if (IsKeyPressed(KEY_ENTER) and IsFocused()) {
		if (!m_isNestedFocus) {
			SetCellFocuses(appContext);
		}
		else {
			m_onEnter();
		}
	}

	for (auto& c : m_cells) {
		c->CheckAndUpdate(mousePosition, appContext);
	}

	SetUsedColors(appContext);
	SetColorFromFocus();
	CheckforValidColor(appContext);
}
void ColorPicker::Render(AppContext const& appContext) {
	// update here to make shure all CheckAndUpdate() is done
	m_previousColorCell = m_currentColorCell;

	DrawTexturePro(
		*m_backGround,
		Rectangle(
			0.0f,
			0.0f,
			static_cast<float>(m_backGround->width),
			static_cast<float>(m_backGround->height)
		),
		m_colider,
		Vector2(0.0f, 0.0f),
		0.0f,
		WHITE
	);

	DrawRectangleLinesEx(
		m_colider,
		3.0f,
		PURPLE
	);

	for (auto& c : m_cells) {
		c->Render(appContext);
	}

	if (m_currentColorCell) {
		DrawRectangleLinesEx(
			m_currentColorCell->GetCollider(),
			3.0f,
			WHITE
		);
	}
}
void ColorPicker::Resize(Vector2 resolution, AppContext const& appContext) {
	m_colider = {
		resolution.x * m_pos.x,
		resolution.y * m_pos.y,
		resolution.x * m_size.x,
		resolution.y * m_size.y
	};

	for (auto& c : m_cells) {
		c->Resize(resolution, appContext);
	}
}