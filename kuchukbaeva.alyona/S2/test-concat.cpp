#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "evaluator.hpp"

BOOST_AUTO_TEST_CASE(testConcatenationValue) {
  BOOST_CHECK_EQUAL(kuchukbaeva::concatenateNumbers(12, 34), 1234);
  BOOST_CHECK_EQUAL(kuchukbaeva::concatenateNumbers(5, 0), 50);
  BOOST_CHECK_EQUAL(kuchukbaeva::concatenateNumbers(0, 7), 7);
}

BOOST_AUTO_TEST_CASE(testConcatInExpression) {
  std::string expr = "10 << 20 + 5";
  BOOST_CHECK_EQUAL(kuchukbaeva::evaluateExpression(expr), 1025);
}

BOOST_AUTO_TEST_CASE(testConcatenationOverflow) {
  long long large = 1000000000000000000LL;
  BOOST_CHECK_THROW(kuchukbaeva::concatenateNumbers(large, large), std::overflow_error);
}


