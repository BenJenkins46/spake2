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

#include "KeyDerivationFunctions.hpp"

#include <vector>

#include "StringHelpers.hpp"
#include <openssl/evp.h>
#include <openssl/kdf.h>

/** A Key Derivation function which takes the following parameters.
    @param data - The data to derive a key from.
    @param info - An additional string to pass into the Key Derivation Function.
    In the context of SPAKE2, this value is hard-coded to "ConfirmationKeys".
    @param aad  - Additional associated data which may be shared by each 
    endpoint. If it is shared, it must be identical between both parties.
    @return The KDF of data, with additional info with aad appended.
    @cite https://docs.openssl.org/3.2/man3/EVP_PKEY_CTX_new/
*/
KeyDerivationFunction
HKDF_RFC5869 = [](const std::string& data, 
                  const std::string& info_in, 
                  const std::string& aad) -> std::string
{
  constexpr std::size_t output_len = 32;
  std::vector<unsigned char> out_key(output_len);

  std::string info = info_in;
  info.append(aad);

  EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, nullptr);

  if ( pctx == nullptr )
  {
    throw std::runtime_error("EVP_PKEY_CTX_new_id failed.");
  }

  if (EVP_PKEY_derive_init(pctx)                    <= 0 ||
      EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256())  <= 0 ||
      EVP_PKEY_CTX_set1_hkdf_salt(pctx, nullptr, 0) <= 0 ||
      EVP_PKEY_CTX_set1_hkdf_key(pctx, 
        reinterpret_cast<const unsigned char*>(
          data.data()), data.size()) <= 0                ||
      EVP_PKEY_CTX_add1_hkdf_info(pctx, 
        reinterpret_cast<const unsigned char*>(
          info.data()), info.size()) <= 0)
  {
    EVP_PKEY_CTX_free(pctx);
    throw std::runtime_error("HKDF parameter setup failed.");
  }

  std::size_t len = output_len;
  if ( EVP_PKEY_derive(pctx, out_key.data(), &len) <= 0 )
  {
    EVP_PKEY_CTX_free(pctx);
    throw std::runtime_error("HKDF derivation failed.");
  }

  EVP_PKEY_CTX_free(pctx);
  if ( len != output_len ) 
  {
    throw std::runtime_error("Incorrect output length.");
  }
   
  return binaryToHexString(out_key.data(), out_key.size());
};