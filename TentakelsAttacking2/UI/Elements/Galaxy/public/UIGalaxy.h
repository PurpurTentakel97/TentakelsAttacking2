//
// Purpur Tentakel
// 12.11.2022
//

#pragma once
#include "UIElement.hpp"
#include "EventListener.hpp"
#include "Focusable.h"
#include <functional>

class UIGalaxy : public UIElement, public EventListener, public Focusable {
private:
	bool m_isEnabled = true;
	bool m_isScaling = true;
	float m_scaleFacor = 1.0f;
	Rectangle m_colider;
	Rectangle m_absoluteSize;
	std::function<void(float)> m_onZoom = [](float) {};

	void CheckPosition();

public:
	UIGalaxy(unsigned int ID, Vector2 pos, Vector2 size, Alignment alignment, Vector2 resolution);

	void SetIsScaling(bool isScaling);
	[[nodiscard]] bool IsScaling() const;

	[[nodiscard]] float GetScaleFactor() const;
	void Zoom(bool zoomIn, int factor = 1);

	void SetOnZoom(std::function<void(float)> onZoom);

	void UpdateColider(Vector2 resolution) override;

	void CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) override;
	void Render(AppContext const& appContext) override;
	void Resize(Vector2 resolution, AppContext const& appContext) override;

	void SetEnabled(bool isEnabled);
	[[nodiscard]] bool IsEnabled() const override;
	[[nodiscard]] Rectangle GetCollider() const override;

	void OnEvent(Event const& event) override;
};