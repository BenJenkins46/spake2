#include "Spake2Version.hpp"

#include <iostream>

#include "Spake2.hpp"

/** Display the usage of SPAKE2 and exit.
    @param exec_name The name of the executable calling main().
 */
[[noreturn]] void displayUsage(const std::string& exec_name);

int main(int argc, char* argv[])
{
  std::cout << "SPAKE2 v" 
            << SPAKE_2_MAJOR_VERSION << "." << SPAKE_2_MINOR_VERSION 
            << std::endl;

  /// Enable client mode by default.
  bool client_mode                          = true;

  /// The SPAKE2 spec allows for empty identities.
  std::string identity                      = "";

  /// Parties may share additional authenticated data for the Key Derivation
  std::string additional_authenticated_data = "";
  
  /// The shared password between both parties. 
  std::string shared_password               = "";

  for ( int arg = 1; arg < argc; ++arg )
  {
    const std::string argument = argv[arg];
    if ( argument == "-s" || argument == "-server" )
    {
      client_mode = false;
    } 
    else if ( ( argument == "-i" || argument == "-identity" ) && 
              ( arg + 1 < argc ) )
    {
      identity = argv[++arg];
    }
    else if ( ( argument == "-aad" ) && ( arg + 1 < argc ) )
    {
      additional_authenticated_data = argv[++arg];
    }
    else if ( ( argument == "-pw" ) && ( arg + 1 < argc ) )
    {
      shared_password = argv[++arg];
    }
    else if ( ( argument == "-h" ) || ( argument == "-help" ) )
    {
      displayUsage(argv[0]);
    }
  }
  
  if ( shared_password.empty() )
  {
    displayUsage(argv[0]);
  }
  
  Spake2 spake2(identity, 
                shared_password, 
                client_mode, 
                additional_authenticated_data);

  /// The private key and password should be set before setupPhase().
  spake2.setupPhase();

  /// Prompt user to execute other SPAKE2 setup phase.
  if ( spake2.readOtherPartiesPublicKey() )
  {
    /// Derive the keys, then wait for other instance to do the same.
    spake2.keyDerivationPhase();

    /// Prompt user to execute other SPAKE2 key derivation phase.
    if ( spake2.readOtherPartiesConfirmationKey() )
    {
      /// Check if shared key matches other instances, indicating success.
      spake2.checkProtocolComplete();
    }
  }
}

// =============================================================================
void displayUsage(const std::string& exec_name)
{
  std::cout << R"(Usage: )" << exec_name << R"( -pw <password> [OPTIONS]

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
)" << exec_name << R"( -s -pw foo 
      Runs SPAKE2 in server mode, with the password "foo".

)" << exec_name << R"( -i alice -pw bar
      Runs SPAKE2 in client mode with identity "alice", with the password "bar".

)" << exec_name << R"( -s -i server -aad foo -pw bar
      Runs SPAKE2 using identity "server" and shared AAD "foo", with the password "bar".
      
Notes:
  - The pw value must be identical for both parties exercising SPAKE2.
  - If the -aad option is used, the value must match exactly on both client and server.
  - Identity is optional, but can be useful to distinguish parties during key exchange.
)" << std::endl;

  std::exit(EXIT_FAILURE);
}