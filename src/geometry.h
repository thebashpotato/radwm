#ifndef RADWM_GEOMETRY_H
#define RADWM_GEOMETRY_H

/**
 * A simple geometry library meant for use specifically with Xlib. Althought the library
 * has no dependency on Xlib and could be used standalone. Since Xlib offers no basic geometric types
 * or functions many window managers and X clients that use it such as dwm have giant nasty functions that compute
 * x,y,w,h at the callsite all over their codebase, which is highly unreadable, repetitive and error prone.
 * This also leads to functions requiring way more parameters than they should take. It's much easier to pass
 * a single Rect through. There are also many qaulity of life functions which operate on a Rect that are very useful
 * when writing Layout algorithms such as tiling, monocle, fib, column so on and so forth.
 * */

#include <stdbool.h>

#include "types.h"

struct Point {
	i32 x;
	i32 y;
};

struct SignedDimension {
	i32 sw;
	i32 sh;
};

struct Dimension {
	u32 w;
	u32 h;
};

enum RectCorner {
	RectCornerTopLeft = 0,
	RectCornerTopRight,
	RectCornerBottomRight,
	RectCornerBottomLeft,
	RectCornerLast
};

struct RectCorners {
	struct Point corners[RectCornerLast];
};

struct Rect {
	struct Point pt;
	struct Dimension dim;
};

////////////////////////////////
///         Point
////////////////////////////////

/**
 * @brief Builds a default point (x: 0, y: 0)
 * */
struct Point point_default(void);

/**
 * @brief Builds a user defined Point
 * */
struct Point point_new(const i32 x, const i32 y);

/**
 * @brief Adds two Points together and returns the result as a new Point
 * */
struct Point point_add(const struct Point lhs, const struct Point rhs);

/**
 * @brief Subtracts two Points and returns the result as a new Point
 * */
struct Point point_sub(const struct Point lhs, const struct Point rhs);

/**
 * @brief Compares two points for equality
 * */
bool point_eq(const struct Point lhs, const struct Point rhs);

/**
 * @brief Compares two points for inequality
 * */
bool point_ne(const struct Point lhs, const struct Point rhs);

////////////////////////////////
///         Dimension
////////////////////////////////

/**
 * @brief Builds a default Dimension of (w: 1, h: 1)
 * */
struct Dimension dimension_default(void);

/**
 * @brief Builds w: 1, h: 1)
 * @details if width is 0 it is clamped to 1, if height is 0 it is clamped to 1
 * */
struct Dimension dimension_new(const u32 w, const u32 h);

/**
 * @brief Converts a regular unsigned Dimension to a SignedDimension
 * */
struct SignedDimension dimension_to_signed(const struct Dimension dim);

/**
 * @brief Adds two Dimensions and returns the result as a new Dimension
 * */
struct Dimension dimension_add(const struct Dimension lhs,
			       const struct Dimension rhs);

/**
 * @brief Subtracts two Dimensions and returns the result as a new Dimension
 * */
struct Dimension dimension_sub(const struct Dimension lhs,
			       const struct Dimension rhs);

/**
 * @brief Check equality
 * @details lhs == rhs
 * */
bool dimension_eq(const struct Dimension lhs, const struct Dimension rhs);

/**
 * @brief Check inequality
 * @details lhs != rhs
 * */
bool dimension_ne(const struct Dimension lhs, const struct Dimension rhs);

/**
 * @brief Check less than
 * @details lhs < rhs 
 * */
bool dimension_lt(const struct Dimension lhs, const struct Dimension rhs);

/**
 * @brief Check less than or equal
 * @details lhs <= rhs 
 * */
bool dimension_lteq(const struct Dimension lhs, const struct Dimension rhs);

/**
 * @brief Check greater than
 * @details lhs > rhs 
 * */
bool dimension_gt(const struct Dimension lhs, const struct Dimension rhs);

/**
 * @brief Check greater than or equal
 * @details lhs >= rhs 
 * */
bool dimension_gteq(const struct Dimension lhs, const struct Dimension rhs);

////////////////////////////////
///         Rect
////////////////////////////////

/**
 * @brief Builds a default rect with (x:0,y:0,w:1,h:1)
 * */
struct Rect rect_default(void);

/**
 * @brief Builds a user defined Dimension
 * @details If w:0 or h:0 values will be clamped to 1
 * */
struct Rect rect_new(const struct Point point, struct Dimension dim);

/**
 * @brief Builds a rect with (x: 0, y: 0) and a user defined Dimension
 * @details If w:0 or h:0 values will be clamped to 1
 * */
struct Rect rect_from_dimension(struct Dimension dim);

/**
 * @brief Adds two Rect's and returns the result as a new Rect
 * */
struct Rect rect_add(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Subtracts two Rect's and returns the result as a new Rect
 * */
struct Rect rect_sub(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Check equality of rect dimensions only
 * @details lhs.dim == rhs.dim
 * */
bool rect_eq(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Check inequalityof rect dimensions only
 * @details lhs.dim != rhs.dim
 * */
bool rect_ne(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Check less than of rect dimensions only
 * @details lhs.dim < rhs.dim
 * */
bool rect_lt(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Check less than or equal of rect dimensions only
 * @details lhs.dim <= rhs.dim
 * */
bool rect_lteq(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Check greater than of rect dimensions only
 * @details lhs.rim > rhs.dim
 * */
bool rect_gt(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Check greater than or equal of rect dimensions only
 * @details lhs.dim >= rhs.dim
 * */
bool rect_gteq(const struct Rect lhs, const struct Rect rhs);

/**
 * @brief Get's the four corners of given rect in Point form returned in clockwise order
 * from the top left corner
 * */
struct RectCorners rect_corners(const struct Rect rect);

/**
 * @brief Get the midpoint of a given Rect
 * @details Odd side lengths will lead to a truncated point towards to top left corner
 * in order to maintain integer coordinats
 * */
struct Point rect_midpoint(const struct Rect rect);

/**
 * @brief Creates new Rect with a shrunken width and height by the given pixel border,
 * maintaining the current (x,y) coordinates
 *
 * @details The resulting Rect will always have a minimum width and height of 1
 * */
struct Rect rect_shrinkin(const struct Rect rect, const u32 border);

/**
 * @brief Creates a new Rect with a width equal to the factor * the current width
 * */
struct Rect rect_scale_width(const struct Rect rect, const double factor);

/**
 * @brief Creates a new Rect with a height equal to the factory * current height
 * */
struct Rect rect_scale_height(const struct Rect rect, const double factor);

/**
 * @brief Mutate the width and height of a Rect inplace by specified deltas
 *
 * @param rect The Rect to be mutated
 * @param dw width delta
 * @param dh heigh delta
 * */
void rect_resize(struct Rect *rect, const i32 dw, const i32 dh);

/**
 * @brief Mutate the position of a Rect by specified deltas
 * @details Minimum (x, y) coordinates are clamped at x=0, y=0.
 *
 * @param rect The Rect to be mutated
 * @param dx x delta
 * @param dy y delta
 * */
void rect_reposition(struct Rect *rect, const i32 dx, const i32 dy);

/**
 * @brief Check whether rect contains `other` as a sub-rect
 * */
bool rect_contains_other(const struct Rect rect, const struct Rect other);

/**
 * @brief Check if `rect` contains `point`
 * */
bool rect_contains_point(const struct Rect rect, const struct Point point);

#endif
