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

#include "Spake2.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

#include "EllipticCurveConstants.hpp"
#include "HashFunctions.hpp"
#include "KeyDerivationFunctions.hpp"
#include "MessageAuthenticationCodeFunctions.hpp"
#include "Spake2Constants.hpp"
#include "StringHelpers.hpp"
#include "sodium.h"

using std::cout;
using std::endl;
// ============================================================================
Spake2::Spake2(const std::string&                 identity_in, 
               const std::string&                 password_in,
               bool                               client,
               const std::string&                 addl_auth_data,
               Curves                             curve,
               HashFunctions                      hash_function,
               KeyDerivationFunctions             key_derivation_function,
               MessageAuthenticationCodeFunctions mac_function)
  : identity                 (identity_in),
    mode                     (client ? Mode::CLIENT : Mode::SERVER),
    cipher_suite             (curve, 
                              hash_function, 
                              key_derivation_function, 
                              mac_function),
    k_pub                    (),
    K                        (),
    transcript               (),
    transcript_hash          (),
    symmetric_secrets        (),
    mac_keys                 (),
    addl_auth_data           (addl_auth_data),
    confirmation_key         (),
    expected_key             (),
    other_party_identity     (),
    other_party_public_key   ()
{
  cout << "SPAKE2 with identity \"" << identity << "\" running in " 
            << getMode() << " mode. EC = "         
            << cipher_suite.getCurve().getCurveName() << endl;
  
  transcript.      reserve(1024u);
  transcript_hash. reserve(transcript.capacity());
  confirmation_key.reserve(transcript.capacity());

  mpz_inits(w, xy, k_pri, nullptr);  

  /// Pick x / y randomly and uniformly in the range [0, p)
  uniformRandomNumber(k_pri, cipher_suite.getCurve().getPrimeModulus());

  /// Compute the shared integer, w, assuming A and B have pre-shared password.
  computeW           (password_in);
}

// ============================================================================
Spake2::~Spake2()
{
  mpz_clears(w, xy, k_pri, nullptr);  
}

// ============================================================================
bool Spake2::readOtherPartiesPublicKey()
{
  cout << "Execute " << getOtherMode() << "'s setup phase in another "
  "instance in the same directory, then enter any key to continue." << endl;
  
  /// Assume automated test environment.
#ifndef CMAKE_TESTING_ENABLED
  std::string junk;
  std::cin >> junk;
#endif

  /// We expect the OTHER party's name to have generated the file.
  const std::string expected_filename = 
    "spake2_" + getOtherMode() + "_kpub.key";
  
  std::ifstream infile(expected_filename);

  if ( !infile.good() )
  {
    std::cerr << "Error ingesting other party's public key!"      << endl;
    std::cerr << "Attempted to open file: " << expected_filename  << endl;
    std::cerr << "Ensure the other party has generated their public key into "
                 "the same directory as this instance of SPAKE2 and try again."
              << endl;
    return false;
  }

  std::string line;

  std::string other_party_public_key_uncompressed;
  if ( std::getline(infile, line) )
  {
    std::istringstream stream(line);
    std::getline(stream, other_party_identity, ',');
    std::getline(stream, other_party_public_key_uncompressed);
  }

  cout << "Successfully read other party's identity as \""        
       << other_party_identity << "\"." << endl;
  
  /// The public key is in uncompressed format. Convert to a compressed point.
  /// Skip the leading 0x04
  std::string other_party_public_key_no_prefix = 
    other_party_public_key_uncompressed.substr(UNCOMPRESSED_PREFIX_LEN);
  
  std::size_t coordinate_length = other_party_public_key_no_prefix.length() / 2;

  other_party_public_key.at_infinity = false;

  mpz_set_str(other_party_public_key.x, 
    other_party_public_key_no_prefix.substr(0, coordinate_length).c_str(),
    Base::HEX);
  mpz_set_str(other_party_public_key.y, 
    other_party_public_key_no_prefix.substr(coordinate_length)   .c_str(),
    Base::HEX);

  infile.close();
  return true;
}

// ============================================================================
bool Spake2::readOtherPartiesConfirmationKey()
{
  cout << "Execute " << getOtherMode() << "'s key derivation phase in "
          "another instance in the same directory, then enter any key "
          "to continue." << endl;

  /// Assume automated testing environment.
#ifndef CMAKE_TESTING_ENABLED
  std::string junk;
  std::cin >> junk;
#endif

  /// We expect the OTHER party's name to have generated the file.
  const std::string expected_filename = 
    "spake2_" + getOtherMode() + "_kconf.key";
  
  std::ifstream infile(expected_filename);

  if ( !infile.good() )
  {
    std::cerr << "Error ingesting other party's confirmation key!" << endl;
    std::cerr << "Attempted to open file: " << expected_filename   << endl;
    std::cerr << "Ensure the other party has generated their confirmation key "
                 "into the same directory as this instance of SPAKE2 and try again." << endl;
    return false;
  }

  std::string line;

  std::string new_other_party_identity;
  std::string new_other_party_confirmation_key;
  if ( std::getline(infile, line) )
  {
    std::istringstream stream(line);
    std::getline(stream, new_other_party_identity, ',');
    std::getline(stream, new_other_party_confirmation_key);
  }

  /// Ensure the other party's identity hasn't changed since public key phase.
  if ( other_party_identity != new_other_party_identity )
  {
    std::cerr << "Other party identity mismatch! Read identity was \""
              << new_other_party_identity << "\" Expected identity was \""
              << other_party_identity << "\"" << endl;

    return false;
  } 

  cout << "Confirmed other party's identity as \""        
            << other_party_identity << "\"." << endl;
  cout << "Read confirmation key " << new_other_party_confirmation_key << endl;
  
  other_party_confirmation_key.assign(new_other_party_confirmation_key);

  infile.close();
  return true;
}

// ============================================================================
void Spake2::transmitPublicKey() const
{
  const std::string filename = "spake2_" + getMode() + "_kpub.key";
  
  std::ofstream outfile(filename);

  if ( !outfile.good() )
  {
    std::cerr << "Error saving public key!" << endl;
    std::abort();
  }

  outfile << identity << "," << getUncompressedPublicKey() << endl;

  outfile.close();

  cout << "Setup phase complete." << endl;
  gmp_printf(" w    = %#Zx\n", w);
  cout << " kpub = " << getUncompressedPublicKey()   << endl;
  cout << "Public key successfully written to file." << endl;
}

// ============================================================================
void Spake2::transmitConfirmationKey() const
{
  const std::string filename = "spake2_" + getMode() + "_kconf.key";
  
  std::ofstream outfile(filename);

  if ( !outfile.good() )
  {
    std::cerr << "Error saving confirmation key!" << endl;
    std::abort();
  }

  outfile << identity << "," << confirmation_key << endl;

  outfile.close();

  cout << "Confirmation key successfully written to file. " 
            << "Key derivation phase complete."             << endl;
  cout << "  k_conf = " << confirmation_key                 << endl;
  cout << "Will expect the following confirmation key: \n"
            << "  " << expected_key                         << endl;
}

// ============================================================================
void Spake2::computeW(const std::string& pw)
{
  /// Take mod p of a hash 64 bits longer than needed to represent p.
  const std::size_t hash_bytes = 
    ( getMpzNumBits(cipher_suite.getCurve().getPrimeModulus()) + 64 ) / BITS_PER_BYTE;

  std::vector<unsigned char> hash(hash_bytes);
  const unsigned char* salt = reinterpret_cast<const unsigned char*>("foo");

  if (crypto_pwhash(hash.data(), 
                    sizeof(hash),
                    pw.c_str(), 
                    pw.length(),
                    salt,
                    crypto_pwhash_OPSLIMIT_MODERATE,
                    crypto_pwhash_MEMLIMIT_MODERATE,
                    crypto_pwhash_ALG_DEFAULT) != 0)
  {
    throw std::runtime_error("pwhash failure.");
  }

  /// Take w = MHF(pw) % p
  mpz_import(w, sizeof(hash), 1, 1, 0, 0, hash.data());
  mpz_mod   (w, w, cipher_suite.getCurve().getPrimeModulus());
}

// ============================================================================
void Spake2::computeGroupElement()
{
  mpz_t    h_x_or_y;
  mpz_init(h_x_or_y);

  /// w{M/N}
  const EllipticCurve::Point wM_or_N = 
    cipher_suite.getCurve().scalarMultiplication(w, 
      ( mode == Mode::CLIENT ) ? cipher_suite.getN() : cipher_suite.getM());

  /// (p{A/B} - w*{M/N}
  const EllipticCurve::Point temp    = 
    cipher_suite.getCurve().operate(
      other_party_public_key, cipher_suite.getCurve().negatePoint(wM_or_N));

  mpz_mul(h_x_or_y, cipher_suite.getCurve().getCofactor(), k_pri);
  
  K = cipher_suite.getCurve().scalarMultiplication(h_x_or_y, temp);

  mpz_clear(h_x_or_y);
}

// ============================================================================
void Spake2::computeTranscript()
{
  std::ostringstream ostr;
  ostr << HEX_PREFIX_LOWERCASE;

  const std::string this_kpub   = 
    k_pub.getUncompressedFormat(
      cipher_suite.getCurve().getFieldSizeBytes(), false);
  
  const std::string other_kpub  = 
    other_party_public_key.getUncompressedFormat(
      cipher_suite.getCurve().getFieldSizeBytes(), false);

  const std::string K_str       =
    K.getUncompressedFormat(cipher_suite.getCurve().getFieldSizeBytes(), false);

  std::size_t       w_num_bytes = ( mpz_sizeinbase(w, 2) + 7 ) / 8;

  /// len(A) || A || len(B) || B || len(pA) || pA || len(pB) || pB
  if ( mode == Mode::CLIENT )
  {
    ostr << padValueLittleEndian(getIdentity(). length(),                           8) << stringToAsciiHex(getIdentity())
         << padValueLittleEndian(other_party_identity.length(),                     8) << stringToAsciiHex(other_party_identity)
         << padValueLittleEndian(k_pub.getUncompressedByteCount(),                  8) << this_kpub
         << padValueLittleEndian(other_party_public_key.getUncompressedByteCount(), 8) << other_kpub;
  }
  else
  {
    ostr << padValueLittleEndian(other_party_identity.length(),                     8) << stringToAsciiHex(other_party_identity) 
         << padValueLittleEndian(getIdentity(). length(),                           8) << stringToAsciiHex(getIdentity())
         << padValueLittleEndian(other_party_public_key.getUncompressedByteCount(), 8) << other_kpub
         << padValueLittleEndian(k_pub.getUncompressedByteCount(),                  8) << this_kpub;
  }
  
  /// || len(K) || K || len(w) || w
  ostr << padValueLittleEndian(K.getUncompressedByteCount(), 8) << K_str;
  ostr << padValueLittleEndian(w_num_bytes, 8)                  << padMpz(w, w_num_bytes * 2);

  transcript.assign(ostr.str());
}

// ============================================================================
void Spake2::computeTranscriptHash()
{
  transcript_hash.assign(cipher_suite.getHashFunction()(transcript));

  /// Ke || Ka = Hash(TT), where |Ke| == |Ka|
  splitHexStringInHalf(transcript_hash, 
                     symmetric_secrets.Ke, 
                     symmetric_secrets.Ka);

  assert(symmetric_secrets.Ke.length() == symmetric_secrets.Ka.length());

  /// Keys MUST be at least 128 bits in length.
  assert(symmetric_secrets.Ke.length() >= 128u / BITS_PER_BYTE);
}

// ============================================================================
void Spake2::computeSharedSymmetricSecrets()
{ 
  /// Both parties use Ka to derive shared symmetric secrets.
  std::vector<unsigned char> Kc = hexStringToBytes(symmetric_secrets.Ka);
  const std::string keys = 
    cipher_suite.getKeyDerivationFunction()(
    std::string(reinterpret_cast<const char*>(Kc.data()), Kc.size()), "ConfirmationKeys", 
    addl_auth_data);

  /// Split the KDF's output in half.
  splitHexStringInHalf(keys, mac_keys.KcA, mac_keys.KcB);

  assert(mac_keys.KcA.length() == mac_keys.KcB.length());

  /// Keys MUST be at least 128 bits in length.
  assert(mac_keys.KcA.length() >= 128u / BITS_PER_BYTE);
  assert(mac_keys.KcB.length() >= 128u / BITS_PER_BYTE);
}

// ============================================================================
void Spake2::computeKeyConfirmationMessage()
{
  confirmation_key.assign(HEX_PREFIX_LOWERCASE + cipher_suite.getMacFunction()(
    ( mode == Mode::CLIENT ) ? mac_keys.KcA : mac_keys.KcB, transcript));

  /// Precompute what we expect the other's confirmation key to be.
  expected_key.    assign(HEX_PREFIX_LOWERCASE + cipher_suite.getMacFunction()( 
    ( mode == Mode::CLIENT ) ? mac_keys.KcB : mac_keys.KcA, transcript));
}

// ============================================================================
bool Spake2::checkProtocolComplete() const
{
  bool success = !expected_key.empty() &&
                  expected_key == other_party_confirmation_key;

  if ( success )
  {
    cout << "SPAKE2 protocol passes. Both keys match." << endl;
  }
  else
  {
    std::cerr << "SPAKE2 failure. Confirmation keys do not match."      << endl;
    std::cerr << "Expected key  : \n  " << expected_key                 << endl;
    std::cerr << "Actual key    : \n  " << other_party_confirmation_key << endl;
  }
  return success;
}
