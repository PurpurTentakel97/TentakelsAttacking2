//
// Purpur Tentakel
// 30.09.22
//

#include "EmptyCell.h"

Vector2 EmptyCell::GetNeededSize() {
	return { 0.05f,0.05f };
}

EmptyCell::EmptyCell(unsigned int ID, Vector2 pos, Vector2 size,
	Alignment alignment, Vector2 resolution)
	:AbstractTableCell(ID, pos, size, alignment, resolution) { }

void EmptyCell::CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) {

	UIElement::CheckAndUpdate(mousePosition, appContext);
}
void EmptyCell::Render([[maybe_unused]] AppContext const& appContext) {
	DrawRectangleLinesEx(
		m_collider,
		3.0f,
		WHITE
	);
};
