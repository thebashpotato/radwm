#include "utest.h"
#include "../src/geometry.h"

static void test_point_default(void)
{
	struct Point p = point_default();
	ASSERT(p.x == 0);
	ASSERT(p.y == 0);
}

static void test_point_new(void)
{
	struct Point p = point_new(10, -20);
	ASSERT(p.x == 10);
	ASSERT(p.y == -20);
}

static void test_point_add(void)
{
	struct Point a = point_new(1, 2);
	struct Point b = point_new(3, 4);
	struct Point c = point_add(a, b);
	ASSERT(c.x == 4 && c.y == 6);
}

static void test_point_add_negative(void)
{
	struct Point a = point_new(-5, 10);
	struct Point b = point_new(3, -7);
	struct Point c = point_add(a, b);
	ASSERT(c.x == -2);
	ASSERT(c.y == 3);
}

static void test_point_sub(void)
{
	struct Point a = point_new(10, 20);
	struct Point b = point_new(3, 5);
	struct Point c = point_sub(a, b);
	ASSERT(c.x == 7);
	ASSERT(c.y == 15);
}

static void test_point_sub_goes_negative(void)
{
	struct Point a = point_new(1, 1);
	struct Point b = point_new(5, 10);
	struct Point c = point_sub(a, b);
	ASSERT(c.x == -4);
	ASSERT(c.y == -9);
}

static void test_point_eq(void)
{
	struct Point a = point_new(5, 10);
	struct Point b = point_new(5, 10);
	ASSERT(point_eq(a, b));
}

static void test_point_eq_false_when_different(void)
{
	struct Point a = point_new(5, 10);
	struct Point b = point_new(5, 11);
	ASSERT(!point_eq(a, b));
}

static void test_point_ne(void)
{
	struct Point a = point_new(1, 2);
	struct Point b = point_new(3, 4);
	ASSERT(point_ne(a, b));
}

static void test_point_ne_false_when_equal(void)
{
	struct Point a = point_new(7, 7);
	struct Point b = point_new(7, 7);
	ASSERT(!point_ne(a, b));
}

static void test_dimension_default(void)
{
	struct Dimension d = dimension_default();
	ASSERT(d.w == 1);
	ASSERT(d.h == 1);
}

static void test_dimension_new(void)
{
	struct Dimension d = dimension_new(800, 600);
	ASSERT(d.w == 800);
	ASSERT(d.h == 600);
}

static void test_dimension_new_clamps_zero_w(void)
{
	struct Dimension d = dimension_new(0, 500);
	ASSERT(d.w == 1);
	ASSERT(d.h == 500);
}

static void test_dimension_new_clamps_zero_h(void)
{
	struct Dimension d = dimension_new(500, 0);
	ASSERT(d.w == 500);
	ASSERT(d.h == 1);
}

static void test_dimension_new_clamps_both_zero(void)
{
	struct Dimension d = dimension_new(0, 0);
	ASSERT(d.w == 1);
	ASSERT(d.h == 1);
}

static void test_dimension_to_signed(void)
{
	struct Dimension d = dimension_new(100, 200);
	struct SignedDimension sd = dimension_to_signed(d);
	ASSERT(sd.sw == 100);
	ASSERT(sd.sh == 200);
}

static void test_dimension_add(void)
{
	struct Dimension a = dimension_new(10, 20);
	struct Dimension b = dimension_new(5, 15);
	struct Dimension c = dimension_add(a, b);
	ASSERT(c.w == 15);
	ASSERT(c.h == 35);
}

static void test_dimension_sub(void)
{
	struct Dimension a = dimension_new(100, 200);
	struct Dimension b = dimension_new(30, 50);
	struct Dimension c = dimension_sub(a, b);
	ASSERT(c.w == 70);
	ASSERT(c.h == 150);
}

static void test_dimension_eq(void)
{
	struct Dimension a = dimension_new(10, 20);
	struct Dimension b = dimension_new(10, 20);
	ASSERT(dimension_eq(a, b));
}

static void test_dimension_eq_false_when_different(void)
{
	struct Dimension a = dimension_new(10, 20);
	struct Dimension b = dimension_new(10, 21);
	ASSERT(!dimension_eq(a, b));
}

static void test_dimension_ne(void)
{
	struct Dimension a = dimension_new(10, 20);
	struct Dimension b = dimension_new(30, 40);
	ASSERT(dimension_ne(a, b));
}

static void test_dimension_ne_false_when_equal(void)
{
	struct Dimension a = dimension_new(10, 20);
	struct Dimension b = dimension_new(10, 20);
	ASSERT(!dimension_ne(a, b));
}

static void test_dimension_lt(void)
{
	struct Dimension a = dimension_new(5, 10);
	struct Dimension b = dimension_new(20, 30);
	ASSERT(dimension_lt(a, b));
}

static void test_dimension_lt_false_when_equal(void)
{
	struct Dimension a = dimension_new(10, 10);
	struct Dimension b = dimension_new(10, 10);
	ASSERT(!dimension_lt(a, b));
}

static void test_dimension_lt_false_when_greater(void)
{
	struct Dimension a = dimension_new(20, 30);
	struct Dimension b = dimension_new(5, 10);
	ASSERT(!dimension_lt(a, b));
}

static void test_dimension_lteq(void)
{
	struct Dimension a = dimension_new(5, 10);
	struct Dimension b = dimension_new(20, 30);
	ASSERT(dimension_lteq(a, b));
}

static void test_dimension_lteq_when_equal(void)
{
	struct Dimension a = dimension_new(10, 10);
	struct Dimension b = dimension_new(10, 10);
	ASSERT(dimension_lteq(a, b));
}

static void test_dimension_lteq_false_when_greater(void)
{
	struct Dimension a = dimension_new(20, 30);
	struct Dimension b = dimension_new(5, 10);
	ASSERT(!dimension_lteq(a, b));
}

static void test_dimension_gt(void)
{
	struct Dimension a = dimension_new(20, 30);
	struct Dimension b = dimension_new(5, 10);
	ASSERT(dimension_gt(a, b));
}

static void test_dimension_gt_false_when_less(void)
{
	struct Dimension a = dimension_new(5, 10);
	struct Dimension b = dimension_new(20, 30);
	ASSERT(!dimension_gt(a, b));
}

static void test_dimension_gteq(void)
{
	struct Dimension a = dimension_new(20, 30);
	struct Dimension b = dimension_new(5, 10);
	ASSERT(dimension_gteq(a, b));
}

static void test_dimension_gteq_when_equal(void)
{
	struct Dimension a = dimension_new(10, 10);
	struct Dimension b = dimension_new(10, 10);
	ASSERT(dimension_gteq(a, b));
}

static void test_dimension_gteq_false_when_less(void)
{
	struct Dimension a = dimension_new(5, 10);
	struct Dimension b = dimension_new(20, 30);
	ASSERT(!dimension_gteq(a, b));
}

static void test_rect_default(void)
{
	struct Rect r = rect_default();
	ASSERT(r.pt.x == 0);
	ASSERT(r.pt.y == 0);
	ASSERT(r.dim.w == 1);
	ASSERT(r.dim.h == 1);
}

static void test_rect_new(void)
{
	struct Rect r = rect_new(point_new(10, 20), dimension_new(100, 200));
	ASSERT(r.pt.x == 10);
	ASSERT(r.pt.y == 20);
	ASSERT(r.dim.w == 100);
	ASSERT(r.dim.h == 200);
}

static void test_rect_from_dimension(void)
{
	struct Rect r = rect_from_dimension(dimension_new(50, 60));
	ASSERT(r.pt.x == 0);
	ASSERT(r.pt.y == 0);
	ASSERT(r.dim.w == 50);
	ASSERT(r.dim.h == 60);
}

static void test_rect_add(void)
{
	struct Rect a = rect_new(point_new(1, 2), dimension_new(10, 20));
	struct Rect b = rect_new(point_new(3, 4), dimension_new(5, 6));
	struct Rect c = rect_add(a, b);
	ASSERT(c.pt.x == 4);
	ASSERT(c.pt.y == 6);
	ASSERT(c.dim.w == 15);
	ASSERT(c.dim.h == 26);
}

static void test_rect_sub(void)
{
	struct Rect a = rect_new(point_new(10, 20), dimension_new(100, 200));
	struct Rect b = rect_new(point_new(3, 5), dimension_new(30, 50));
	struct Rect c = rect_sub(a, b);
	ASSERT(c.pt.x == 7);
	ASSERT(c.pt.y == 15);
	ASSERT(c.dim.w == 70);
	ASSERT(c.dim.h == 150);
}

static void test_rect_eq(void)
{
	struct Rect a = rect_new(point_new(1, 2), dimension_new(10, 20));
	struct Rect b = rect_new(point_new(1, 2), dimension_new(10, 20));
	ASSERT(rect_eq(a, b));
}

static void test_rect_eq_false_when_different(void)
{
	struct Rect a = rect_new(point_new(1, 2), dimension_new(10, 20));
	struct Rect b = rect_new(point_new(1, 2), dimension_new(10, 21));
	ASSERT(!rect_eq(a, b));
}

static void test_rect_ne(void)
{
	struct Rect a = rect_new(point_new(1, 2), dimension_new(10, 20));
	struct Rect b = rect_new(point_new(3, 4), dimension_new(30, 40));
	ASSERT(rect_ne(a, b));
}

static void test_rect_ne_false_when_equal(void)
{
	struct Rect a = rect_new(point_new(1, 2), dimension_new(10, 20));
	struct Rect b = rect_new(point_new(1, 2), dimension_new(10, 20));
	ASSERT(!rect_ne(a, b));
}

static void test_rect_lt(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(5, 10));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(20, 30));
	ASSERT(rect_lt(a, b));
}

static void test_rect_lt_false_when_equal(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(10, 10));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(10, 10));
	ASSERT(!rect_lt(a, b));
}

static void test_rect_lteq(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(5, 10));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(20, 30));
	ASSERT(rect_lteq(a, b));
}

static void test_rect_lteq_when_equal(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(10, 10));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(10, 10));
	ASSERT(rect_lteq(a, b));
}

static void test_rect_gt(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(20, 30));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(5, 10));
	ASSERT(rect_gt(a, b));
}

static void test_rect_gt_false_when_less(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(5, 10));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(20, 30));
	ASSERT(!rect_gt(a, b));
}

static void test_rect_gteq(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(20, 30));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(5, 10));
	ASSERT(rect_gteq(a, b));
}

static void test_rect_gteq_when_equal(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(10, 10));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(10, 10));
	ASSERT(rect_gteq(a, b));
}

static void test_rect_corners(void)
{
	struct Rect r = rect_new(point_new(10, 20), dimension_new(100, 50));
	struct RectCorners rc = rect_corners(r);
	ASSERT(rc.corners[RectCornerTopLeft].x == 10);
	ASSERT(rc.corners[RectCornerTopLeft].y == 20);
	ASSERT(rc.corners[RectCornerTopRight].x == 110);
	ASSERT(rc.corners[RectCornerTopRight].y == 20);
	ASSERT(rc.corners[RectCornerBottomRight].x == 110);
	ASSERT(rc.corners[RectCornerBottomRight].y == 70);
	ASSERT(rc.corners[RectCornerBottomLeft].x == 10);
	ASSERT(rc.corners[RectCornerBottomLeft].y == 70);
}

static void test_rect_midpoint(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 200));
	struct Point mid = rect_midpoint(r);
	ASSERT(mid.x == 50);
	ASSERT(mid.y == 100);
}

static void test_rect_midpoint_odd(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(101, 201));
	struct Point mid = rect_midpoint(r);
	ASSERT(mid.x == 50);
	ASSERT(mid.y == 100);
}

static void test_rect_midpoint_with_offset(void)
{
	struct Rect r = rect_new(point_new(10, 20), dimension_new(100, 200));
	struct Point mid = rect_midpoint(r);
	ASSERT(mid.x == 60);
	ASSERT(mid.y == 120);
}

static void test_rect_shrinkin(void)
{
	struct Rect r = rect_new(point_new(5, 5), dimension_new(100, 200));
	struct Rect s = rect_shrinkin(r, 10);
	ASSERT(s.pt.x == 5);
	ASSERT(s.pt.y == 5);
	ASSERT(s.dim.w == 80);
	ASSERT(s.dim.h == 180);
}

static void test_rect_shrinkin_clamps_to_one(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(10, 10));
	struct Rect s = rect_shrinkin(r, 100);
	ASSERT(s.dim.w == 1);
	ASSERT(s.dim.h == 1);
}

static void test_rect_scale_width(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 50));
	struct Rect s = rect_scale_width(r, 0.5);
	ASSERT(s.dim.w == 50);
	ASSERT(s.dim.h == 50);
	ASSERT(s.pt.x == 0);
}

static void test_rect_scale_width_identity(void)
{
	struct Rect r = rect_new(point_new(5, 10), dimension_new(100, 50));
	struct Rect s = rect_scale_width(r, 1.0);
	ASSERT(s.dim.w == 100);
	ASSERT(s.dim.h == 50);
}

static void test_rect_scale_width_clamps_to_one(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(1, 50));
	struct Rect s = rect_scale_width(r, 0.01);
	ASSERT(s.dim.w == 1);
}

static void test_rect_scale_height(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 200));
	struct Rect s = rect_scale_height(r, 0.25);
	ASSERT(s.dim.h == 50);
	ASSERT(s.dim.w == 100);
}

static void test_rect_scale_height_identity(void)
{
	struct Rect r = rect_new(point_new(5, 10), dimension_new(100, 200));
	struct Rect s = rect_scale_height(r, 1.0);
	ASSERT(s.dim.h == 200);
}

static void test_rect_scale_height_clamps_to_one(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 1));
	struct Rect s = rect_scale_height(r, 0.01);
	ASSERT(s.dim.h == 1);
}

static void test_rect_resize(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 200));
	rect_resize(&r, 50, -50);
	ASSERT(r.dim.w == 150);
	ASSERT(r.dim.h == 150);
}

static void test_rect_resize_clamps_to_one(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(10, 10));
	rect_resize(&r, -100, -100);
	ASSERT(r.dim.w == 1);
	ASSERT(r.dim.h == 1);
}

static void test_rect_resize_null(void)
{
	rect_resize(NULL, 10, 10);
	ASSERT(1);
}

static void test_rect_reposition(void)
{
	struct Rect r = rect_new(point_new(50, 50), dimension_new(10, 10));
	rect_reposition(&r, 20, -30);
	ASSERT(r.pt.x == 70);
	ASSERT(r.pt.y == 20);
}

static void test_rect_reposition_clamps_to_zero(void)
{
	struct Rect r = rect_new(point_new(5, 5), dimension_new(10, 10));
	rect_reposition(&r, -100, -100);
	ASSERT(r.pt.x == 0);
	ASSERT(r.pt.y == 0);
}

static void test_rect_reposition_null(void)
{
	rect_reposition(NULL, 10, 10);
	ASSERT(1);
}

static void test_rect_contains_other(void)
{
	struct Rect outer = rect_new(point_new(0, 0), dimension_new(100, 100));
	struct Rect inner = rect_new(point_new(10, 10), dimension_new(20, 20));
	ASSERT(rect_contains_other(outer, inner));
}

static void test_rect_contains_other_exact_fit(void)
{
	struct Rect a = rect_new(point_new(0, 0), dimension_new(100, 100));
	struct Rect b = rect_new(point_new(0, 0), dimension_new(100, 100));
	ASSERT(rect_contains_other(a, b));
}

static void test_rect_contains_other_false_when_outside(void)
{
	struct Rect outer = rect_new(point_new(0, 0), dimension_new(50, 50));
	struct Rect inner = rect_new(point_new(60, 60), dimension_new(10, 10));
	ASSERT(!rect_contains_other(outer, inner));
}

static void test_rect_contains_other_false_when_partially_outside(void)
{
	struct Rect outer = rect_new(point_new(0, 0), dimension_new(50, 50));
	struct Rect inner = rect_new(point_new(40, 40), dimension_new(20, 20));
	ASSERT(!rect_contains_other(outer, inner));
}

static void test_rect_contains_point(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 100));
	ASSERT(rect_contains_point(r, point_new(50, 50)));
}

static void test_rect_contains_point_on_edge(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 100));
	ASSERT(rect_contains_point(r, point_new(0, 0)));
	ASSERT(rect_contains_point(r, point_new(100, 100)));
}

static void test_rect_contains_point_false_when_outside(void)
{
	struct Rect r = rect_new(point_new(0, 0), dimension_new(100, 100));
	ASSERT(!rect_contains_point(r, point_new(200, 200)));
}

int main(void)
{
	RUN_TEST(test_point_default);
	RUN_TEST(test_point_new);
	RUN_TEST(test_point_add);
	RUN_TEST(test_point_add_negative);
	RUN_TEST(test_point_sub);
	RUN_TEST(test_point_sub_goes_negative);
	RUN_TEST(test_point_eq);
	RUN_TEST(test_point_eq_false_when_different);
	RUN_TEST(test_point_ne);
	RUN_TEST(test_point_ne_false_when_equal);

	RUN_TEST(test_dimension_default);
	RUN_TEST(test_dimension_new);
	RUN_TEST(test_dimension_new_clamps_zero_w);
	RUN_TEST(test_dimension_new_clamps_zero_h);
	RUN_TEST(test_dimension_new_clamps_both_zero);
	RUN_TEST(test_dimension_to_signed);
	RUN_TEST(test_dimension_add);
	RUN_TEST(test_dimension_sub);
	RUN_TEST(test_dimension_eq);
	RUN_TEST(test_dimension_eq_false_when_different);
	RUN_TEST(test_dimension_ne);
	RUN_TEST(test_dimension_ne_false_when_equal);
	RUN_TEST(test_dimension_lt);
	RUN_TEST(test_dimension_lt_false_when_equal);
	RUN_TEST(test_dimension_lt_false_when_greater);
	RUN_TEST(test_dimension_lteq);
	RUN_TEST(test_dimension_lteq_when_equal);
	RUN_TEST(test_dimension_lteq_false_when_greater);
	RUN_TEST(test_dimension_gt);
	RUN_TEST(test_dimension_gt_false_when_less);
	RUN_TEST(test_dimension_gteq);
	RUN_TEST(test_dimension_gteq_when_equal);
	RUN_TEST(test_dimension_gteq_false_when_less);

	RUN_TEST(test_rect_default);
	RUN_TEST(test_rect_new);
	RUN_TEST(test_rect_from_dimension);
	RUN_TEST(test_rect_add);
	RUN_TEST(test_rect_sub);
	RUN_TEST(test_rect_eq);
	RUN_TEST(test_rect_eq_false_when_different);
	RUN_TEST(test_rect_ne);
	RUN_TEST(test_rect_ne_false_when_equal);
	RUN_TEST(test_rect_lt);
	RUN_TEST(test_rect_lt_false_when_equal);
	RUN_TEST(test_rect_lteq);
	RUN_TEST(test_rect_lteq_when_equal);
	RUN_TEST(test_rect_gt);
	RUN_TEST(test_rect_gt_false_when_less);
	RUN_TEST(test_rect_gteq);
	RUN_TEST(test_rect_gteq_when_equal);
	RUN_TEST(test_rect_corners);
	RUN_TEST(test_rect_midpoint);
	RUN_TEST(test_rect_midpoint_odd);
	RUN_TEST(test_rect_midpoint_with_offset);
	RUN_TEST(test_rect_shrinkin);
	RUN_TEST(test_rect_shrinkin_clamps_to_one);
	RUN_TEST(test_rect_scale_width);
	RUN_TEST(test_rect_scale_width_identity);
	RUN_TEST(test_rect_scale_width_clamps_to_one);
	RUN_TEST(test_rect_scale_height);
	RUN_TEST(test_rect_scale_height_identity);
	RUN_TEST(test_rect_scale_height_clamps_to_one);
	RUN_TEST(test_rect_resize);
	RUN_TEST(test_rect_resize_clamps_to_one);
	RUN_TEST(test_rect_resize_null);
	RUN_TEST(test_rect_reposition);
	RUN_TEST(test_rect_reposition_clamps_to_zero);
	RUN_TEST(test_rect_reposition_null);
	RUN_TEST(test_rect_contains_other);
	RUN_TEST(test_rect_contains_other_exact_fit);
	RUN_TEST(test_rect_contains_other_false_when_outside);
	RUN_TEST(test_rect_contains_other_false_when_partially_outside);
	RUN_TEST(test_rect_contains_point);
	RUN_TEST(test_rect_contains_point_on_edge);
	RUN_TEST(test_rect_contains_point_false_when_outside);

	TEST_SUMMARY();
	TEST_EXIT();
}
