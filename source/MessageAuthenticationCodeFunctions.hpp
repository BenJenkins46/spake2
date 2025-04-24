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

#ifndef MESSAGE_AUTHENTICATION_CODE_FUNCTIONS_HPP
#define MESSAGE_AUTHENTICATION_CODE_FUNCTIONS_HPP

#include <functional>
#include <map>
#include <string>

enum class MessageAuthenticationCodeFunctions
{
  HMAC,
};

using MessageAuthenticationCodeFunction = 
  std::function<std::string(const std::string&, const std::string&)>;

/// @brief HMAC MAC specified by RFC2104.
extern MessageAuthenticationCodeFunction HMAC_RFC2104;

/// @brief Map of available MAC functitons. Currently limited to RFC2104.
const 
std::map<MessageAuthenticationCodeFunctions, MessageAuthenticationCodeFunction> mac_functions = 
{
  { MessageAuthenticationCodeFunctions::HMAC, HMAC_RFC2104 }
};

#endif