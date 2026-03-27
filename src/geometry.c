#include <math.h>
#include <stdint.h>

#include "types.h"
#include "util.h"
#include "geometry.h"

////////////////////////////////
///         Point
////////////////////////////////

struct Point point_default(void)
{
	return (struct Point){ 0 };
}

struct Point point_new(const i32 x, const i32 y)
{
	return (struct Point){ .x = x, .y = y };
}

struct Point point_add(const struct Point lhs, const struct Point rhs)
{
	return (struct Point){ .x = (lhs.x + rhs.x), .y = (lhs.y + rhs.y) };
}

struct Point point_sub(const struct Point lhs, const struct Point rhs)
{
	return (struct Point){ .x = (lhs.x - rhs.x), .y = (lhs.y - rhs.y) };
}

bool point_eq(const struct Point lhs, const struct Point rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool point_ne(const struct Point lhs, const struct Point rhs)
{
	return !point_eq(lhs, rhs);
}

////////////////////////////////
///         Dimension
////////////////////////////////

struct Dimension dimension_default(void)
{
	return (struct Dimension){ .w = 1, .h = 1 };
}

struct Dimension dimension_new(const u32 w, const u32 h)
{
	return (struct Dimension){ .w = (w == 0 ? 1 : w),
				   .h = (h == 0 ? 1 : h) };
}

struct SignedDimension dimension_to_signed(const struct Dimension dim)
{
	return (struct SignedDimension){ .sw = (i32)dim.w, .sh = (i32)dim.h };
}

struct Dimension dimension_add(const struct Dimension lhs,
			       const struct Dimension rhs)
{
	return (struct Dimension){ .w = (lhs.w + rhs.w), .h = (lhs.h + rhs.h) };
}

struct Dimension dimension_sub(const struct Dimension lhs,
			       const struct Dimension rhs)
{
	return (struct Dimension){ .w = (lhs.w - rhs.w), .h = (lhs.h - rhs.h) };
}

bool dimension_eq(const struct Dimension lhs, const struct Dimension rhs)
{
	return lhs.w == rhs.w && lhs.h == rhs.h;
}

bool dimension_ne(const struct Dimension lhs, const struct Dimension rhs)
{
	return !dimension_eq(lhs, rhs);
}

bool dimension_lt(const struct Dimension lhs, const struct Dimension rhs)
{
	return lhs.w < rhs.w && lhs.h < rhs.h;
}

bool dimension_lteq(const struct Dimension lhs, const struct Dimension rhs)
{
	return lhs.w <= rhs.w && lhs.h <= rhs.h;
}

bool dimension_gt(const struct Dimension lhs, const struct Dimension rhs)
{
	return lhs.w > rhs.w && lhs.h > rhs.h;
}

bool dimension_gteq(const struct Dimension lhs, const struct Dimension rhs)
{
	return lhs.w >= rhs.w && lhs.h >= rhs.h;
}

////////////////////////////////
///         Rect
////////////////////////////////

struct Rect rect_default(void)
{
	return (struct Rect){ .pt = point_default(),
			      .dim = dimension_default() };
}

struct Rect rect_new(const struct Point point, struct Dimension dim)
{
	if (dim.w == 0) {
		dim.w = 1;
	}
	if (dim.h == 0) {
		dim.h = 1;
	}
	return (struct Rect){ .pt = point, .dim = dim };
}

struct Rect rect_from_dimension(struct Dimension dim)
{
	if (dim.w == 0) {
		dim.w = 1;
	}
	if (dim.h == 0) {
		dim.h = 1;
	}
	return (struct Rect){ .pt = point_default(), .dim = dim };
}

struct Rect rect_add(const struct Rect lhs, const struct Rect rhs)
{
	return (struct Rect){ .pt = point_add(lhs.pt, rhs.pt),
			      .dim = dimension_add(lhs.dim, rhs.dim) };
}

struct Rect rect_sub(const struct Rect lhs, const struct Rect rhs)
{
	return (struct Rect){ .pt = point_sub(lhs.pt, rhs.pt),
			      .dim = dimension_sub(lhs.dim, rhs.dim) };
}

bool rect_eq(const struct Rect lhs, const struct Rect rhs)
{
	return point_eq(lhs.pt, rhs.pt) && dimension_eq(lhs.dim, rhs.dim);
}

bool rect_ne(const struct Rect lhs, const struct Rect rhs)
{
	return !rect_eq(lhs, rhs);
}

bool rect_lt(const struct Rect lhs, const struct Rect rhs)
{
	return dimension_lt(lhs.dim, rhs.dim);
}

bool rect_lteq(const struct Rect lhs, const struct Rect rhs)
{
	return dimension_lteq(lhs.dim, rhs.dim);
}

bool rect_gt(const struct Rect lhs, const struct Rect rhs)
{
	return dimension_gt(lhs.dim, rhs.dim);
}

bool rect_gteq(const struct Rect lhs, const struct Rect rhs)
{
	return dimension_gteq(lhs.dim, rhs.dim);
}

struct RectCorners rect_corners(const struct Rect rect)
{
	struct RectCorners rc = { 0 };
	struct SignedDimension sd = dimension_to_signed(rect.dim);

	rc.corners[RectCornerTopLeft] = rect.pt;
	rc.corners[RectCornerTopRight] =
		point_new(rect.pt.x + sd.sw, rect.pt.y);
	rc.corners[RectCornerBottomRight] =
		point_new(rect.pt.x + sd.sw, rect.pt.y + sd.sh);
	rc.corners[RectCornerBottomLeft] =
		point_new(rect.pt.x, rect.pt.y + sd.sh);

	return rc;
}

struct Point rect_midpoint(const struct Rect rect)
{
	struct SignedDimension sd = dimension_to_signed(rect.dim);
	return point_new(rect.pt.x + (sd.sw / 2), rect.pt.y + (sd.sh / 2));
}

struct Rect rect_shrinkin(const struct Rect rect, const u32 border)
{
	const u32 shrink_amt = border * 2;
	u32 shrunken_width;
	u32 shrunken_height;

	if (rect.dim.w <= shrink_amt) {
		shrunken_width = 1;
	} else {
		shrunken_width = rect.dim.w - shrink_amt;
	}

	if (rect.dim.h <= shrink_amt) {
		shrunken_height = 1;
	} else {
		shrunken_height = rect.dim.h - shrink_amt;
	}

	return rect_new(rect.pt,
			dimension_new(shrunken_width, shrunken_height));
}

struct Rect rect_scale_width(const struct Rect rect, const double factor)
{
	if (factor == 1.0) {
		return rect;
	}

	const double scaled = floor(rect.dim.w * factor);

	u32 new_width = 0;

	if (scaled < 1.0) {
		new_width = 1;
	} else if (scaled > UINT32_MAX) {
		// Unlikely to ever happen, but hey..
		new_width = UINT32_MAX;
	} else {
		new_width = (u32)scaled;
	}

	return rect_new(rect.pt, dimension_new(new_width, rect.dim.h));
}

struct Rect rect_scale_height(const struct Rect rect, const double factor)
{
	if (factor == 1.0) {
		return rect;
	}

	const double scaled = floor(rect.dim.h * factor);
	u32 new_height = 0;

	if (scaled < 1.0) {
		new_height = 1;
	} else if (scaled > UINT32_MAX) {
		// Unlikely to ever happen, but hey..
		new_height = UINT32_MAX;
	} else {
		new_height = (u32)scaled;
	}

	return rect_new(rect.pt, dimension_new(rect.dim.w, new_height));
}

void rect_resize(struct Rect *rect, const i32 dw, const i32 dh)
{
	if (rect == NULL) {
		return;
	}

	const struct SignedDimension sd = dimension_to_signed(rect->dim);

	if (dw != 0) {
		rect->dim.w = (u32)MAX(1, sd.sw + dw);
	}
	if (dh != 0) {
		rect->dim.h = (u32)MAX(1, sd.sh + dh);
	}
}

void rect_reposition(struct Rect *rect, const i32 dx, const i32 dy)
{
	if (rect == NULL) {
		return;
	}

	if (dx != 0) {
		rect->pt.x = MAX(0, dx + rect->pt.x);
	}

	if (dy != 0) {
		rect->pt.y = MAX(0, dy + rect->pt.y);
	}
}

bool rect_contains_other(const struct Rect rect, const struct Rect other)
{
	if (other.pt.x < rect.pt.x || other.pt.y < rect.pt.y) {
		return false;
	}

	const struct SignedDimension rect_sd = dimension_to_signed(rect.dim);
	const struct SignedDimension other_sd = dimension_to_signed(other.dim);

	if ((other.pt.x + other_sd.sw) > (rect.pt.x + rect_sd.sw)) {
		return false;
	}

	if ((other.pt.y + other_sd.sh) > (rect.pt.y + rect_sd.sh)) {
		return false;
	}

	return true;
}

bool rect_contains_point(const struct Rect rect, const struct Point point)
{
	const struct SignedDimension sd = dimension_to_signed(rect.dim);

	return (rect.pt.x <= point.x && point.x <= (rect.pt.x + sd.sw)) &&
	       (rect.pt.y <= point.y && point.y <= (rect.pt.y + sd.sh));
}
