#include <unity.h>

// tests:
void test_dummyTest_assertion_true(void){
  TEST_ASSERT_EQUAL_UINT32(1, 1);
}

void test_dummyTest_message_true(void) {
  TEST_ASSERT_MESSAGE( 1 == 1 , "This string will not get printed ;)");
}

void test_dummyTest_assertion_false(void){
  TEST_ASSERT_EQUAL_UINT32(1, 0);
}

void test_dummyTest_message_false(void) {
  TEST_ASSERT_MESSAGE( 1 == 0 , "1 is not equal to 0 :'(");
}


// unity functions:
void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_dummyTest_assertion_true);
  RUN_TEST(test_dummyTest_message_true);
  //RUN_TEST(test_dummyTest_assertion_false); // this test will fail
  //RUN_TEST(test_dummyTest_message_false); // this test will fail
  UNITY_END();
}
