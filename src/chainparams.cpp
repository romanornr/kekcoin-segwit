// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "streams.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << CScriptNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    // txNew.vout[0].scriptPubKey.clear();
    txNew.vout[0].nValue = 0;
    txNew.vout[0].scriptPubKey.clear();

    CBlock genesis;
    genesis.nTime    = 1498944188;
    genesis.nBits    = 0x1e0fffff;
    genesis.nNonce   = 922517;
    genesis.nVersion = 1;
    genesis.vtx.push_back(txNew);
    genesis.vtx[0].nTime = 1498944188;
    genesis.vtx[0].UpdateHash();
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "21 Jan 2017 Wall Street ends higher as Trump becomes president";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000091bc0f9d1578c7979142b2ff70e6bf8ff7c388cf3dcb486cf19a7518949");
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetTimespan = 60; // two weeks
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 7056; // 70% of 10080
        consensus.nMinerConfirmationWindow = 10080; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1514764800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1546300800; // May 1st, 2017

        // Deployment of SegWit (BIP141 and BIP143)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1514764800; // May 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1546300800; // May 1st, 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x11;
        pchMessageStart[1] = 0x22;
        pchMessageStart[2] = 0x33;
        pchMessageStart[3] = 0x44;
        nDefaultPort = 13337;
        nPruneAfterHeight = 100000;
        bnProofOfWorkLimit = arith_uint256(~arith_uint256() >> 20);

        genesis = CreateGenesisBlock(1498944188, 922517, 0x1e0fffff, 1, 0 * COIN);

	      uint256 hashGenesisBlock = uint256S("0x0000091bc0f9d1578c7979142b2ff70e6bf8ff7c388cf3dcb486cf19a7518949");

	      // Change to true to enable genesis block creation

        if (false && genesis.GetHash() != hashGenesisBlock)
        {
            printf("recalculating params for mainnet.\n");
            printf("old mainnet genesis nonce: %d\n", genesis.nNonce);
            printf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(; genesis.GetHash() < consensus.powLimit; genesis.nNonce++){ }
            LogPrintf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            LogPrintf("new mainnet genesis nonce: %d\n", genesis.nNonce);
            LogPrintf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }

	      consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x0000091bc0f9d1578c7979142b2ff70e6bf8ff7c388cf3dcb486cf19a7518949"));
        assert(genesis.hashMerkleRoot == uint256S("0x833c5ee04faab2268a87d8d0a41e96ccb514068a66c7e74322315322f4f149d0"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,45);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,133);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x03)(0x77)(0xD4)(0x4D).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x03)(0x77)(0xEE)(0xE1).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds.clear();
      	vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("139.59.165.143", "139.59.165.143"));
        vSeeds.push_back(CDNSSeedData("188.166.173.73", "188.166.173.73"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0,   uint256S("0x0000091bc0f9d1578c7979142b2ff70e6bf8ff7c388cf3dcb486cf19a7518949"))  //Genesis
            (100,   uint256S("0x863e40bbb850df57a5e18c329ce11e04c28aba71dfc61cd1c608dc89239f1014")) //Last PoW
            (101,   uint256S("0x29a21bdb6d93749f25391bd89c433ae1821dfff52678687ce54ace373c1096e8")),//First PoS
            1500672704, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            7000         // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 900000;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 16);
        consensus.nPowTargetTimespan = 30; // two weeks
        consensus.nPowTargetSpacing = 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 90; // 75% of 120
        consensus.nMinerConfirmationWindow = 120; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141 and BIP143)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        pchMessageStart[0] = 0x55;
        pchMessageStart[1] = 0x66;
        pchMessageStart[2] = 0x77;
        pchMessageStart[3] = 0x88;
        nDefaultPort = 13777;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1498944188, 18717, 0x1e0fffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        uint256 hashGenesisBlock = uint256S("0x0000c7b67a057053c5043fad3ae7896f3d3172361ba4a850abb24f6dd80df5dc");

        // Change to true to enable genesis block creation

        if (false && genesis.GetHash() != hashGenesisBlock)
        {
            printf("recalculating params for testnet.\n");
            printf("old testnet genesis nonce: %d\n", genesis.nNonce);
            printf("old testnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(; genesis.GetHash() > consensus.powLimit; genesis.nNonce++){ }
            printf("new testnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            printf("new testnet genesis nonce: %d\n", genesis.nNonce);
            printf("new testnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }



        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0000000013cb675cc890cf8c7a22f1f3948684b297ccd2553d6e203e00198ae0")),
            1484131714,
            0,
            100
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0x99;
        pchMessageStart[1] = 0x98;
        pchMessageStart[2] = 0x97;
        pchMessageStart[3] = 0x96;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1411111111, 0, 0x1e0fffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

	// Need to be reviewed

        //assert(consensus.hashGenesisBlock == uint256S("0x0000000013cb675cc890cf8c7a22f1f3948684b297ccd2553d6e203e00198ae0"));
        //assert(genesis.hashMerkleRoot == uint256S("0x3c71675ea78d84a0aafd2ec31a82745cd7c2844b6f77616a7f36134f0fd3e30b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x0000000013cb675cc890cf8c7a22f1f3948684b297ccd2553d6e203e00198ae0")),
            0,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,96);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,39);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0x38)(0x34)(0x76).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x06)(0x37)(0x64)(0x13).convert_to_container<std::vector<unsigned char> >();
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
