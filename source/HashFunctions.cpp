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

#include "HashFunctions.hpp"

#include <sstream>
#include <string>

#include "StringHelpers.hpp"

#include <openssl/sha.h>

/** Perform the SHA-256 hash of the input.
    @param input The input to take the SHA-256 hash of.
    @return The SHA-256 hash of input.
    @cite https://docs.openssl.org/3.1/man3/SHA256_Init/
 */
HashFunction SHA_256 = [](const std::string& input) -> std::string
{
  unsigned char              hash[SHA256_DIGEST_LENGTH];
  std::vector<unsigned char> bytes = hexStringToBytes(input);
  
  SHA256(bytes.data(), bytes.size(), hash);
  
  return binaryToHexString(hash, SHA256_DIGEST_LENGTH, true);
};