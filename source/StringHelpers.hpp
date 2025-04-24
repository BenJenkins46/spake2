/**                           888                .d8888b.  
                              888               d88P  Y88b 
                              888                      888 
   .d8888b  88888b.   8888b.  888  888  .d88b.       .d88P 
   88K      888 "88b     "88b 888 .88P d8P  Y8b  .od888P"  
   "Y8888b. 888  888 .d888888 888888K  88888888 d88P"      
        X88 888 d88P 888  888 888 "88b Y8b.     888"       
    88888P' 88888P"  "Y888888 888  888  "Y8888  888888888  
            888                                            
            888                                            
            888                                            
*/

#ifndef STRING_HELPERS_HPP
#define STRING_HELPERS_HPP

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Constants.hpp"

/** Splits input into two equal hex-string parts, first_half and second_half.
    input is assumed to be a hex-encoded string, i.e. "0x12345678". The hex
    identifier, "0x" is optional. input must not be empty.
    @param input The hex-string to split in half.
    @param first_half The first half of input.
    @param second_half The second half of input.
    @return True if input was non-empty and successfully split in half.
 */
inline bool splitHexStringInHalf(const std::string& input, 
                                 std::string&       first_half,
                                 std::string&       second_half)
{
  if ( input.empty() )
  {
    return false;
  }

  const std::size_t start_char = ( input.rfind(HEX_PREFIX_LOWERCASE) == 0 || 
                                   input.rfind(HEX_PREFIX_UPPERCASE) == 0 ) 
                                  ? HEX_PREFIX_LEN 
                                  : 0;
  
  const std::size_t midpoint   = ( input.length() - start_char ) / 2;

  first_half.
    assign(HEX_PREFIX_LOWERCASE + input.substr(start_char,  midpoint));
  
  second_half.
    assign(HEX_PREFIX_LOWERCASE + input.substr(start_char + midpoint));

  return true;
}

/** Helper method to convert binary data into a hex-encoded string.
    @param data Pointer to binary data to be encoded.
    @param len  The number of characters pointed to by data.
    @param use_prefix Add the "0x" prefix to the result.
    @return Hex-encoded string representation of data.
 */
inline std::string binaryToHexString(const unsigned char* data, 
                                     std::size_t          len, 
                                     bool                 use_prefix = false)
{
  std::ostringstream ostr;

  if ( use_prefix && len > 0 )
  {
    ostr << HEX_PREFIX_LOWERCASE;
  }

  for ( std::size_t i = 0; i < len; ++i )
  {
    ostr << std::hex << std::setw(2) << std::setfill('0') 
         << static_cast<int>(data[i]);
  }
  return ostr.str();
}

/** Helper to pad a value with num_bytes appropriate bytes, in Little Endian 
    format.
    @param value The value to pad.
    @param num_bytes The number of bytes to pad value with as appropriate.
    @return Hex-encoded string of value padded by num_bytes.
 */
inline std::string padValueLittleEndian(std::size_t value, 
                                        std::size_t num_bytes)
{
  std::ostringstream ostr;

  for ( std::size_t i = 0; i < num_bytes; ++i )
  {
    uint8_t byte = static_cast<uint8_t>((value >> (8 * i)) & 0xFF);
    ostr << std::hex << std::setw(2) << std::setfill('0') 
         << static_cast<int>(byte);
  };

  return ostr.str();
}

/** Helper function to convert a std::string to its ASCII representation in 
    hexadecimal format. 
    @param input The string to convert.
    @return input converted into its ASCII form, in hexadecimal.
*/
inline 
std::string stringToAsciiHex(const std::string& input, bool use_prefix = false)
{
  if ( input.empty() )
  {
    return std::string();
  }

  std::ostringstream ostr;

  if ( use_prefix )
  {
    ostr << HEX_PREFIX_LOWERCASE;
  }

  for (unsigned char c : input)
  {
    ostr << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
  }

  return ostr.str();
}

/** Helper method to convert a Hex-encoded string to its underlying bytes. The 
    input may or may not start with the Hex identifier "0x" or "0X".
    @return A std::vector containing the bytes encoded by input.
 */
inline std::vector<unsigned char> hexStringToBytes(const std::string& input)
{
  if ( input.empty() )
  {
    return std::vector<unsigned char>();
  }

  /// Skip the leading 0x prefix if it exists.
  std::size_t start = ( input.rfind(HEX_PREFIX_LOWERCASE) == 0 || 
                        input.rfind(HEX_PREFIX_UPPERCASE) == 0 ) 
                      ? HEX_PREFIX_LEN 
                      : 0;

  std::vector<unsigned char> underlying_bytes;
  underlying_bytes.reserve(input.length() / 2);
  
  for ( std::size_t i = start; i < input.length(); i += 2 )
  {
    unsigned char byte      = static_cast<unsigned char>(
      std::stoul(input.substr(i, 2).c_str(), nullptr, Base::HEX));
    underlying_bytes.push_back(byte);
  }
  return underlying_bytes;
}

#endif