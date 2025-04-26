# SPAKE2

This project implements the SPAKE2 protocol, a Password Authenticated Key Exchange (PAKE), which allows two parties with a shared password to derive a strong shared key without disclosing the password. Specifically, it implements the protocol as described in RFC 9382[[1]](#1). Currently, this implementation uses ciphersuite PAKE2-P256-SHA256-HKDF-HMAC. It's written in pure C++11. 

## Requirements
```
  - Linux (Tested with gcc 11.4.0 on Ubuntu 22.04).

  - The following prerequisites are required:
  sudo apt install build-essential curl m4

  - The following libraries are required:
  _______________________________________________________________________
  | Library    | Version | Justification                                |
  -----------------------------------------------------------------------
  | libgmp     | v6.3.0  | Large-integer Elliptic Curve arithmetic      |
  | openssl    | v3.3.0  | SHA-256, MAC, and KDF                        |
  | libsodium  | v1.0.19 | Memory-Hard-Function for deriving w          |
  | lcov       | v1.16   | Optional.  Only used for code coverage tests |
  -----------------------------------------------------------------------
  The above libraries should be installed via ./install_dependencies_local.sh
```
## How to Build

The required libraries must be installed locally first:
```bash
  # Only have to do this once. Give it a few minutes.
  chmod +x install_dependencies_local.sh
  ./install_dependencies_local.sh 
```

After building the libraries, build SPAKE2:
```bash
  # If it exists
  rm -rf build
  mkdir build ; cd build
  cmake -DCMAKE_BUILD_TYPE=Release ..
  make
```

## Usage
```
./spake2 -pw <password> [OPTIONS]

Options:
  -pw <password>            Required. The shared password between parties. Must
                            be identical for both parties using SPAKE2.
  -s, -server               Run in server mode. If not specified, runs in client 
                            mode. One instance of SPAKE2 must run in server mode.
  -i, -identity <identity>  Optional. Set the identity string for this party 
                            using the SPAKE2 protocol. Empty by default.
  -aad <data>               Optional. Provide additional authentication data for
                            key derivation. If specified, both parties must use 
                            the same value.
Examples:
./spake2 -s -pw foo 
      Runs SPAKE2 in server mode, with the password "foo".

./spake2 -i alice -pw bar
      Runs SPAKE2 in client mode with identity "alice", with the password "bar".

./spake2 -s -i server -aad foo -pw bar
      Runs SPAKE2 using identity "server" and shared AAD "foo", with the password "bar".
      
Notes:
  - The pw value must be identical for both parties exercising SPAKE2.
  - If the -aad option is used, the value must match exactly on both client and server.
  - Identity is optional, but can be useful to distinguish parties during key exchange.
```

## Sample Usage

  Two instances of spake2 are required to fully exercise the protocol. One instance, we'll call 'A', operates as the client, while the other instance, 'B' operates as the server. As per the spake2 specification, it is assumed A and B have already agreed on a shared password before invoking the protocol. Both instances are required to be executing in the same directory. For this example, we assume the shared password between A and B is "foo". We also assume A's identity is "alice", and B's is "bob".

  A typical spake2 flow looks like:

  1) In Terminal 1, start the client instance, 'A'.
  ```bash
  ./spake2 -i alice -pw foo 
  ```

  2) In Terminal 2, start the server instance, 'B'.
  ```bash
  ./spake2 -i bob -s -pw foo 
  ```

  Running spake2 automatically invokes the "Setup Phase", which computes the Public Key and writes them to a file.

  3) Both instances should now be indicating "Public Key successfully written to file", and to execute the setup phase in another instance, and to enter a key to continue. Enter any key in both terminals to continue.

  Both instances will now execute the Key Derivation Phase.

  4) Both instances should now be indicating "Key derivation phase complete". Enter any key in both terminals to continue.

  5) Both instances will now try and validate each other's keys. If the above steps were done correctly, both terminals should be indicating "protocol validated" to show that the other instance's key was successfully validated and matched. This concludes the protocol.

## Running Tests

### Google Test

Google Tests are available for most of the helper functions used. Additionally, the test vectors in [[1]](#1) are exercised and validated using Google Test. Tests for Elliptic Curve arithmetic are also provided.
```bash
  # If it exists
  rm -rf build
  mkdir build ; cd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON  ..
  make
  ./tests/spake2_tests
```
### Google Test With Code Coverage

Additionally, LCOV coverage reports are available to be generated for the above tests if desired. LCOV is required to be installed to use.

```bash
  # If it exists
  rm -rf build
  mkdir build ; cd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DTEST_COVERAGE ..
  make spake2_tests_coverage
  # Open coverage test results in your favorite browser.
  firefox spake2_tests_coverage/index.html
```

## Known Limitations
- While it would have been nice to implement the hash_to_curve() given in the original paper[[1]](#1), the values of M and N are currently limited to those given by [[2]](#2) for curve P-256.
- Currently, only curve P-256 is supported. Curve parameters were obtained via [[3]](#3).

## Contributions/References

<a id="1">[1]</a> 
The original paper :
https://www.rfc-editor.org/rfc/rfc9382.html

<a id="2">[2]</a> 
The values for M and N were taken from:
https://github.com/jiep/spake2plus/blob/main/spake2plus/ciphersuites/ciphersuites.py#L14

<a id="3">[3]</a> 
Curve P-256 parameters https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-186.pdf

