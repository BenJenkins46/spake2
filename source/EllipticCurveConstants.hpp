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

#ifndef ELLIPTIC_CURVE_CONSTANTS_HPP
#define ELLIPTIC_CURVE_CONSTANTS_HPP

#include <map>
#include <string>

enum class Curves
{
  P256,
};

/// @brief Core parameters defining an Elliptic Curve.
struct CurveParameters
{
  /// @brief Curve name
  const char * name;

  /// @brief Prime Modulus
  const char * p;

  /// @brief Order (number of elements)
  const char * n;
  
  /// @brief Generator point
  const char * gx;
  const char * gy;

  /// @brief Curve parameters
  const char * a;
  const char * b;

  /// @brief Cofactor
  const char * h;

  unsigned int field_size_bytes;
};

/** @brief NIST-published recommended curve parameters.
    Source : Section 3.2.1.3 of :
    https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186.pdf
*/
const std::map<Curves, CurveParameters> curve_parameters = 
{
  { Curves::P256, 
    { 
      "P-256",
      "115792089210356248762697446949407573530086143415290314195533631308867097853951", // p
      "115792089210356248762697446949407573529996955224135760342422259061068512044369", // n
      "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296",               // gx
      "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5",               // gy
      "-3",                                                                             // a
      "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b",               // b
      "1",                                                                              // h
      32
    }
  } // p256
};

#endif