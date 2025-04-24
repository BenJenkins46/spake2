#include <gtest/gtest.h>

#include "StringHelpers.hpp"

// ============================================================================
TEST(SplitHexStringInHalfTest, TestEmptyInputIsValid)
{
  std::string empty_value = "";
  std::string first_half;
  std::string second_half;

  ASSERT_FALSE(splitHexStringInHalf(empty_value, first_half, second_half));
  ASSERT_TRUE(first_half.empty());
  ASSERT_TRUE(second_half.empty());
}

// ============================================================================
TEST(SplitHexStringInHalfTest, TestValidInputWithLowercasePrefix)
{
  std::string test_value = "0x12345678";
  std::string first_half;
  std::string second_half;

  ASSERT_TRUE(splitHexStringInHalf(test_value, first_half, second_half));
  ASSERT_STREQ(first_half.c_str(),  "0x1234");
  ASSERT_STREQ(second_half.c_str(), "0x5678");
}

// ============================================================================
TEST(SplitHexStringInHalfTest, TestValidInputWithUppercasePrefix)
{
  std::string test_value = "0X12345678";
  std::string first_half;
  std::string second_half;

  ASSERT_TRUE(splitHexStringInHalf(test_value, first_half, second_half));
  ASSERT_STREQ(first_half.c_str(),  "0x1234");
  ASSERT_STREQ(second_half.c_str(), "0x5678");
}

// ============================================================================
TEST(SplitHexStringInHalfTest, TestValidInputWithNoPrefix)
{
  std::string test_value = "12345678";
  std::string first_half;
  std::string second_half;

  ASSERT_TRUE(splitHexStringInHalf(test_value, first_half, second_half));
  ASSERT_STREQ(first_half.c_str(),  "0x1234");
  ASSERT_STREQ(second_half.c_str(), "0x5678");
}

// ============================================================================
TEST(BinaryToHexStringTest, TestValidInputNoPrefix)
{
  std::size_t       num_bytes      = 4;
  unsigned char     test_value[]   = {0xDE, 0xAD, 0xBE, 0xEF};
  const std::string expected_value = "deadbeef";

  ASSERT_STREQ(binaryToHexString(test_value, num_bytes, false).c_str(),        
               expected_value.c_str());
}

// ============================================================================
TEST(BinaryToHexStringTest, TestValidInputWithPrefix)
{
  std::size_t       num_bytes      = 4;
  unsigned char     test_value[]   = {0xDE, 0xAD, 0xBE, 0xEF};
  const std::string expected_value = "0xdeadbeef";

  ASSERT_STREQ(binaryToHexString(test_value, num_bytes, true).c_str(),        
               expected_value.c_str());
}

// ============================================================================
TEST(PadValueLittleEndianTest, TestEmptyInput)
{
  const std::string expected_value = "";
  ASSERT_STREQ(expected_value.c_str(), padValueLittleEndian(0, 0).c_str());
}

// ============================================================================
TEST(PadValueLittleEndianTest, TestValidInputOne)
{
  const std::string expected_value = "0100000000";
  ASSERT_STREQ(expected_value.c_str(), padValueLittleEndian(1, 5).c_str());
}

// ============================================================================
TEST(PadValueLittleEndianTest, TestValidInputOneHundred)
{
  const std::string expected_value = "6400000000";
  ASSERT_STREQ(expected_value.c_str(), padValueLittleEndian(100, 5).c_str());
}

// ============================================================================
TEST(StringToAsciiHexTest, TestEmptyInputNoPrefix)
{
  std::string       test_value     = "";
  const std::string expected_value = "";
  ASSERT_STREQ(
    expected_value.c_str(), stringToAsciiHex(test_value, false).c_str());
}

// ============================================================================
TEST(StringToAsciiHexTest, TestEmptyInputWithPrefix)
{
  std::string       test_value     = "";
  const std::string expected_value = "";
  ASSERT_STREQ(
    expected_value.c_str(), stringToAsciiHex(test_value, true).c_str());
}


// ============================================================================
TEST(StringToAsciiHexTest, TestValidInputNoPrefix)
{
  std::string       test_value     = "abcde";
  const std::string expected_value = "6162636465";
  ASSERT_STREQ(
    expected_value.c_str(), stringToAsciiHex(test_value, false).c_str());
}

// ============================================================================
TEST(StringToAsciiHexTest, TestValidInputWithPrefix)
{
  std::string       test_value     = "abcde";
  const std::string expected_value = "0x6162636465";
  ASSERT_STREQ(
    expected_value.c_str(), stringToAsciiHex(test_value, true).c_str());
}

// ============================================================================
TEST(HexStringToBytesTest, TestEmptyInput)
{
  std::string test_value = "";
  ASSERT_TRUE(hexStringToBytes(test_value).empty());
}


// ============================================================================
TEST(HexStringToBytesTest, TestValidInputWithNoPrefix)
{
  std::string test_value                    = "DEADBEEF";
  std::vector<unsigned char> expected_value = {0xDE, 0xAD, 0xBE, 0xEF};
  const auto                 actual_value   = hexStringToBytes(test_value);
  
  ASSERT_TRUE(!actual_value.empty());
  ASSERT_TRUE(actual_value.size() == expected_value.size());

  for ( unsigned int byte = 0; byte < actual_value.size(); ++byte )
  {
    ASSERT_EQ(expected_value.at(byte), actual_value.at(byte));
  }
}

// ============================================================================
TEST(HexStringToBytesTest, TestValidInputWithLowercasePrefix)
{
  std::string test_value                    = "0xDEADBEEF";
  std::vector<unsigned char> expected_value = {0xDE, 0xAD, 0xBE, 0xEF};
  const auto                 actual_value   = hexStringToBytes(test_value);
  
  ASSERT_TRUE(!actual_value.empty());
  ASSERT_TRUE(actual_value.size() == expected_value.size());

  for ( unsigned int byte = 0; byte < actual_value.size(); ++byte )
  {
    ASSERT_EQ(expected_value.at(byte), actual_value.at(byte));
  }
}

// ============================================================================
TEST(HexStringToBytesTest, TestValidInputWithUppercasePrefix)
{
  std::string test_value                    = "0XDEADBEEF";
  std::vector<unsigned char> expected_value = {0xDE, 0xAD, 0xBE, 0xEF};
  const auto                 actual_value   = hexStringToBytes(test_value);
  
  ASSERT_TRUE(!actual_value.empty());
  ASSERT_TRUE(actual_value.size() == expected_value.size());

  for ( unsigned int byte = 0; byte < actual_value.size(); ++byte )
  {
    ASSERT_EQ(expected_value.at(byte), actual_value.at(byte));
  }
}


