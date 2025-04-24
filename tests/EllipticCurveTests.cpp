#include <gtest/gtest.h>

#include "EllipticCurve.hpp"

// ============================================================================
TEST(EllipticCurveTests, TestDoubleAndAdd)
{
  const unsigned int         num_tests = 18u;
  EllipticCurve              curve("foo", 2, 2, 17, 1, 21, 2);
  EllipticCurve::Point       P(5, 1);
  
  const EllipticCurve::Point expected_values[num_tests] =
  {
    {5,  1}, {6,  3}, {10,  6}, {3,  1}, {9, 16}, {16, 13}, {0,  6}, {13,  7},
    {7,  6}, {7, 11}, {13, 10}, {0, 11}, {16, 4}, {9,   1}, {3, 16}, {10, 11},
    {6, 14}, {5, 16}
  };
  
  for ( unsigned int i = 1; i < num_tests; ++i )
  {
    ASSERT_TRUE(curve.scalarMultiplication(i, P) == expected_values[i - 1]);
  }
}
