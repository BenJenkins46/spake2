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

#ifndef MPZ_MATH_HELPERS_HPP
#define MPZ_MATH_HELPERS_HPP

#include <random>
#include <string>

#include "Constants.hpp"
#include "gmp.h"

/** Compute a uniform random number from [0, upper_bound)
    @param value The value to put the random number into.
    @param upper_bound The upper bound for the random number. Note that this 
    bound is not included.
*/
inline void uniformRandomNumber(mpz_t& value, const mpz_t& upper_bound)
{
  static bool            initialized = false;
  static gmp_randstate_t random_state;

  if ( !initialized )
  {
    gmp_randinit_default(random_state);
    gmp_randseed_ui     (random_state, std::random_device{}());
    initialized = true;
  }
  mpz_urandomm(value, random_state, upper_bound);
}

/** Helper function to convert mpz_t into a padded string (without "0x" prefix).
    @param num The number to convert and potentially pad.
    @param width The desired padded width of num. If the width of num is less than this number, it is padded by padded_char.
    @param base The base to interpret num as.
    @param padding_char The character to potentially pad num with if it is too short.
    @return String representation of num padded, interpreted as base, with padding_char.
*/
inline std::string padMpz(const mpz_t& num, 
                          std::size_t  width        = 0, 
                          Base         base         = Base::HEX,
                          char         padding_char = '0')
{
  char* hex_str = mpz_get_str(nullptr, base, num);
  std::string result(hex_str);
  free(hex_str);

  if (width > result.length())
  {
    result.insert(0, width - result.length(), padding_char);
  }
  return result;
}

/** Helper function to compute the number of bits which represent n.
    @param n The mpz_t to compute the number of bits for.
    @return The number of bits needed to represent n.
 */
inline std::size_t getMpzNumBits(const mpz_t& n)
{
  return mpz_sizeinbase(n, Base::BINARY);
}

/** Helper function to compute the number of bytes which represent n, rounded 
    up.
    @param n The mpz_t to compute the number of bytes for.
    @return The number of bytes needed to represent n.
 */
inline std::size_t getMpzNumBytes(const mpz_t& n)
{
  return ( ( getMpzNumBits(n) + 7 ) / BITS_PER_BYTE );
}
#endif