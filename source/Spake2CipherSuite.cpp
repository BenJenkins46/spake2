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

#include "Spake2CipherSuite.hpp"

#include "EllipticCurveConstants.hpp"
#include "HashFunctions.hpp"
#include "KeyDerivationFunctions.hpp"
#include "Spake2Constants.hpp"

// ============================================================================
Spake2CipherSuite::
Spake2CipherSuite(Curves                             curve,
                  HashFunctions                      hash_function,
                  KeyDerivationFunctions             key_derivation_function,
                  MessageAuthenticationCodeFunctions mac_function)
  : M            (spake_2_parameters.at(curve).M),
    N            (spake_2_parameters.at(curve).N),
    curve        (curve),
    hash_function(hash_functions.at(hash_function)),
    key_derivation_function(
      key_derivation_functions.at(key_derivation_function)),
    mac_function(mac_functions.at(mac_function))
{
}

// ============================================================================
Spake2CipherSuite::~Spake2CipherSuite()
{
}