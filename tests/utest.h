#ifndef UTEST_H
#define UTEST_H

#include <stdio.h>

static int utest_passes = 0;
static int utest_fails = 0;

#define ASSERT(expr)                                                    \
	do {                                                            \
		if (!(expr)) {                                          \
			printf("  FAIL: %s (%s:%d)\n", #expr, __FILE__, \
			       __LINE__);                               \
			utest_fails++;                                  \
		} else {                                                \
			utest_passes++;                                 \
		}                                                       \
	} while (0)

#define RUN_TEST(fn)                   \
	do {                           \
		printf("  %s\n", #fn); \
		fn();                  \
	} while (0)

#define TEST_SUMMARY() \
	printf("\n  %d passed, %d failed\n", utest_passes, utest_fails)

#define TEST_EXIT() return (utest_fails > 0) ? 1 : 0

#endif
