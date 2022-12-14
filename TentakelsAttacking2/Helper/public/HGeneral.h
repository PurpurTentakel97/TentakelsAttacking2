//
// Purpur Tentakel
// 05.09.2022
//

#pragma once
#include <raylib.h>
#include <cstddef>

// Colors
/**
 * compares if two colors are the same by comparing all single values.
 */
[[nodiscard]] bool operator==(Color lhs, Color rhs);
/**
 * calls operator== and inverts it.
 */
[[nodiscard]] bool operator!=(Color lhs, Color rhs);

// Position
/**
 * returns the relative element position acording to the parent position and size.
 */
Vector2 GetElementPosition(Vector2 const& pos, Vector2 const& size,
	float x, float y);
/**
 * returns the global element position.
 */
Vector2 GetElementPositionRversed(Vector2 const& posP, Vector2 const& sizeP,
	Vector2 const& posC);
/**
 * returns the relative element size acording to the parent size.
 */
Vector2 GetElementSize(Vector2 const& size, float x, float y);
/**
 * returns the global element size.
 */
Vector2 GetElementSizeReversed(Vector2 const& sizeP, Vector2 const& sizeC);

// Index
/**
 * returns the intex of an array by row and column.
 */
size_t GetIndexFromRowAndColumn(size_t row, size_t column, size_t maxColumn);
