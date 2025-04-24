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

#include "MessageAuthenticationCodeFunctions.hpp"

#include "StringHelpers.hpp"
#include <openssl/hmac.h>

/** Perform the RFC2104 MAC function on the given message with the given key.
    Assumes SHA-256.
    @param key The key for the MAC function. Should be a hexadecimal string.
    @param message The message to execute MAC on. Should be a hexadecimal 
    string.
    @return The MAC of message with the given key.
    @cite https://docs.openssl.org/3.0/man3/HMAC/
*/
MessageAuthenticationCodeFunction 
HMAC_RFC2104 = [](const std::string& key, 
                  const std::string& message) -> std::string
{
  auto key_bytes = hexStringToBytes(key);
  auto msg_bytes = hexStringToBytes(message);

  unsigned int  len = EVP_MAX_MD_SIZE;
  unsigned char hmac[EVP_MAX_MD_SIZE];

  if ( HMAC(EVP_sha256(), key_bytes.data(), key_bytes.size(), msg_bytes.data(), 
            msg_bytes.size(), hmac, &len) == NULL )
  {
    throw std::runtime_error("HMAC threw error!");
  } 
  return binaryToHexString(hmac, len);
};