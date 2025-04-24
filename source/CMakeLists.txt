set(LIB_NAME spake2_core)

include_directories(${EXTERN_DIR}/openssl/include ${EXTERN_DIR}/sodium/include)

set(LIB_SPAKE_2_SRC
    EllipticCurve.hpp                      EllipticCurve.cpp
    HashFunctions.hpp                      HashFunctions.cpp
    KeyDerivationFunctions.hpp             KeyDerivationFunctions.cpp
    MessageAuthenticationCodeFunctions.hpp MessageAuthenticationCodeFunctions.cpp
    Spake2.hpp                             Spake2.cpp
    Spake2CipherSuite.hpp                  Spake2CipherSuite.cpp)

add_library(${LIB_NAME} ${LIB_SPAKE_2_SRC})

target_link_libraries(${LIB_NAME} PUBLIC ${EXTERN_DIR}/gmp/lib/libgmp.a
  ${EXTERN_DIR}/sodium/lib/libsodium.a
  ${EXTERN_DIR}/openssl/lib64/libssl.a
  ${EXTERN_DIR}/openssl/lib64/libcrypto.a)
