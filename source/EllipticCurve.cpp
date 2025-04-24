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

#include "EllipticCurve.hpp"

#include <iostream>
#include <string>

#include "EllipticCurveConstants.hpp"

// ============================================================================
EllipticCurve::EllipticCurve(const std::string& curve_name_in,
                             unsigned int       a_in, 
                             unsigned int       b_in, 
                             unsigned int       p_in,
                             unsigned int       h_in,
                             unsigned int       n_in,
                             unsigned int       field_size_bytes_in)
  : curve_name      (curve_name_in),
    a               (),
    b               (),
    p               (),
    h               (),
    n               (),
    generator       (),
    field_size_bytes(field_size_bytes_in)
{
  mpz_init_set_ui(a, a_in);
  mpz_init_set_ui(b, b_in);
  mpz_init_set_ui(p, p_in);
  mpz_init_set_ui(h, h_in);
  mpz_init_set_ui(n, n_in);
}

// ============================================================================
EllipticCurve::EllipticCurve(Curves curve_name_in)
  : curve_name      (curve_parameters.at(curve_name_in).name),
    a               (),
    b               (),
    p               (),
    h               (),
    n               (),
    generator       (),
    field_size_bytes(curve_parameters.at(curve_name_in).field_size_bytes)
{
  mpz_init_set_str(a,           curve_parameters.at(curve_name_in).a,  10);
  mpz_init_set_str(b,           curve_parameters.at(curve_name_in).b,  16);
  mpz_init_set_str(p,           curve_parameters.at(curve_name_in).p,  10);
  mpz_init_set_str(h,           curve_parameters.at(curve_name_in).h,  10);
  mpz_init_set_str(n,           curve_parameters.at(curve_name_in).n,  10);
  mpz_init_set_str(generator.x, curve_parameters.at(curve_name_in).gx, 16);
  mpz_init_set_str(generator.y, curve_parameters.at(curve_name_in).gy, 16); 
}

// ============================================================================
EllipticCurve::~EllipticCurve()
{
  mpz_clears(a, b, p, h, n, nullptr);
}

// ============================================================================
EllipticCurve::Point 
EllipticCurve::operate(const Point& P, const Point& Q) const
{
  if ( P.at_infinity )
  {
    return Q;
  }

  if ( Q.at_infinity )
  {
    return P;
  }

  return ( mpz_cmp(P.x, Q.x) == 0 && mpz_cmp(P.y, Q.y) == 0 )
    ? pointDoubling(P, Q) 
    : pointAddition(P, Q);
}

// ============================================================================
EllipticCurve::
Point EllipticCurve::pointAddition(const Point& P, const Point& Q) const
{
  Point result;

  mpz_t numerator;
  mpz_t denominator;
  mpz_t denominator_inverse;
  mpz_t x_squared;
  mpz_t x_squared_times_three;
  mpz_t slope;
  mpz_t intermediate;
  
  mpz_inits(numerator, 
            denominator, 
            denominator_inverse,
            x_squared,
            x_squared_times_three,
            slope, 
            intermediate,
            nullptr);
  
  /// y2 - y1
  mpz_sub(numerator, Q.y, P.y);

  /// x2 - x1
  mpz_sub(denominator, Q.x, P.x);

  /// (x2 - x1)^-1
  getInverse(denominator, denominator_inverse);

  /// s = (y2 - y1)((x2 - x1)^-1)
  mpz_mul(slope, numerator, denominator_inverse);

  /// x3 = s^2 - x1 - 2x mod p
  mpz_mul(result.x, slope, slope);
  mpz_sub(result.x, result.x, P.x);
  mpz_sub(result.x, result.x, Q.x);
  mpz_mod(result.x, result.x, getPrimeModulus());

  /// y3 = s(x1 - x3) - y1 mod p
  mpz_sub(intermediate, P.x, result.x);
  mpz_mul(result.y, slope, intermediate);
  mpz_sub(result.y, result.y, P.y);
  mpz_mod(result.y, result.y, getPrimeModulus());
  
  mpz_clears(numerator, 
             denominator, 
             denominator_inverse,
             x_squared,
             x_squared_times_three,
             slope, 
             intermediate,
             nullptr);

  result.at_infinity = false;
  return result;
}

// ============================================================================
EllipticCurve::Point 
EllipticCurve::pointDoubling(const Point& P, const Point& Q) const
{
  Point result;

  mpz_t numerator;
  mpz_t denominator;
  mpz_t denominator_inverse;
  mpz_t x_squared;
  mpz_t x_squared_times_three;
  mpz_t intermediate;
  mpz_t slope;

  mpz_inits(numerator, 
            denominator, 
            denominator_inverse,
            x_squared,
            x_squared_times_three,
            intermediate,
            slope, 
            nullptr);

  /// 3x1^2 + a
  mpz_mul   (x_squared,             P.x,                   P.x);
  mpz_mul_ui(x_squared_times_three, x_squared,             3ul);
  mpz_add   (numerator,             x_squared_times_three, getA());

  /// (2y1)^-1
  mpz_mul_ui(denominator, P.y, 2ul);
  getInverse(denominator, denominator_inverse);

  /// (3x1^2 + a)((2y1)^-1) % p
  mpz_mul(slope, numerator, denominator_inverse);
  mpz_mod(slope, slope,     getPrimeModulus());

  /// x3 = s^2 - x1 - 2x mod p
  mpz_mul(result.x, slope,    slope);
  mpz_sub(result.x, result.x, P.x);
  mpz_sub(result.x, result.x, Q.x);
  mpz_mod(result.x, result.x, getPrimeModulus());

  /// y3 = s(x1 - x3) - y1 mod p
  mpz_sub(intermediate, P.x,      result.x);
  mpz_mul(result.y,     slope,    intermediate);
  mpz_sub(result.y,     result.y, P.y);
  mpz_mod(result.y,     result.y, getPrimeModulus());

  mpz_clears(numerator, 
             denominator, 
             denominator_inverse,
             x_squared,
             x_squared_times_three,
             intermediate,
             slope, 
             nullptr);

  result.at_infinity = false;
  return result;
}

// ============================================================================
EllipticCurve::Point 
EllipticCurve::scalarMultiplication(const mpz_t& scalar, const Point& P) const
{
  /// Ignore the initial assignment bit (leftmost).
  const unsigned int num_bits = mpz_sizeinbase(scalar, 2) - 1;
  
  Point T(P);
  
  for ( int i = num_bits - 1; i >= 0; --i )
  {
    T = pointDoubling(T, T);
    /// If bit is one.
    if ( mpz_tstbit(scalar, i) )
    {
      T = pointAddition(T, P);
    }
  }
  return T;
}

// ============================================================================
EllipticCurve::Point 
EllipticCurve::negatePoint(const EllipticCurve::Point& P) const
{
  if ( P.at_infinity )
  {
    return P;
  }

  EllipticCurve::Point result;

  // Negation is reflected across the x-axis, so x_neg = x
  mpz_set(result.x, P.x);

  // y_neg = -y mod p
  mpz_neg(result.y, P.y);         
  mpz_mod(result.y, result.y, p); 
  
  result.at_infinity = false;
  return result;
}

std::ostream& operator<<(std::ostream& os, const EllipticCurve::Point& P)
{
  if (P.at_infinity)
  {
    os << "Point at Infinity\n";
  }
  else
  {
    gmp_printf("P = (0x%Zx, 0x%Zx)\n",  P.x, P.y);
  }
  return os;
}