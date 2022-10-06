//
// Purpur Tentakel
// 02.10.2022
//

#include "ColorCell.h"
#include "AppContext.h"

void ColorCell::CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) {
	Cell::CheckAndUpdate(mousePosition, appContext);
	if (ShouldEdit(mousePosition)) {
		auto event = ShowColorCellPopUpEvent(
			"Change Color",
			this
		);
		appContext.eventManager.InvokeEvent(event);
	}
}

void ColorCell::Render(AppContext const& appContext) {
	float spacing = 5.0f;
	Rectangle toFill = {
		 m_colider.x + spacing,
		 m_colider.y + spacing,
		 m_colider.width - 2 * spacing,
		 m_colider.height - 2 * spacing
	};
	DrawRectanglePro(
		toFill,
		Vector2(0.0f, 0.0f),
		0.0f,
		value
	);

	Cell::Render(appContext);
}

