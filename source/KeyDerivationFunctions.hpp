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

#ifndef KEY_DERIVATION_FUNCTIONS_HPP
#define KEY_DERIVATION_FUNCTIONS_HPP

#include <functional>
#include <map>
#include <string>

enum class KeyDerivationFunctions
{
  HKDF,
};

/** A Key Derivation function which takes the following parameters.
    @param data - The data to derive a key from.
    @param info - An additional string to pass into the Key Derivation Function.
    In the context of SPAKE2, this value is hard-coded to "ConfirmationKeys".
    @param aad  - Additional associated data which may be shared by each 
    endpoint. If it is shared, it must be identical between both parties.
*/
using KeyDerivationFunction = std::function<std::string(const std::string&, 
                                                        const std::string&, 
                                                        const std::string&)>;

/// @brief The HKDF as specified by RFC5869.
extern KeyDerivationFunction HKDF_RFC5869;

/// @brief Map of available Key Derivation Functions
const std::map<KeyDerivationFunctions, KeyDerivationFunction> key_derivation_functions = 
{
  { KeyDerivationFunctions::HKDF, HKDF_RFC5869 }
};

#endif