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

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstddef>

/// @brief This file contains various constants required by SPAKE2.

enum Base
{
  BINARY  = 2,
  OCTAL   = 8,
  DECIMAL = 10,
  HEX     = 16
};

const std::string HEX_PREFIX_LOWERCASE = "0x";
const std::string HEX_PREFIX_UPPERCASE = "0X";

const std::string UNCOMPRESSED_PREFIX  = "0x04";

const std::size_t HEX_PREFIX_LEN         = HEX_PREFIX_LOWERCASE.length();
const std::size_t UNCOMPRESSED_PREFIX_LEN = UNCOMPRESSED_PREFIX.length();
const unsigned int BITS_PER_BYTE = 8u;

#endif