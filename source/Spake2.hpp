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

#ifndef SPAKE_2_HPP
#define SPAKE_2_HPP

#include <string>

#include "EllipticCurveConstants.hpp"
#include "HashFunctions.hpp"
#include "Spake2CipherSuite.hpp"
#include "Spake2Constants.hpp"

#include <gmp.h>

/** This class executes the SPAKE2 protocol - a Password Authenticated Key 
    Exchange (PAKE) protocol. Two instances of this class are required to fully
    exercise the protocol - one as the client, and the other as the server. It
    is assumed that both parties have already shared their shared secret before
    invoking this protocol. This class operates in three phases: 
      - setupPhase() : Derives the public key and writes to a file for the other
      instance to read. Should then call readOtherPartiesPublicKey().
      - keyDerivationPhase() : Use the other instance's public key to derive 
      shared secrets, and write to file for the other instance. Should then
      call readOtherPartiesConfirmationKey().
      - checkProtocolComplete() : Read the other instances shared secrets, and
      compare against current instances. If they match, the protocol is 
      considered valid and complete.
    The above assumes both instances are running in separate programs (but must
    operate in the same directory). Alternatively, if both instances run in the
    same program, readOtherPartiesPublicKey() can be substituted with 
    putPublicKeyOther() and readOtherPartiesConfirmationKey() can be substituted
    with putConfirmationKeyOther(). This skips the need to prompt for user input
    to ensure the other party has performed their appropriate stage.
    Currently, this class is limited to the PAKE2-P256-SHA256-HKDF-HMAC
    ciphersuite. Future iterations could benefit for a wider selection.

    Motivation/Source : https://www.rfc-editor.org/rfc/rfc9382.html
 */
class Spake2
{
public:

  /** Construct a new Spake2 instance.
      @param identity_in The identity for this SPAKE2 instance. Typically,
      something like 'server', 'client', 'alice', 'bob'. May be empty.
      @param password_in The shared password between both instances. This is
      passed into a Memory Hard Function (MHF) to compute w. It MUST be
      identical for both instances otherwise the protocol will not validate.
      @param client Is this SPAKE2 instance operating as the client or server?
      If true, will operate as the client. NOTE, one instance of SPAKE2 must 
      be running as the server.
      @param curve_name The elliptic curve to use. Defaults to P-256.
      @param hash_function The hash function to use. Defaults to SHA256.
      @param key_derivation_function The key derivation function (KDF) to use.
      Defaults to HKDF.
      @param mac_function The message authentication code function (MAC) to use.
      Defaults to HMAC.
   */
  Spake2(const std::string&                 identity_in, 
         const std::string&                 password_in,
         bool                               client,
         const std::string&                 addl_auth_data          = "",
         Curves                             curve                   = Curves::P256,
         HashFunctions                      hash_function           = HashFunctions::SHA256,
         KeyDerivationFunctions             key_derivation_function = KeyDerivationFunctions::HKDF,
         MessageAuthenticationCodeFunctions mac_function            = MessageAuthenticationCodeFunctions::HMAC);

  /// @brief The destructor clears memory allocated by mpz_inits().
  ~Spake2();

  /// @brief The MAC keys are expressed as KcA || KcB
  struct MacKeys
  {
    std::string KcA;
    std::string KcB;
  };

  /// @brief The symmetric secrets are expressed as Ke || Ka
  struct SymmetricSecrets
  {
    std::string Ka;
    std::string Ke;
  };

  /** Accessor for this instance's identity.
      @return Const-reference to this instance's identity.
  */
  const std::string& getIdentity() const;

  /** Accessor for this instance's mode of operation. 
      An instance can either operate in client or server mode.
      @return Const-reference to a string representation of the mode of operation.
  */
  const std::string& getMode() const;

  /** Accessor for the other instance's mode of operation. Needed for knowing
      the expected filename to read.
      An instance can either operate in client or server mode.
      @return Const-reference to a string representation of the other
      party's assumed mode of operation.
  */
  const std::string& getOtherMode() const;

  /** Accessor for the public key (pA/pB). 
      The public key is generated within setupPhase().
      Should not be called until after setupPhase().
      @return Const-reference to the public key.
  */
  const EllipticCurve::Point& getPublicKey() const;
  
  /** Accessor for the uncompressed public key (pA/pB). 
      Should not be called until after setupPhase().
      @return Const-reference to the public key.
  */
  std::string getUncompressedPublicKey() const;

  /** Accessor for the uncompressed group element, K.
      Should not be called until after setupPhase().
      @return Const-reference to the group element.
  */
  std::string getUncompressedGroupElement() const;

  /** Accessor for the transcript, TT. 
      @return Const-reference to the transcript.
  */
  const std::string& getTranscript() const;

  /** Accessor for the Hash of the transcript, Hash(TT).
      @return Const-reference to the hash of the transcript.
  */
  const std::string& getTranscriptHash() const;

  /** Accessor for the shared symmetric secrets, Ke and Ka.
      @return Const-reference to the shared symmetric secrets.
  */
  const SymmetricSecrets& getSharedSymmetricSecrets() const;

  /** Accessor for the MAC Keys, KcA and KcB.
      @return Const-reference to the MAC Keys.
  */
  const MacKeys& getMacKeys() const;

  /** Accessor for the MAC Keys, KcA and KcB.
     @return Const-reference to the MAC Keys.
  */
  const std::string& getConfirmationKey() const;

#if defined CMAKE_TESTING_ENABLED
  /** Mutator for the private key. Should only be used in testing environments.
      @param key The key, in string format.
   */
  void putPrivateKey(const std::string& key);

  /** Mutator for the password, w. Should only be used in testing environments.
      @param key The key, in string format.
   */
  void putPassword(const std::string& key);
#endif

  /** Execute the setup phase for Spake2. This is considered to be the first
      step in the Spake2 protocl. The setup phase consists of computing public 
      keys, pA and pB, and transmitting to the other party.
   */
  void setupPhase();

  /** Read the other party's public key, pA or pB.
      This should only be called after this instance has executed setupPhase()!
      @return True if the other party's public key was successfully read, and
      the key derivation phase may proceed.
   */
  bool readOtherPartiesPublicKey();

  /** Read the other party's confirmation key, A conf / B conf.
      This should only be called after this instance has executed setupPhase()!
      @return True if the other party's public key was successfully read, and
      the key derivation phase may proceed.
   */
  bool readOtherPartiesConfirmationKey();

  /** Execute the second phase of Spake2, the key derivation phase. This phase
      relies on having had received the other party's public key, pA or pB.
   */
  void keyDerivationPhase();

  /// Compute the group element, K. Relies on other party's public key.
  void computeGroupElement();

  /** Mutator for the public key of another SPAKE2 instance. This allows for
      multiple SPAKE2 instances to run in the same process, circumventing the
      need to block reading a file. If used, should be called before this 
      instance calls keyDerivationPhase().
      @param identity_other The identity of the other SPAKE2 instance.
      @param public_key_other The public key of the other SPAKE2 instance.
  */
  void putPublicKeyOther(const std::string&          identity_other,
                         const EllipticCurve::Point& public_key_other);

  /** Mutator for the confirmation key of another SPAKE2 instance. This allows 
      for multiple SPAKE2 instances to run in the same process, circumventing 
      the need to block reading a file. If used, should be called after this 
      instance calls keyDerivationPhase(), but before this instance calls 
      checkProtocolComplete().
      @param confirmation_key_other The identity of the other SPAKE2 instance.
  */
  void putConfirmationKeyOther(const std::string& confirmation_key_other);

  /** Validate the computed confirmation key equals the given confirmation key
      from the other SPAKE2 instance. This is the final stage in the SPAKE2
      protocol, and should only be called after keyDerivationPhase().
      @return True if the confirmation keys match, and the protocol can be
      considered complete.
   */
  bool checkProtocolComplete() const;

protected:
private:

  /// @brief The Spake2 instance name. Typically, 'client', or 'server'.
  const std::string       identity;

  /// @brief This instances' mode of operation. Either CLIENT or SERVER.
  Mode                    mode;
  
  const Spake2CipherSuite cipher_suite;

  /// @brief A hash of the password, pw.
  mpz_t w;

  /// @brief Private Key.
  mpz_t xy;

  /// @brief Should be in [0, p)
  mpz_t k_pri;
  
  /// @brief Publically-accessible public key. pA/pB
  EllipticCurve::Point k_pub;

  /// @brief Publically-accessible group element. Ka/Kb
  EllipticCurve::Point K;

  /// @brief The transcript, TT
  std::string transcript;

  /// @brief The hash of the transcript, Hash(TT).
  std::string transcript_hash;

  /// @brief Shared symmetric secrets.
  SymmetricSecrets symmetric_secrets;

  /// @brief Key used for MAC confirmation.
  MacKeys mac_keys;

  /// @brief Optional AAD to be used in the Key Derivation Function
  std::string          addl_auth_data;

  /// @brief The confirmation key, cA/cB
  std::string          confirmation_key;

  /// @brief The expected confirmation key from the other party.
  std::string          expected_key;

  /// @brief Components of the other party's key derivation.
  std::string          other_party_identity;
  EllipticCurve::Point other_party_public_key;
  std::string          other_party_confirmation_key;

  /** Compute the shared integer, w, using a Memory Hard Function to prevent
      brute-force attacks. The currently chosen Memory Hard Function is the 
      default libsodium algorithm, crypto_pwhash_ALG_DEFAULT. A constant salt
      value is used for reproducability.
      Source : https://libsodium.gitbook.io/doc/password_hashing/default_phf
      @param pw The shared password between A and B to derive w from.
  */
  void computeW(const std::string& pw);

  /// Compute the public key, pA / pB, using w, M/N and X/Y. Done in setup phase.
  void computePublicKey();

  /** Compute the transcript, TT. The transcript is defined as
      TT = len(A)  || A
        || len(B)  || B
        || len(pA) || pA
        || len(pB) || pB
        || len(K)  || K
        || len(w)  || w
  */
  void computeTranscript();

  /// Compute the hash of the transcript, Hash(TT)
  void computeTranscriptHash();

  /** Use TT to derive shared symmetric secrets Ke and Ka as Ke || Ka = Hash
      (TT), with |Ke| = |Ka|. The length of each key is equal to half of the 
      digest output, e.g., 128 bits for SHA-256. Keys MUST be at least 128 bits 
      in length.
  */
  void computeSharedSymmetricSecrets();

  /** Compute the key confirmation message and the expected confirmation key
      from the other party.
  */
  void computeKeyConfirmationMessage();

  /// Transmits the public key using a file. Done during setup phase.
  void transmitPublicKey() const;

  /** Transmits the confirmation key using a file. Done at the end of the key 
      derivation phase.
  */
  void transmitConfirmationKey() const;

  /// Both copy assignment and copy constructors are deleted.
  Spake2 operator=(const Spake2& object) = delete;
  Spake2          (const Spake2& object) = delete;
};

// ============================================================================
inline const std::string& Spake2::getIdentity() const
{
  return identity;
}

// ============================================================================
inline const std::string& Spake2::getMode() const
{
  const static std::string modes[Mode::NUM_MODES] = {"client", "server"};

  return modes[mode];
}

// ============================================================================
inline const std::string& Spake2::getOtherMode() const
{
  const static std::string modes[Mode::NUM_MODES] = {"client", "server"};

  return modes[( mode == Mode::CLIENT ) ? Mode::SERVER : Mode::CLIENT];
}

#if defined CMAKE_TESTING_ENABLED
// ============================================================================
inline void Spake2::putPrivateKey(const std::string& key)
{
  mpz_init_set_str(k_pri, key.c_str(), 0);
}

// ============================================================================
inline void Spake2::putPassword(const std::string& key)
{
  mpz_init_set_str(w, key.c_str(), 0);
}
#endif

// ============================================================================
inline const EllipticCurve::Point& Spake2::getPublicKey() const
{
  return k_pub;
}

// ============================================================================
inline std::string Spake2::getUncompressedPublicKey() const
{
  return 
    k_pub.getUncompressedFormat(cipher_suite.getCurve().getFieldSizeBytes());
}

// ============================================================================
inline std::string Spake2::getUncompressedGroupElement() const
{
  return 
    K.getUncompressedFormat(cipher_suite.getCurve().getFieldSizeBytes());
}

// ============================================================================
inline const std::string& Spake2::getTranscript() const
{
  return transcript;
}

// ============================================================================
inline const std::string& Spake2::getTranscriptHash() const
{
  return transcript_hash;
}

// ============================================================================
inline const Spake2::MacKeys& Spake2::getMacKeys() const
{
  return mac_keys;
}

// ============================================================================
inline const std::string& Spake2::getConfirmationKey() const
{
  return confirmation_key;
}

// ============================================================================
inline 
void Spake2::putPublicKeyOther(const std::string&          identity_other,
                               const EllipticCurve::Point& public_key_other)
{
  other_party_identity   = identity_other;
  other_party_public_key = public_key_other;
}

// ============================================================================
inline void 
Spake2::putConfirmationKeyOther(const std::string& confirmation_key_other)
{
  other_party_confirmation_key.assign(confirmation_key_other);
}

// ============================================================================
inline 
const Spake2::SymmetricSecrets& Spake2::getSharedSymmetricSecrets() const
{
  return symmetric_secrets;
}

// ============================================================================
inline void Spake2::setupPhase()
{
  computePublicKey();
  transmitPublicKey();
}

// ============================================================================
inline void Spake2::keyDerivationPhase()
{
  /// Both A and B calculate the group element, K.
  computeGroupElement();

  /// Both A and B calculate the transcript, TT.
  computeTranscript();

  /// Both A and B compute the hash of the transcript, Hash(TT) = Ke || Ka
  computeTranscriptHash();

  /** Both A and B compute MAC keys for key confirmation messages 
      KDF(Ka, "ConfirmationKeys" || AADS, null) = KcA || KcB
  */
  computeSharedSymmetricSecrets();

  /// Both A and B compute their respective confirmation keys, A conf / B conf.
  computeKeyConfirmationMessage();

  /// Write the confirmation key, A conf / B conf to file.
  transmitConfirmationKey();
}

// ============================================================================
inline void Spake2::computePublicKey()
{
  /// {X/Y} = {x/y}P;
  const EllipticCurve::Point X_or_Y = 
    cipher_suite.getCurve().
      scalarMultiplication(k_pri, cipher_suite.getCurve().getGenerator());

  /// w{M/N}
  const EllipticCurve::Point wM_or_N = 
  cipher_suite.getCurve().scalarMultiplication(w, 
      ( mode == Mode::CLIENT ) ? cipher_suite.getM() : cipher_suite.getN());
  
  /// p{A/B} = w*{M/N} + X/Y
  k_pub = cipher_suite.getCurve().operate(X_or_Y, wM_or_N);
}

#endif
