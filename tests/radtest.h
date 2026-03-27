#ifndef RADTEST_H
#define RADTEST_H

#include <stdio.h>

static int radtest_passes = 0;
static int radtest_fails = 0;

#define ASSERT(expr)                                                    \
	do {                                                            \
		if (!(expr)) {                                          \
			printf("  FAIL: %s (%s:%d)\n", #expr, __FILE__, \
			       __LINE__);                               \
			radtest_fails++;                                \
		} else {                                                \
			radtest_passes++;                               \
		}                                                       \
	} while (0)

#define RUN_TEST(fn)                   \
	do {                           \
		printf("  %s\n", #fn); \
		fn();                  \
	} while (0)

#define TEST_SUMMARY() \
	printf("\n  %d passed, %d failed\n", radtest_passes, radtest_fails)

#define TEST_EXIT() return (radtest_fails > 0) ? 1 : 0

#endif
