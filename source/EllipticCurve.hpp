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

#ifndef ELLIPTIC_CURVE_HPP
#define ELLIPTIC_CURVE_HPP

#include <sstream>
#include <string>

#include "Constants.hpp"
#include "EllipticCurveConstants.hpp"
#include "MpzMathHelpers.hpp"
#include <gmp.h>

/** Class defining an Elliptic Curve, defined as an algebraic curve of the 
    form y^2 = x^3 + ax + b. An Elliptic Curve has no cusps or 
    self-intersections. This class provides the basic Elliptic Curve operations
    such as multiplication and addition. Scalar multiplication is also 
    supported. A NIST-recommended curve is predefined within 
    EllipticCurveConstants.hpp, and may be instantiated by using this class' 
    secondary constructor.
*/
class EllipticCurve
{
public:

  /** Construct a new Elliptic Curve.
      @param curve_name_in The name for this curve.
      @param a_in This curve's A parameter.
      @param b_in This curve's B parameter.
      @param p_in This curve's prime modulus.
      @param h_in This curve's cofactor.
      @param n_in The number of elements within this curve, including the point 
      of infinity.
      @param field_size_bytes_in The size of the field for this curve, in bytes.
   */
  EllipticCurve(const std::string& curve_name_in,
                unsigned int       a_in,
                unsigned int       b_in,
                unsigned int       p_in,
                unsigned int       h_in,
                unsigned int       n_in,
                unsigned int       field_size_bytes_in);

  /** Construct an Elliptic Curve with a predefined curve.
      @param curve_name A NIST Recommended curve. Currently, this is limited to 
      curve P-256.
   */
  EllipticCurve(Curves curve_name);

  /// @brief The descructor clears memory allocated by mpz_init().
  ~EllipticCurve();
  
  /** Structure defining a Point on an Elliptic Curve. A point has an x and y   
      coordinate, or may be at the point of infinity.
  */
  struct Point
  {
    Point()
      : x(), y(), at_infinity(true)
    {
      mpz_init(x);
      mpz_init(y);
    }
    Point(unsigned int x_in, unsigned int y_in)
      : x(), y(), at_infinity(false)
    {
      mpz_init_set_ui(x, x_in);
      mpz_init_set_ui(y, y_in);
    }
    Point(const std::string& x_in, const std::string& y_in, Base base)
    : x(), y(), at_infinity(false)
    {
      mpz_init_set_str(x, x_in.c_str(), base);
      mpz_init_set_str(y, y_in.c_str(), base);
    }
    Point(const mpz_t& x_in, const mpz_t& y_in)
      : x(), y(), at_infinity(false)
    {
      mpz_init_set(x, x_in);
      mpz_init_set(y, y_in);
    }

    ~Point()
    {
      mpz_clear(x);
      mpz_clear(y);
    }

    Point(const Point& other)
    {
      mpz_init_set(x, other.x);
      mpz_init_set(y, other.y);
      at_infinity = other.at_infinity;
    }
  
    Point& operator=(const Point& other)
    {
      if (this != &other)
      {
        mpz_set(x, other.x);
        mpz_set(y, other.y);
        at_infinity = other.at_infinity;
      }
      return *this;
    }

    bool operator!=(const Point& object) const
    {
      return !(*this == object);
    }

    bool operator==(const Point& object) const
    {
      return mpz_cmp(x, object.x) == 0 &&
             mpz_cmp(y, object.y) == 0 &&
             at_infinity == object.at_infinity;
    }

    std::size_t getUncompressedByteCount() const
    {
      /// Add in the "0x04" lead byte.
      return 1 + getRawByteCount();
    }

    std::size_t getRawByteCount() const
    {
      return getMpzNumBytes(x) + getMpzNumBytes(y);
    }

    std::string getUncompressedFormat(unsigned int field_size_bytes, 
                                      bool         preface_hex = true) const
    {
      std::stringstream result;
      std::size_t       len      = field_size_bytes * 2;
      std::string       x_padded = padMpz(x, len, Base::HEX);
      std::string       y_padded = padMpz(y, len, Base::HEX);

      result << ( preface_hex ? "0x04" : "04" ) << x_padded << y_padded;
      return result.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point);

    mpz_t x;
    mpz_t y;
    bool  at_infinity;
  };
  
  /** Accessor for the name of this curve.
      @return The name of this curve.
   */
  const std::string& getCurveName() const;

  /** Accessor for the Prime Modulus, p, for this curve. The Prime Modulus is
      the prime number that defines the finite field in which the curve is 
      defined.
      @return Reference to the Prime Modulus, p.
  */
  const mpz_t& getPrimeModulus() const;
  
  /** Accessor for this curve's 'a' parameter.
      @return Reference to the curve parameter a.
  */
  const mpz_t& getA() const;

  /** Accessor for this curve's 'b' parameter.
      @return Reference to the curve parameter b.
  */
  const mpz_t& getB() const;
 
  /** Accessor for the Order, n, for this curve. The Order is the number of 
      points on the curve.
      @return Reference to the Order, n.
  */
  const mpz_t& getOrder() const;
  
  /** Accessor for the Generator for this curve.
      @return Reference to the generator point.
  */
  const Point& getGenerator() const;

  /** Accessor for the field size of this curve, in bytes.
      @return The field size, in bytes.
   */
  unsigned int getFieldSizeBytes() const;

  /** Accessor for this curve's cofactor, h.
      @return Const reference to the cofactor of this curve.
   */
  const mpz_t& getCofactor() const;

  /** Operate on two points on this curve. If P == Q, point doubling is 
      performed. Otherwise, point addition is performed. 
      @param P The first point on this curve.
      @param Q The second point on this curve.
      @return If P == Q, point doubling of P and Q. If P != Q, point addition
      of P and Q. If P is at infinity, Q is returned. If Q is at infinity, P 
      is returned.
   */
  Point operate(const Point& P, const Point& Q) const;

  /** Performs the Double-and-Add Algorithm to perform Point Multiplication.
      This is analogous to T = dP, where d is a scalar and P is a point.
      @param scalar The scalar to multiply P by.
      @param point  The point on the curve to be multiplied.
   */
  Point scalarMultiplication(unsigned int scalar, const Point& point) const;
  
  /** Performs the Double-and-Add Algorithm to perform Point Multiplication.
      This is analogous to T = dP, where d is a scalar and P is a point.
      @param scalar The scalar to multiply P by.
      @param point  The point on the curve to be multiplied.
  */
  Point scalarMultiplication(const mpz_t& scalar, const Point& point) const;

  /** Find the negative of the given point, P. The negative of a point, -P, is
      defined as -P = (x, -y), where the point is reflected across the X axis.
      The inverse of infinity is itself.
      @param P The point on the curve to find the negation of.
      @return The negation of P. If P was at infinity, P is returned.
   */
  Point negatePoint(const EllipticCurve::Point& P) const;

protected:
private:

  /// @brief The name for this elliptic curve.
  std::string curve_name;

  /// @brief This curve's a parameter.
  mpz_t a;

  /// @brief This curve's b parameter.
  mpz_t b;

  /// @brief This curve prime modulus.
  mpz_t p;

  /// @brief This curve's cofactor.
  mpz_t h;

  /// @brief This curves order, or number of points.
  mpz_t n;

  /// @brief This curve's defined generator element.
  Point generator;

  /// @brief The field size of this curve, in bytes.
  unsigned int field_size_bytes;

  /** Find the multiplicative inverse of a point on this curve. 
      @param P The value to find the inverse of.
      @param result The result to place the inverse of a into, if it exists.
      @return True if the inverse exists, and result was updated.
  */
  bool getInverse(const mpz_t& P, mpz_t result) const;

  /** Perform point doubling for two points on this curve.
      @param P The first point to double.
      @param Q The second point to double.
      @return P + Q.
  */
  Point pointDoubling(const Point& P, const Point& Q) const;

  /** Perform point doubling for two points on this curve.
    @param P The first point to double.
    @param Q The second point to double.
    @return PQ.
  */
  Point pointAddition(const Point& P, const Point& Q) const;

  /// Both copy assignment and copy constructors are deleted.
  EllipticCurve operator=(const EllipticCurve& object) = delete;
  EllipticCurve          (const EllipticCurve& object) = delete;
};

// ============================================================================
inline const std::string& EllipticCurve::getCurveName() const
{
  return curve_name;
}

// ============================================================================
inline const mpz_t& EllipticCurve::getA() const
{
  return a;
}

// ============================================================================
inline const mpz_t& EllipticCurve::getB() const
{
  return b;
}

// ============================================================================
inline const mpz_t& EllipticCurve::getPrimeModulus() const
{
  return p;
}

// ============================================================================
inline const mpz_t& EllipticCurve::getOrder() const
{
  return n;
}

// ============================================================================
inline const mpz_t& EllipticCurve::getCofactor() const
{
  return h;
}

// ============================================================================
inline const EllipticCurve::Point& EllipticCurve::getGenerator() const
{
  return generator;
}

// ============================================================================
inline unsigned int EllipticCurve::getFieldSizeBytes() const
{
  return field_size_bytes;
}

// ============================================================================
inline bool EllipticCurve::getInverse(const mpz_t& P, mpz_t result) const
{
  return mpz_invert(result, P, getPrimeModulus()) != 0;
}

// ============================================================================
inline EllipticCurve::Point EllipticCurve::
scalarMultiplication(unsigned int scalar_ui, const Point& point) const
{
  mpz_t scalar;
  mpz_init_set_ui(scalar, scalar_ui);
  Point T = scalarMultiplication(scalar, point);
  mpz_clear(scalar);
  return T;
}
#endif