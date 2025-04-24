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

#ifndef SPAKE_2_CONSTANTS_HPP
#define SPAKE_2_CONSTANTS_HPP

#include <map>

#include "Constants.hpp"
#include "EllipticCurve.hpp"

enum Mode
{
  CLIENT    = 0,
  SERVER    = 1,
  NUM_MODES = 2
};

struct Spake2Keys
{
  const EllipticCurve::Point M;
  const EllipticCurve::Point N;
};

/** @brief Blinding factors for SPAKE2.
    Future iterations of SPAKE2 should self-derive them.
    @cite https://github.com/jiep/spake2plus/blob/main/spake2plus/ciphersuites/ciphersuites.py#L14
*/
const std::map<Curves, Spake2Keys> spake_2_parameters =
{
  { Curves::P256, 
    {
      EllipticCurve::Point("61709229055687782219344352628424647386531596507379261315813478518843566432559",
                           "43399651700267013692148409492066214468674361939146464406474584691695279811872",
                            Base::DECIMAL),
      EllipticCurve::Point("98031458012971070369465795029179261841266230867477002166417845678366165379913",
                           "3544368724946236282841049099645644789675854804295951046212527731618188549095",
                            Base::DECIMAL)
    }
  }
};

#endif