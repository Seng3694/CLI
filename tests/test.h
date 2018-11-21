#ifndef CLI_TEST_H
#define CLI_TEST_H

#define TEST(file, desc) void file##_##desc##_test(void)

#define TESTMAIN()		   \
static int test_count = 0; \
int main(void)

#define RUNTEST(file, desc) ++test_count; file##_##desc##_test()
#define streq(lhs, rhs) (strcmp(lhs, rhs) == 0)

#endif 


