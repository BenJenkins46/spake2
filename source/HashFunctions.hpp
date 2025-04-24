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

#ifndef HASH_FUNCTIONS_HPP
#define HASH_FUNCTIONS_HPP

#include <functional>
#include <map>
#include <string>

enum class HashFunctions
{
  SHA256,
};

using HashFunction = std::function<std::string(const std::string&)>;

extern HashFunction SHA_256;

/// @brief Map to store available hash functions. Currently limited to SHA-256
const std::map<HashFunctions, HashFunction> hash_functions
{
  { HashFunctions::SHA256, SHA_256 }
};

#endif