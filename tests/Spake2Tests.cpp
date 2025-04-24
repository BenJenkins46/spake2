#include <gtest/gtest.h>
#include <gmp.h>

#include "Spake2Tests.hpp"

const Spake2Tests::GivenValue Spake2Tests::given_values[num_test_vectors] =
{
  // A='server', B='client'
  {
    "server",
    "client",
    "0x2ee57912099d31560b3a44b1184b9b4866e904c49d12ac5042c97dca461b1a5f",
    "0x43dd0fd7215bdcb482879fca3220c6a968e66d70b1356cac18bb26c84a78d729",
    "0xdcb60106f276b02606d8ef0a328c02e4b629f84f89786af5befb0bc75b6e66be"
  }, 
  // A='', B='client'
  {
    "",
    "client",
    "0x0548d8729f730589e579b0475a582c1608138ddf7054b73b5381c7e883e2efae",
    "0x403abbe3b1b4b9ba17e3032849759d723939a27a27b9d921c500edde18ed654b",
    "0x903023b6598908936ea7c929bd761af6039577a9c3f9581064187c3049d87065"
  }, 
  // A='server', B=''
  {
    "server",
    "",
    "0x626e0cdc7b14c9db3e52a0b1b3a768c98e37852d5db30febe0497b14eae8c254",
    "0x07adb3db6bc623d3399726bfdbfd3d15a58ea776ab8a308b00392621291f9633",
    "0xb6a4fc8dbb629d4ba51d6f91ed1532cf87adec98f25dd153a75accafafedec16"
  }, 
  // A='', B=''
  {
    "",
    "",
    "0x7bf46c454b4c1b25799527d896508afd5fc62ef4ec59db1efb49113063d70cca",
    "0x8cef65df64bb2d0f83540c53632de911b5b24b3eab6cc74a97609fd659e95473",
    "0xd7a66f64074a84652d8d623a92e20c9675c61cb5b4f6a0063e4648a2fdc02d53"
  } 
};

const Spake2Tests::TestVector Spake2Tests::expected_values[num_test_vectors] = 
{
  {
    "0x04a56fa807caaa53a4d28dbb9853b9815c61a411118a6fe516a8798434751470f9010153ac33d0d5f2047ffdb1a3e42c9b4e6be662766e1eeb4116988ede5f912c",
    "0x0406557e482bd03097ad0cbaa5df82115460d951e3451962f1eaf4367a420676d09857ccbc522686c83d1852abfa8ed6e4a1155cf8f1543ceca528afb591a1e0b7",
    "0x0412af7e89717850671913e6b469ace67bd90a4df8ce45c2af19010175e37eed69f75897996d539356e2fa6a406d528501f907e04d97515fbe83db277b715d3325",
    "0x06000000000000007365727665720600000000000000636c69656e74410000000000000004a56fa807caaa53a4d28dbb9853b9815c61a411118a6fe516a8798434751470f9010153ac33d0d5f2047ffdb1a3e42c9b4e6be662766e1eeb4116988ede5f912c41000000000000000406557e482bd03097ad0cbaa5df82115460d951e3451962f1eaf4367a420676d09857ccbc522686c83d1852abfa8ed6e4a1155cf8f1543ceca528afb591a1e0b741000000000000000412af7e89717850671913e6b469ace67bd90a4df8ce45c2af19010175e37eed69f75897996d539356e2fa6a406d528501f907e04d97515fbe83db277b715d332520000000000000002ee57912099d31560b3a44b1184b9b4866e904c49d12ac5042c97dca461b1a5f",
    "0x0e0672dc86f8e45565d338b0540abe6915bdf72e2b35b5c9e5663168e960a91b",
    "0x0e0672dc86f8e45565d338b0540abe69",
    "0x15bdf72e2b35b5c9e5663168e960a91b",
    "0x00c12546835755c86d8c0db7851ae86f",
    "0xa9fa3406c3b781b93d804485430ca27a",
    "0x58ad4aa88e0b60d5061eb6b5dd93e80d9c4f00d127c65b3b35b1b5281fee38f0",
    "0xd3e2e547f1ae04f2dbdbf0fc4b79f8ecff2dff314b5d32fe9fcef2fb26dc459b"
  }, // A='server', B='client'
  {
    "0x04a897b769e681c62ac1c2357319a3d363f610839c4477720d24cbe32f5fd85f44fb92ba966578c1b712be6962498834078262caa5b441ecfa9d4a9485720e918a",
    "0x04e0f816fd1c35e22065d5556215c097e799390d16661c386e0ecc84593974a61b881a8c82327687d0501862970c64565560cb5671f696048050ca66ca5f8cc7fc",
    "0x048f83ec9f6e4f87cc6f9dc740bdc2769725f923364f01c84148c049a39a735ebda82eac03e00112fd6a5710682767cff5361f7e819e53d8d3c3a2922e0d837aa6",
    "0x00000000000000000600000000000000636c69656e74410000000000000004a897b769e681c62ac1c2357319a3d363f610839c4477720d24cbe32f5fd85f44fb92ba966578c1b712be6962498834078262caa5b441ecfa9d4a9485720e918a410000000000000004e0f816fd1c35e22065d5556215c097e799390d16661c386e0ecc84593974a61b881a8c82327687d0501862970c64565560cb5671f696048050ca66ca5f8cc7fc4100000000000000048f83ec9f6e4f87cc6f9dc740bdc2769725f923364f01c84148c049a39a735ebda82eac03e00112fd6a5710682767cff5361f7e819e53d8d3c3a2922e0d837aa620000000000000000548d8729f730589e579b0475a582c1608138ddf7054b73b5381c7e883e2efae",
    "0x642f05c473c2cd79909f9a841e2f30a70bf89b18180af97353ba198789c2b963",
    "0x642f05c473c2cd79909f9a841e2f30a7",
    "0x0bf89b18180af97353ba198789c2b963",
    "0xc6be376fc7cd1301fd0a13adf3e7bffd",
    "0xb7243f4ae60440a49b3f8cab3c1fba07",
    "0x47d29e6666af1b7dd450d571233085d7a9866e4d49d2645e2df975489521232b",
    "0x3313c5cefc361d27fb16847a91c2a73b766ffa90a4839122a9b70a2f6bd1d6df"
  }, // A='', B='client'
  {
    "0x04f88fb71c99bfffaea370966b7eb99cd4be0ff1a7d335caac4211c4afd855e2e15a873b298503ad8ba1d9cbb9a392d2ba309b48bfd7879aefd0f2cea6009763b0",
    "0x040c269d6be017dccb15182ac6bfcd9e2a14de019dd587eaf4bdfd353f031101e7cca177f8eb362a6e83e7d5e729c0732e1b528879c086f39ba0f31a9661bd34db",
    "0x0445ee233b8ecb51ebd6e7da3f307e88a1616bae2166121221fdc0dadb986afaf3ec8a988dc9c626fa3b99f58a7ca7c9b844bb3e8dd9554aafc5b53813504c1cbe",
    "0x06000000000000007365727665720000000000000000410000000000000004f88fb71c99bfffaea370966b7eb99cd4be0ff1a7d335caac4211c4afd855e2e15a873b298503ad8ba1d9cbb9a392d2ba309b48bfd7879aefd0f2cea6009763b04100000000000000040c269d6be017dccb15182ac6bfcd9e2a14de019dd587eaf4bdfd353f031101e7cca177f8eb362a6e83e7d5e729c0732e1b528879c086f39ba0f31a9661bd34db41000000000000000445ee233b8ecb51ebd6e7da3f307e88a1616bae2166121221fdc0dadb986afaf3ec8a988dc9c626fa3b99f58a7ca7c9b844bb3e8dd9554aafc5b53813504c1cbe2000000000000000626e0cdc7b14c9db3e52a0b1b3a768c98e37852d5db30febe0497b14eae8c254",
    "0x005184ff460da2ce59062c87733c299c3521297d736598fc0a1127600efa1afb",
    "0x005184ff460da2ce59062c87733c299c",
    "0x3521297d736598fc0a1127600efa1afb",
    "0xf3da53604f0aeecea5a33be7bddf6edf",
    "0x9e3f86848736f159bd92b6e107ec6799",
    "0xbc9f9bbe99f26d0b2260e6456e05a86196a3307ec6663a18bf6ac825736533b2",
    "0xc2370e1bf813b086dff0d834e74425a06e6390f48f5411900276dcccc5a297ec"
  }, // A='server', B=''
  {
    "0x04a65b367a3f613cf9f0654b1b28a1e3a8a40387956c8ba6063e8658563890f46ca1ef6a676598889fc28de2950ab8120b79a5ef1ea4c9f44bc98f585634b46d66",
    "0x04589f13218822710d98d8b2123a079041052d9941b9cf88c6617ddb2fcc0494662eea8ba6b64692dc318250030c6af045cb738bc81ba35b043c3dcb46adf6f58d",
    "0x041a3c03d51b452537ca2a1fea6110353c6d5ed483c4f0f86f4492ca3f378d40a994b4477f93c64d928edbbcd3e85a7c709b7ea73ee97986ce3d1438e135543772",
    "0x00000000000000000000000000000000410000000000000004a65b367a3f613cf9f0654b1b28a1e3a8a40387956c8ba6063e8658563890f46ca1ef6a676598889fc28de2950ab8120b79a5ef1ea4c9f44bc98f585634b46d66410000000000000004589f13218822710d98d8b2123a079041052d9941b9cf88c6617ddb2fcc0494662eea8ba6b64692dc318250030c6af045cb738bc81ba35b043c3dcb46adf6f58d4100000000000000041a3c03d51b452537ca2a1fea6110353c6d5ed483c4f0f86f4492ca3f378d40a994b4477f93c64d928edbbcd3e85a7c709b7ea73ee97986ce3d1438e13554377220000000000000007bf46c454b4c1b25799527d896508afd5fc62ef4ec59db1efb49113063d70cca",
    "0xfc6374762ba5cf11f4b2caa08b2cd1b9907ae0e26e8d6234318d91583cd74c86",
    "0xfc6374762ba5cf11f4b2caa08b2cd1b9",
    "0x907ae0e26e8d6234318d91583cd74c86",
    "0x5dbd2f477166b7fb6d61febbd77a5563",
    "0x7689b4654407a5faeffdc8f18359d8a3",
    "0xdfb4db8d48ae5a675963ea5e6c19d98d4ea028d8e898dad96ea19a80ade95dca",
    "0xd0f0609d1613138d354f7e95f19fb556bf52d751947241e8c7118df5ef0ae175"    
  } // A='', B=''
};

Spake2* Spake2Tests::alice[num_test_vectors];
Spake2* Spake2Tests::bob  [num_test_vectors];

// ============================================================================
void Spake2Tests::SetUpTestSuite()
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /** These tests usurp the "foo" password by calling putPrivateKey/
        putPassword
    */
    alice[i] = new Spake2(given_values[i].A_name, "foo", true);
    bob  [i] = new Spake2(given_values[i].B_name, "foo", false);

    alice[i]->putPrivateKey(given_values[i].x);
    alice[i]->putPassword  (given_values[i].w);
    bob  [i]->putPrivateKey(given_values[i].y);
    bob  [i]->putPassword  (given_values[i].w);
    
    alice[i]->setupPhase();
    bob  [i]->setupPhase();

    /// For testing, assume correct identity and public key.
    alice[i]->putPublicKeyOther(bob  [i]->getIdentity(), 
                                bob  [i]->getPublicKey());
    bob  [i]->putPublicKeyOther(alice[i]->getIdentity(), 
                                alice[i]->getPublicKey());
    
    alice[i]->keyDerivationPhase();
    bob  [i]->keyDerivationPhase();
  }
}

// ============================================================================
void Spake2Tests::TearDownTestSuite()
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    delete alice[i];
    alice[i] = nullptr;
    
    delete bob[i];
    bob[i] = nullptr;
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareIdentities)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Public Keys p{A/B}
    ASSERT_STREQ(alice       [i]->getIdentity().c_str(), 
                 given_values[i].A_name.c_str());

    ASSERT_STREQ(bob         [i]->getIdentity().c_str(), 
                 given_values[i].B_name.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testComparePublicKeys)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Public Keys p{A/B}
    ASSERT_STREQ(alice[i]->getUncompressedPublicKey().c_str(), 
                 expected_values[i].pA.c_str());

    ASSERT_STREQ(bob[i]->getUncompressedPublicKey().c_str(), 
                 expected_values[i].pB.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareGroupElement)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Group Element K
    ASSERT_STREQ(alice[i]->getUncompressedGroupElement().c_str(), 
                 bob  [i]->getUncompressedGroupElement().c_str());
     
    ASSERT_STREQ(alice[i]->getUncompressedGroupElement().c_str(), 
                 expected_values[i].K.c_str());

    ASSERT_STREQ(bob[i]->getUncompressedGroupElement().c_str(), 
                 expected_values[i].K.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareTranscript)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Transcript TT
    ASSERT_STREQ(alice[i]->getTranscript().c_str(), 
                 bob  [i]->getTranscript().c_str());

    ASSERT_STREQ(alice[i]->getTranscript().c_str(), 
                 expected_values[i].TT.c_str());

    ASSERT_STREQ(bob[i]->getTranscript().c_str(), 
                 expected_values[i].TT.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareTranscriptHash)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Hash(TT)
    ASSERT_STREQ(alice[i]->getTranscript().c_str(), 
                 bob  [i]->getTranscript().c_str());

    ASSERT_STREQ(alice[i]->getTranscriptHash().c_str(), 
                 expected_values[i].hash_TT.c_str());

    ASSERT_STREQ(bob[i]->getTranscriptHash().c_str(), 
                 expected_values[i].hash_TT.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareSharedSymmetricSecrets)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Ke and Ka
    ASSERT_STREQ(alice[i]->getSharedSymmetricSecrets().Ke.c_str(), 
                 expected_values[i].Ke.c_str());

    ASSERT_STREQ(bob[i]->getSharedSymmetricSecrets().Ka.c_str(), 
                 expected_values[i].Ka.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareMacKeys)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// KcA || KcB
    ASSERT_STREQ(alice[i]->getMacKeys().KcA.c_str(), 
                 bob  [i]->getMacKeys().KcA.c_str());

    ASSERT_STREQ(alice[i]->getMacKeys().KcB.c_str(), 
                 bob  [i]->getMacKeys().KcB.c_str());

    ASSERT_STREQ(alice          [i]->getMacKeys().KcA.c_str(), 
                 expected_values[i].KcA.c_str());

    ASSERT_STREQ(bob            [i]->getMacKeys().KcB.c_str(), 
                 expected_values[i].KcB.c_str());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testCompareKeyConfirmations)
{
  for ( unsigned int i = 0; i < num_test_vectors; ++i )
  {
    /// Result of MAC
    ASSERT_STREQ(alice          [i]->getConfirmationKey().c_str(), 
                 expected_values[i].A_conf.c_str());

    ASSERT_STREQ(bob            [i]->getConfirmationKey().c_str(),
                 expected_values[i].B_conf.c_str());

    alice[i]->putConfirmationKeyOther(bob  [i]->getConfirmationKey());
    bob  [i]->putConfirmationKeyOther(alice[i]->getConfirmationKey());
    
    ASSERT_TRUE(alice[i]->checkProtocolComplete());
    ASSERT_TRUE(bob  [i]->checkProtocolComplete());
  }
}

// ============================================================================
TEST_F(Spake2Tests, testNominalExecution)
{
  Spake2 alice("alice", "foo", true);
  Spake2 bob  ("bob",   "foo", false);

  alice.setupPhase();
  bob.  setupPhase();

  alice.readOtherPartiesPublicKey();
  bob.  readOtherPartiesPublicKey();

  alice.keyDerivationPhase();
  bob.  keyDerivationPhase();

  alice.readOtherPartiesConfirmationKey();
  bob.  readOtherPartiesConfirmationKey();

  ASSERT_TRUE(alice.checkProtocolComplete());
  ASSERT_TRUE(bob.  checkProtocolComplete());
}

// ============================================================================
TEST_F(Spake2Tests, testMismatchedPrivateKeys)
{
  Spake2 alice("alice", "foo", true);
  Spake2 bob  ("bob",   "far", false);

  alice.setupPhase();
  bob.  setupPhase();

  alice.readOtherPartiesPublicKey();
  bob.  readOtherPartiesPublicKey();

  alice.keyDerivationPhase();
  bob.  keyDerivationPhase();

  alice.readOtherPartiesConfirmationKey();
  bob.  readOtherPartiesConfirmationKey();

  ASSERT_TRUE(!alice.checkProtocolComplete());
  ASSERT_TRUE(!bob.  checkProtocolComplete());
}

// ============================================================================
TEST_F(Spake2Tests, testIdenticalAdditionalAuthenticatedDats)
{
  Spake2 alice("alice", "foo", true,  "bar");
  Spake2 bob  ("bob",   "foo", false, "bar");

  alice.setupPhase();
  bob.  setupPhase();

  alice.readOtherPartiesPublicKey();
  bob.  readOtherPartiesPublicKey();

  alice.keyDerivationPhase();
  bob.  keyDerivationPhase();

  alice.readOtherPartiesConfirmationKey();
  bob.  readOtherPartiesConfirmationKey();

  ASSERT_TRUE(alice.checkProtocolComplete());
  ASSERT_TRUE(bob.  checkProtocolComplete());
}

// ============================================================================
TEST_F(Spake2Tests, testMismatchedAdditionalAuthenticatedDats)
{
  Spake2 alice("alice", "foo", true,  "bar");
  Spake2 bob  ("bob",   "foo", false, "baz");

  alice.setupPhase();
  bob.  setupPhase();

  alice.readOtherPartiesPublicKey();
  bob.  readOtherPartiesPublicKey();

  alice.keyDerivationPhase();
  bob.  keyDerivationPhase();

  alice.readOtherPartiesConfirmationKey();
  bob.  readOtherPartiesConfirmationKey();

  ASSERT_TRUE(!alice.checkProtocolComplete());
  ASSERT_TRUE(!bob.  checkProtocolComplete());
}