#include <gtest/gtest.h>

#include "Spake2.hpp"

class Spake2Tests : public::testing::Test
{
public:
struct GivenValue
{
  std::string A_name;
  std::string B_name;
  std::string w;
  std::string x;
  std::string y;
};

struct TestVector
{
  std::string pA;
  std::string pB;
  std::string K;
  std::string TT;
  std::string hash_TT;
  std::string Ke;
  std::string Ka;
  std::string KcA;
  std::string KcB;
  std::string A_conf;
  std::string B_conf;
};

protected:
  static void SetUpTestSuite   ();
  static void TearDownTestSuite();

  constexpr static unsigned int num_test_vectors =  4u;
  const static GivenValue  given_values   [num_test_vectors];
  const static TestVector  expected_values[num_test_vectors];
  static       Spake2*     alice          [num_test_vectors];
  static       Spake2*     bob            [num_test_vectors];
private:
};