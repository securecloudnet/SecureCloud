// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX Core developers
// Copyright (c) 2017 The SecureCloud Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include "chainparamsbase.h"
#include "checkpoints.h"
#include "primitives/block.h"
#include "protocol.h"
#include "uint256.h"

#include <vector>

typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

struct CDNSSeedData {
    std::string name, host;
    CDNSSeedData(const std::string& strName, const std::string& strHost) : name(strName), host(strHost) {}
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * SecureCloud system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,     // BIP16
        EXT_PUBLIC_KEY, // BIP32
        EXT_SECRET_KEY, // BIP32
        EXT_COIN_TYPE,  // BIP44

        MAX_BASE58_TYPES
    };

    const uint256& HashGenesisBlock()                               const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart()                         const { return pchMessageStart; }
    const std::vector<unsigned char>& AlertKey()                    const { return vAlertPubKey; }
    const std::vector<unsigned char>& AlertKeyOld()                 const { return vAlertPubKeyOld; }
    int GetDefaultPort()                                            const { return nDefaultPort; }
    const uint256& ProofOfWorkLimit()                               const { return bnProofOfWorkLimit; }
    int SubsidyHalvingInterval()                                    const { return nSubsidyHalvingInterval; }
    int EnforceBlockUpgradeMajority()                               const { return nEnforceBlockUpgradeMajority; }   // Used to check majorities for block version upgrade
    int RejectBlockOutdatedMajority()                               const { return nRejectBlockOutdatedMajority; }
    int ToCheckBlockUpgradeMajority()                               const { return nToCheckBlockUpgradeMajority; }
    int MaxReorganizationDepth()                                    const { return nMaxReorganizationDepth; }
    int DefaultMinerThreads()                                       const { return nMinerThreads; }                  // Used if GenerateBitcoins is called with a negative number of threads
    const CBlock& GenesisBlock()                                    const { return genesis; }
    bool MiningRequiresPeers()                                      const { return fMiningRequiresPeers; }           // Make miner wait to have peers to avoid wasting work
    bool HeadersFirstSyncingActive()                                const { return fHeadersFirstSyncingActive; };    // Headers first syncing is disabled
    bool DefaultConsistencyChecks()                                 const { return fDefaultConsistencyChecks; }      // Default value for -checkmempool and -checkblockindex argument
    bool AllowMinDifficultyBlocks()                                 const { return fAllowMinDifficultyBlocks; }      // Allow mining of a min-difficulty block
    bool SkipProofOfWorkCheck()                                     const { return fSkipProofOfWorkCheck; }          // Skip proof-of-work check: allow mining of any difficulty block
    bool RequireStandard()                                          const { return fRequireStandard; }               // Make standard checks
    int64_t TargetTimespan()                                        const { return nTargetTimespan; }
    int64_t TargetSpacing()                                         const { return nTargetSpacing; }
    int64_t Interval()                                              const { return nTargetTimespan / nTargetSpacing; }
    int64_t TargetTimespanPOS()                                     const { return nTargetTimespanPOS; }
    int64_t TargetSpacingPOS()                                      const { return nTargetSpacingPOS; }
    int64_t IntervalPOS()                                           const { return nTargetTimespanPOS / nTargetSpacingPOS; }
    int LAST_POW_BLOCK()                                            const { return nLastPOWBlock; }
    int COINBASE_MATURITY()                                         const { return nMaturity; }
    int ModifierUpgradeBlock()                                      const { return nModifierUpdateBlock; }
    CAmount MaxMoneyOut()                                           const { return nMaxMoneyOut; }
    int MasternodeCountDrift()                                      const { return nMasternodeCountDrift; }          // The masternode count that we will allow the see-saw reward payments to be off by
    bool MineBlocksOnDemand()                                       const { return fMineBlocksOnDemand; }            // Make miner stop after a block is found. In RPC, don't return until nGenProcLimit blocks are generated
    bool TestnetToBeDeprecatedFieldRPC()                            const { return fTestnetToBeDeprecatedFieldRPC; } // In the future use NetworkIDString() for RPC fields
    std::string NetworkIDString()                                   const { return strNetworkID; }                   // Return the BIP70 network string (main, test or regtest)
    const std::vector<CDNSSeedData>& DNSSeeds()                     const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::vector<CAddress>& FixedSeeds()                       const { return vFixedSeeds; }
    int PoolMaxTransactions()                                       const { return nPoolMaxTransactions; }
    std::string SporkKey()                                          const { return strSporkKey; }
    std::string SporkKeyOld()                                       const { return strSporkKeyOld; }
    std::string MasternodePoolDummyAddress()                        const { return strMasternodePoolDummyAddress; }
    int64_t StartMasternodePayments()                               const { return nStartMasternodePayments; }
    int64_t Budget_Fee_Confirmations()                              const { return nBudget_Fee_Confirmations; }
    CBaseChainParams::Network NetworkID()                           const { return networkID; }
    int64_t StartNewKeys()                                          const { return nStartNewKeys; }

    virtual const Checkpoints::CCheckpointData& Checkpoints() const = 0;

protected:
    CChainParams() {}

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    std::vector<unsigned char> vAlertPubKey;
    std::vector<unsigned char> vAlertPubKeyOld;
    int nDefaultPort;
    uint256 bnProofOfWorkLimit;
    int nMaxReorganizationDepth;
    int nSubsidyHalvingInterval;
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;
    int64_t nTargetTimespan;
    int64_t nTargetSpacing;
    int64_t nTargetTimespanPOS;
    int64_t nTargetSpacingPOS;
    int nLastPOWBlock;
    int nMasternodeCountDrift;
    int nMaturity;
    int nModifierUpdateBlock;
    CAmount nMaxMoneyOut;
    int nMinerThreads;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    CBaseChainParams::Network networkID;
    std::string strNetworkID;
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
    bool fMiningRequiresPeers;
    bool fAllowMinDifficultyBlocks;
    bool fDefaultConsistencyChecks;
    bool fRequireStandard;
    bool fMineBlocksOnDemand;
    bool fSkipProofOfWorkCheck;
    bool fTestnetToBeDeprecatedFieldRPC;
    bool fHeadersFirstSyncingActive;
    int nPoolMaxTransactions;
    std::string strSporkKey;
    std::string strSporkKeyOld;
    std::string strMasternodePoolDummyAddress;
    int64_t nStartMasternodePayments;
    int64_t nBudget_Fee_Confirmations;
    int64_t nStartNewKeys;
};

/**
 * Modifiable parameters interface is used by test cases to adapt the parameters in order
 * to test specific features more easily. Test cases should always restore the previous
 * values after finalization.
 */

class CModifiableParams
{
public:
    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) = 0;
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) = 0;
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) = 0;
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) = 0;
    virtual void setDefaultConsistencyChecks(bool aDefaultConsistencyChecks) = 0;
    virtual void setAllowMinDifficultyBlocks(bool aAllowMinDifficultyBlocks) = 0;
    virtual void setSkipProofOfWorkCheck(bool aSkipProofOfWorkCheck) = 0;
};


/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams& Params();

/** Return parameters for the given network. */
CChainParams& Params(CBaseChainParams::Network network);

/** Get modifiable network parameters (UNITTEST only) */
CModifiableParams* ModifiableParams();

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CBaseChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

#endif // BITCOIN_CHAINPARAMS_H
