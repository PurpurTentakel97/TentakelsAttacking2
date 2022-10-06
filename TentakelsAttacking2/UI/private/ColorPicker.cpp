//
// Purpur Tentakel
// 05.10.2022
//

#include "ColorPicker.h"
#include "UIEvents.h"
#include "AppContext.h"
#include "ColorPickerCell.h"
#include "GenerelHelper.h"


void ColorPicker::Initialise(Vector2 resolution) {
	for (int row = 0; row < m_countY; ++row) {
		for (int column = 0; column < m_countX; ++column) {

			size_t index = GetIndexFromRowAndColumn(row, column, m_countX);
						 // start					offset
			float posX = (1.0f / (2 * m_countX)) +  (1.0f / m_countX * column);
			float posY = (1.0f / (2 * m_countY)) + (1.0f / m_countY * row);
			float sizeX = (0.8f / m_countX);
			float sizeY = (0.7f / m_countY);

			m_cells.at(index) = std::make_unique<ColorPickerCell>(
				index + 1,
				GetElementPosition(m_pos, m_size, posX, posY),
				GetElementSize(m_size, sizeX, sizeY),
				Alignment::MID_MID,
				resolution,
				m_colors.at(index),
				this
			);;
		}
	}
}

ColorPicker::ColorPicker(unsigned int ID, Vector2 pos, Vector2 size,
	Alignment alignment, Vector2 resolution) 
	: Focusable(ID), UIElement(pos, size, alignment) {

	m_colider = GetAlignedCollider(m_pos, m_size, alignment, resolution);
	m_backGround = AppContext::GetInstance().assetManager.GetTexture(AssetType::GREY);
	Initialise(resolution);
}
ColorPicker::~ColorPicker() {
	if (m_focusedCells) {
		auto event = DeleteFocusLayerEvent();
		AppContext::GetInstance().eventManager.InvokeEvent(event);
	}
}

Color ColorPicker::GetColor() {
	if (m_currentColorCell) {
		return m_currentColorCell->GetColor();
	}
	return BLANK;
}
bool ColorPicker::SetColor(Color color) {
	for (auto& c : m_cells) {
		if (c->GetColor() == color) {
			if (!c->IsEnabled()) {
				return false;
			}

			m_currentColorCell = c.get();
			return true;
		}
	}

	return false;
}

void ColorPicker::SetCellFocuses(AppContext const& appContext) {
	if (m_focusedCells) {
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

	m_focusedCells = true;
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
		if (m_focusedCells) {
			auto event = DeleteFocusLayerEvent();
			AppContext::GetInstance().eventManager.InvokeEvent(event);
			m_focusedCells = false;
		}
	}

	if (IsKeyPressed(KEY_ENTER) and IsFocused()) {
		if (!m_focusedCells) {
			SetCellFocuses(appContext);
		}
	}

	for (auto& c : m_cells) {
		c->CheckAndUpdate(mousePosition, appContext);
	}
}
void ColorPicker::Render(AppContext const& appContext) {
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