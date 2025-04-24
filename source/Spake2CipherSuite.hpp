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

#ifndef SPAKE_2_CIPHER_SUITE_HPP
#define SPAKE_2_CIPHER_SUITE_HPP

#include <string>

#include "EllipticCurve.hpp"
#include "HashFunctions.hpp"
#include "KeyDerivationFunctions.hpp"
#include "MessageAuthenticationCodeFunctions.hpp"

/** Class to represent a Ciphersuite for SPAKE2. A Ciphersuite is comprised of
    an Elliptic Curve, Hash Function, Key Derivation Function, and MAC function.
    The currently supported CipherSuite is PAKE2-P256-SHA256-HKDF-HMAC

    SPAKE2 Source       : https://www.rfc-editor.org/rfc/rfc9382.html
    Source of M/N Value : https://github.com/jiep/spake2plus/blob/main/spake2plus/ciphersuites/ciphersuites.py#L14
 
 */
class Spake2CipherSuite
{
public:

  /** Construct a new Spake2CipherSuite instance.
      @param curve The desired Elliptic Curve.
      @param hash_function The desired Hash Function.
      @param key_derivation_function The desired key derivation function.
      @param mac_function The desired MAC function.
   */
  Spake2CipherSuite(Curves                             curve,
                    HashFunctions                      hash_function,
                    KeyDerivationFunctions             key_derivation_function,
                    MessageAuthenticationCodeFunctions mac_function);
  
  /// @brief The destructor does nothing.
  ~Spake2CipherSuite();

  /** Accessor for the Point Generation Point M.
      @return The M Point Generation Point.
      @note This is currently fixed to the value of the point generation seed
      for P-256 as given in the paper at the top of this class. The value of
      the point of this seed was obtained from the second link at the top of this class.
   */
  const EllipticCurve::Point& getM() const;

  /** Accessor for the Point Generation Point N.
      @return The N Point Generation Point.
      @note This is currently fixed to the value of the point generation seed
      for P-256 as given in the paper at the top of this class. The value of
      the point of this seed was obtained from the second link at the top of this class.
  */
  const EllipticCurve::Point& getN() const;

  /** Accessor for this Ciphersuite's Elliptic Curve.
      @return Const-reference to the chosen Elliptic Curve.
  */
  const EllipticCurve& getCurve() const;

  /** Accessor for this Ciphersuite's Hash Function.
      @return Const-reference to the chosen Hash Function.
  */
  const HashFunction& getHashFunction() const;

  /** Accessor for this Ciphersuite's Key Derivation Function.
      @return Const-reference to the chosen Key Derivation Function.
  */
  const KeyDerivationFunction& getKeyDerivationFunction() const;

  /** Accessor for this Ciphersuite's MAC Function.
      @return Const-reference to the chosen MAC Function.
  */
  const MessageAuthenticationCodeFunction& getMacFunction() const;

protected:
private:

  /// @brief Blinding factors
  const EllipticCurve::Point M;
  const EllipticCurve::Point N;

  /// @brief The Elliptic Curve this Ciphersuite is using.
  EllipticCurve                     curve;

  /// @brief The Hash Function this Ciphersuite is using.
  HashFunction                      hash_function;

  /// @brief The Key Derivation Function this Ciphersuite is using.
  KeyDerivationFunction             key_derivation_function; 

  /// @brief The MAC Function this Ciphersuite is using.
  MessageAuthenticationCodeFunction mac_function;

  /// @brief Copy constructor and assignment operator are deleted.
  Spake2CipherSuite          (const Spake2CipherSuite& object) = delete;
  Spake2CipherSuite operator=(const Spake2CipherSuite& object) = delete;
};

// ============================================================================
inline const EllipticCurve::Point& Spake2CipherSuite::getM() const
{
  return M;
}
// ============================================================================
inline const EllipticCurve::Point& Spake2CipherSuite::getN() const
{
  return N;
}

// ============================================================================
inline const EllipticCurve& Spake2CipherSuite::getCurve() const
{
  return curve;
}

// ============================================================================
inline const HashFunction& Spake2CipherSuite::getHashFunction() const
{
  return hash_function;
}

// ============================================================================
inline 
const KeyDerivationFunction& Spake2CipherSuite::getKeyDerivationFunction() const
{
  return key_derivation_function;
}

// ============================================================================
inline const 
MessageAuthenticationCodeFunction& Spake2CipherSuite::getMacFunction() const
{
  return mac_function;
}
#endif