//
// Purpur Tentakel
// 18.09.2022
//

#pragma once
#include <raylib.h>

/**
 * provides the over all alignment.
 */
enum class Alignment {
	TOP_LEFT,
	TOP_MID,
	TOP_RIGHT,
	MID_LEFT,
	MID_MID,
	MID_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_MID,
	BOTTOM_RIGHT,
	DEFAULT,
};
/**
 * provides the text alignment.
 */
enum class TextAlignment {
	TOP,
	MID,
	BOTTOM,
	LEFT,
	RIGHT,
};

/**
 * recalculates the position witch the size and alignment.
 */
Rectangle GetAlignedCollider(Vector2& pos, Vector2 size,
	Alignment allignment, Vector2 resolution);
/**
 * recalulatres the provided position witch the provided collider and allignment.
 */
[[nodiscard]] Vector2 GetAllignedPosition(Alignment allignment, Vector2 position, Vector2 size);
/**
 * recalulatres the provided position witch the provided collider and allignment.
 */
[[nodiscard]] Vector2 GetAllignedPositionReversed(Alignment allignment, Vector2 position, Vector2 size);

/**
 * converts the over all alignment into horisontal text alignment.
 */
[[nodiscard]] TextAlignment GetHorisontalTextAlignment(Alignment alignment);
/**
 * converts the over all alignment into vertical text alignment.
 */
[[nodiscard]] TextAlignment GetVerticalTextAlignment(Alignment alignment);
