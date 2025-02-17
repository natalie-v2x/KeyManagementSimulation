//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

//
//  Description:
//  This file contains LTE typedefs and constants.
//  At the end of the file there are some utility functions.
//

#ifndef _LTE_LTECOMMON_H_
#define _LTE_LTECOMMON_H_

#define _NO_W32_PSEUDO_MODIFIERS

#include <iostream>
#include <omnetpp.h>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <set>
#include <queue>
#include <map>
#include <list>
#include <algorithm>
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/Protocol.h"
#include "common/features.h"
#include "common/LteCommonEnum_m.h"

#include <mutex>

#if defined(SIMULTE_EXPORT)
#  define SIMULTE_API    OPP_DLLEXPORT
#elif defined(SIMULTE_IMPORT)
#  define SIMULTE_API    OPP_DLLIMPORT
#else // if defined(INET_EXPORT)
#  define SIMULTE_API
#endif // if defined(INET_EXPORT

class LteBinder;
class LteCellInfo;
class LteMacEnb;
class LteMacBase;
class LtePhyBase;
class LteRealisticChannelModel;
class LteControlInfo;
class ExtCell;


/**
 * Lte specific protocols
 */
class SIMULTE_API LteProtocol {
public:
    static const inet::Protocol ipv4uu;  // IP protocol on the uU interface
    static const inet::Protocol pdcp;    // Packet Data Convergence Protocol
    static const inet::Protocol rlc;     // Radio Link Control
    static const inet::Protocol ltemac;  // LTE Medium Access Control
    static const inet::Protocol gtp;     // GPRS Tunneling Protocol
    static const inet::Protocol x2ap;    // X2AP Protocol
};

// global lock
//extern std::mutex mtx;
//extern int cnt;

/**
 * TODO
 */
#define ELEM(x) {x,#x}

/// Transmission time interval
#define TTI 0.001

/// Current simulation time
#define NOW omnetpp::simTime()

/// Node Id bounds
#define ENB_MIN_ID 1
#define ENB_MAX_ID 255
#define RELAY_MIN_ID 257
#define RELAY_MAX_ID 1023
#define UE_MIN_ID 1025
#define UE_MAX_ID 65535

/// Max Number of Codewords
#define MAX_CODEWORDS 2

// Number of QCI classes
#define LTE_QCI_CLASSES 9

/// MAC node ID
typedef unsigned short MacNodeId;

/// Cell node ID. It is numerically equal to eNodeB MAC node ID.
typedef unsigned short MacCellId;

/// X2 node ID. It is equal to the eNodeB MAC Cell ID
typedef unsigned short X2NodeId;

/// Omnet Node Id
typedef unsigned int OmnetId;

/// Logical Connection Identifier
typedef unsigned short LogicalCid;

/// Connection Identifier: <MacNodeId,LogicalCid>
typedef unsigned int MacCid;

/// Rank Indicator
typedef unsigned short Rank;

/// Channel Quality Indicator
typedef unsigned short Cqi;

/// Precoding Matrix Index
typedef unsigned short Pmi;

/// Transport Block Size
typedef unsigned short Tbs;

/// Logical band
typedef unsigned short Band;

/// Codeword
typedef unsigned short Codeword;




// QCI traffic descriptor
struct SIMULTE_API QCIParameters
{
    int priority;
    double packetDelayBudget;
    double packetErrorLossRate;
};


// Attenuation vector for analogue models
typedef std::vector<double> AttenuationVector;




struct SIMULTE_API ApplicationTable
{
    ApplicationType app;
    std::string appName;
};

const ApplicationTable applications[] = {
    ELEM(VOIP),
    ELEM(VOD),
    ELEM(WEB),
    ELEM(CBR),
    ELEM(FTP),
    ELEM(FULLBUFFER),
    ELEM(UNKNOWN_APP)
};

/**************************
 * Scheduling discipline  *
 **************************/

struct SIMULTE_API SchedDisciplineTable
{
    SchedDiscipline discipline;
    std::string disciplineName;
};

const SchedDisciplineTable disciplines[] = {
    ELEM(DRR),
    ELEM(PF),
    ELEM(MAXCI),
    ELEM(MAXCI_MB),
    ELEM(MAXCI_OPT_MB),
    ELEM(MAXCI_COMP),
    ELEM(ALLOCATOR_BESTFIT),
    ELEM(UNKNOWN_DISCIPLINE)
};

/*************************
 *   Transmission Modes  *
 *************************/
struct SIMULTE_API TxTable
{
    TxMode tx;
    std::string txName;
};

const TxTable txmodes[] = {
    ELEM(SINGLE_ANTENNA_PORT0),
    ELEM(SINGLE_ANTENNA_PORT5),
    ELEM(TRANSMIT_DIVERSITY),
    ELEM(OL_SPATIAL_MULTIPLEXING),
    ELEM(CL_SPATIAL_MULTIPLEXING),
    ELEM(MULTI_USER),
    ELEM(UNKNOWN_TX_MODE)
};


struct SIMULTE_API TxDirectionTable
{
    TxDirectionType txDirection;
    std::string txDirectionName;
};

const TxDirectionTable txDirections[] = {
    ELEM(ANISOTROPIC),
    ELEM(OMNI)
};


struct SIMULTE_API FeedbackRequest
{
    bool request;
    FeedbackGeneratorType genType;
    FeedbackType type;
    //used if genType==real
    TxMode txMode;
    bool dasAware;
    RbAllocationType rbAllocationType;
};

struct SIMULTE_API FeedbackGeneratorTypeTable
{
    FeedbackGeneratorType ty;
    std::string tyname;
};

const FeedbackGeneratorTypeTable feedbackGeneratorTypeTable[] = {
    ELEM(IDEAL),
    ELEM(REAL),
    ELEM(DAS_AWARE),
    ELEM(UNKNOW_FB_GEN_TYPE)
};

/// Number of transmission modes in DL direction.
const unsigned char DL_NUM_TXMODE = MULTI_USER + 1;

/// Number of transmission modes in UL direction.
const unsigned char UL_NUM_TXMODE = MULTI_USER + 1;



struct SIMULTE_API DeploymentScenarioMapping
{
    DeploymentScenario scenario;
    std::string scenarioName;
};

const DeploymentScenarioMapping DeploymentScenarioTable[] = {
    ELEM(INDOOR_HOTSPOT),
    ELEM(URBAN_MICROCELL),
    ELEM(URBAN_MACROCELL),
    ELEM(RURAL_MACROCELL),
    ELEM(SUBURBAN_MACROCELL),
    ELEM(UNKNOW_SCENARIO)
};

const unsigned int txModeToIndex[6] = { 0, 0, 1, 2, 2, 0 };

const TxMode indexToTxMode[3] = {
    SINGLE_ANTENNA_PORT0,
    TRANSMIT_DIVERSITY,
    OL_SPATIAL_MULTIPLEXING
};

typedef std::map<MacNodeId, TxMode> TxModeMap;

const double cqiToByteTms[16] = { 0, 2, 3, 5, 11, 15, 20, 25, 36, 38, 49, 63, 72, 79, 89, 92 };

struct SIMULTE_API Lambda
{
    unsigned int index;
    unsigned int lambdaStart;
    unsigned int channelIndex;
    double lambdaMin;
    double lambdaMax;
    double lambdaRatio;
};

double dBmToLinear(double dbm);
double dBToLinear(double db);
double linearToDBm(double lin);
double linearToDb(double lin);

/*************************
 *      DAS Support      *
 *************************/

struct SIMULTE_API RemoteTable
{
    Remote remote;
    std::string remoteName;
};

const RemoteTable remotes[] = {
    ELEM(MACRO),
    ELEM(RU1),
    ELEM(RU2),
    ELEM(RU3),
    ELEM(RU4),
    ELEM(RU5),
    ELEM(RU6),
    ELEM(UNKNOWN_RU)
};

/**
 * Maximum number of available DAS RU per cell.
 * To increase this number, change former enumerate accordingly.
 * MACRO antenna excluded.
 */
const unsigned char NUM_RUS = RU6;

/**
 * Maximum number of available ANTENNAS per cell.
 * To increase this number, change former enumerate accordingly.
 * MACRO antenna included.
 */
const unsigned char NUM_ANTENNAS = NUM_RUS + 1;

/**
 *  Block allocation Map: # of Rbs per Band, per Remote.
 */
typedef std::map<Remote, std::map<Band, unsigned int> > RbMap;

struct SIMULTE_API LtePhyFrameTable
{
    LtePhyFrameType phyType;
    std::string phyName;
};

const LtePhyFrameTable phytypes[] = {
    ELEM(DATAPKT),
    ELEM(BROADCASTPKT),
    ELEM(FEEDBACKPKT),
    ELEM(HANDOVERPKT),
    ELEM(GRANTPKT),
    ELEM(D2DMODESWITCHPKT),
    ELEM(UNKNOWN_TYPE)
};

struct SIMULTE_API LteNodeTable
{
    LteNodeType node;
    std::string nodeName;
};

const LteNodeTable nodetypes[] = {
    ELEM(INTERNET),
    ELEM(ENODEB),
    ELEM(RELAY),
    ELEM(UE),
    ELEM(UNKNOWN_NODE_TYPE)
};


struct SIMULTE_API LteSubFrameTypeTable
{
    LteSubFrameType type;
    std::string typeName;
};

const LteSubFrameTypeTable subFrametypes[] = {
    ELEM(NORMAL_FRAME_TYPE),
    ELEM(MBSFN),
    ELEM(PAGING),
    ELEM(BROADCAST),
    ELEM(SYNCRO),
    ELEM(ABS),
    ELEM(UNKNOWN_FRAME_TYPE)
};

//|--------------------------------------------------|
//|----------------- ABS Management -----------------|
//|--------------------------------------------------|
//********* See 3GPP TS 36.423 for more info *********
const int ABS_WIN = 40;
typedef std::bitset<ABS_WIN> AbsBitset;

/*
 * ABS_INFO         macro->micro
 * ABS_STATUS_INFO    micro->macro
 *
 * these are the names from standard, so it's not my fault if they are stupid.
 */

// Abs Status Information structure
struct SIMULTE_API AbsStatusInfoMsg
{
    double absStatus;
    AbsBitset usableAbsInfo;
};

//|--------------------------------------------------|

/*****************
 * X2 Support
 *****************/

class X2InformationElement;

/**
 * The Information Elements List, a list
 * of IEs contained inside a X2Message
 */
typedef std::list<X2InformationElement*> X2InformationElementsList;



/**
 * The following structure specifies a band and a byte amount which limits the schedulable data
 * on it.
 * If this limit is -1, it is considered an unlimited capping.
 * If this limit is -2, the band cannot be used.
 * Among other modules, the rtxAcid and grant methods of LteSchedulerEnb use this structure.
 */
struct SIMULTE_API BandLimit
{
    /// Band which the element refers to
    Band band_;
    /// Limit of bytes (per codeword) which can be requested for the current band
    std::vector<int> limit_;

    BandLimit()
    {
        band_ = 0;
        limit_.resize(MAX_CODEWORDS, -1);
    }

    // default "from Band" constructor
    BandLimit(Band b)
    {
        band_ = b;
        limit_.resize(MAX_CODEWORDS, -1);
    }
    bool operator<(const BandLimit rhs) const
    {
        return (limit_[0] > rhs.limit_[0]);
    }
};

/*****************
 * LTE Constants
 *****************/

const unsigned char MAXCW = 2;
const Cqi MAXCQI = 15;
const Cqi NOSIGNALCQI = 0;
const Pmi NOPMI = 0;
const Rank NORANK = 1;
const Tbs CQI2ITBSSIZE = 29;
const unsigned int PDCP_HEADER_UM = 1;
const unsigned int PDCP_HEADER_AM = 2;
const unsigned int RLC_HEADER_UM = 2; // TODO
const unsigned int RLC_HEADER_AM = 2; // TODO
const unsigned int MAC_HEADER = 2;
const unsigned int MAXGRANT = 4294967295U;

/*****************
 * MAC Support
 *****************/

class LteMacBuffer;
class LteMacQueue;
class MacControlElement;
class LteMacPdu;

/**
 * This is a map that associates each Connection Id with
 * a Mac Queue, storing  MAC SDUs (or RLC PDUs)
 */
typedef std::map<MacCid, LteMacQueue*> LteMacBuffers;

/**
 * This is a map that associates each Connection Id with
 *  a buffer storing the  MAC SDUs info (or RLC PDUs).
 */
typedef std::map<MacCid, LteMacBuffer*> LteMacBufferMap;

/**
 * This is the Schedule list, a list of schedule elements.
 * For each CID on each codeword there is a number of SDUs
 */
typedef std::map<std::pair<MacCid, Codeword>, unsigned int> LteMacScheduleList;

/**
 * This is the Pdu list, a list of scheduled Pdus for
 * each user on each codeword.
 */
typedef std::map<std::pair<MacNodeId, Codeword>, inet::Packet*> MacPduList;

/*
 * Codeword list : for each node, it keeps track of allocated codewords (number)
 */
typedef std::map<MacNodeId, unsigned int> LteMacAllocatedCws;

/**
 * The Rlc Sdu List, a list of RLC SDUs
 * contained inside a RLC PDU
 */
typedef std::list<inet::Packet*> RlcSduList;
typedef std::list<size_t> RlcSduListSizes;

/**
 * The Mac Sdu List, a list of MAC SDUs
 * contained inside a MAC PDU
 */
typedef omnetpp::cPacketQueue MacSduList;

/**
 * The Mac Control Elements List, a list
 * of CEs contained inside a MAC PDU
 */
typedef std::list<MacControlElement*> MacControlElementsList;

/*****************
 * HARQ Support
 *****************/

/// Unknown acid code
#define HARQ_NONE 255

/// Number of harq tx processes
#define ENB_TX_HARQ_PROCESSES 8
#define UE_TX_HARQ_PROCESSES 8
#define ENB_RX_HARQ_PROCESSES 8
#define UE_RX_HARQ_PROCESSES 8

/// time interval between two transmissions of the same pdu
#define HARQ_TX_INTERVAL 7*TTI

/// time it takes to generate feedback for a pdu
#define HARQ_FB_EVALUATION_INTERVAL 3*TTI

struct SIMULTE_API RemoteUnitPhyData
{
    int txPower;
    inet::Coord m;
};

// Codeword List - returned by Harq functions
typedef std::list<Codeword> CwList;

/// Pair of acid, list of unit ids
typedef std::pair<unsigned char, CwList> UnitList;

/*********************
 * Incell Interference Support
 *********************/
struct SIMULTE_API EnbInfo
{
    bool init;         // initialization flag
    EnbType type;     // MICRO_ENB or MACRO_ENB
    double txPwr;
    TxDirectionType txDirection;
    double txAngle;
    MacNodeId id;
    LteMacEnb * mac;
    LteRealisticChannelModel * realChan;
    omnetpp::cModule * eNodeB;
    int x2;
};

struct SIMULTE_API UeInfo
{
    bool init;         // initialization flag
    double txPwr;
    MacNodeId id;
    MacNodeId cellId;
    LteRealisticChannelModel * realChan;
    omnetpp::cModule * ue;
    LtePhyBase* phy;
};

// uplink interference support
struct SIMULTE_API UeAllocationInfo{
    MacNodeId nodeId;
    MacCellId cellId;
    LtePhyBase* phy;
    Direction dir;
};

typedef std::vector<ExtCell*> ExtCellList;

/*****************
 *  PHY Support  *
 *****************/

typedef std::vector<std::vector<std::vector<double> > > BlerCurves;

/*************************************
 * Shortcut for structures using STL
 *************************************/

typedef std::vector<Cqi> CqiVector;
typedef std::vector<Pmi> PmiVector;
typedef std::set<Band> BandSet;
typedef std::set<Remote> RemoteSet;
typedef std::map<MacNodeId, bool> ConnectedUesMap;
typedef std::pair<int, omnetpp::simtime_t> PacketInfo;
typedef std::vector<RemoteUnitPhyData> RemoteUnitPhyDataVector;
typedef std::set<MacNodeId> ActiveUser;
typedef std::set<MacCid> ActiveSet;

/**
 * Used at initialization to pass the parameters
 * to the AnalogueModels and Decider.
 *
 * Parameters read from xml file are stored in this map.
 */
typedef std::map<std::string, omnetpp::cMsgPar> ParameterMap;

/*********************
 * Utility functions
 *********************/

SIMULTE_API const std::string dirToA(Direction dir);
SIMULTE_API const std::string d2dModeToA(LteD2DMode mode);
SIMULTE_API const std::string allocationTypeToA(RbAllocationType type);
SIMULTE_API const std::string modToA(LteMod mod);
SIMULTE_API const std::string periodicityToA(FbPeriodicity per);
SIMULTE_API const std::string txModeToA(TxMode tx);
SIMULTE_API TxMode aToTxMode(std::string s);
SIMULTE_API const std::string schedDisciplineToA(SchedDiscipline discipline);
SIMULTE_API SchedDiscipline aToSchedDiscipline(std::string s);
SIMULTE_API Remote aToDas(std::string s);
SIMULTE_API const std::string dasToA(const Remote r);
SIMULTE_API const std::string nodeTypeToA(const LteNodeType t);
SIMULTE_API LteNodeType aToNodeType(std::string name);
SIMULTE_API LteNodeType getNodeTypeById(MacNodeId id);
SIMULTE_API FeedbackType getFeedbackType(std::string s);
SIMULTE_API RbAllocationType getRbAllocationType(std::string s);
SIMULTE_API ApplicationType aToApplicationType(std::string s);
SIMULTE_API const std::string applicationTypeToA(std::string s);
SIMULTE_API const std::string lteTrafficClassToA(LteTrafficClass type);
SIMULTE_API LteTrafficClass aToLteTrafficClass(std::string s);
SIMULTE_API const std::string phyFrameTypeToA(const LtePhyFrameType r);
SIMULTE_API LtePhyFrameType aToPhyFrameType(std::string s);
SIMULTE_API const std::string rlcTypeToA(LteRlcType type);
SIMULTE_API char* cStringToLower(char* str);
SIMULTE_API LteRlcType aToRlcType(std::string s);
SIMULTE_API const std::string planeToA(Plane p);
SIMULTE_API MacNodeId ctrlInfoToUeId(LteControlInfo * info);
SIMULTE_API MacCid idToMacCid(MacNodeId nodeId, LogicalCid lcid);
SIMULTE_API MacCid ctrlInfoToMacCid(LteControlInfo * info);        // get the CID from the packet control info
SIMULTE_API MacNodeId MacCidToNodeId(MacCid cid);
SIMULTE_API LogicalCid MacCidToLcid(MacCid cid);
SIMULTE_API GrantType aToGrantType(std::string a);
SIMULTE_API const std::string grantTypeToA(GrantType gType);
SIMULTE_API LteBinder* getBinder();
SIMULTE_API LteCellInfo* getCellInfo(MacNodeId nodeId);
SIMULTE_API omnetpp::cModule* getMacByMacNodeId(MacNodeId nodeId);
SIMULTE_API omnetpp::cModule* getRlcByMacNodeId(MacNodeId nodeId, LteRlcType rlcType);
SIMULTE_API LteMacBase* getMacUe(MacNodeId nodeId);
SIMULTE_API FeedbackGeneratorType getFeedbackGeneratorType(std::string s);
SIMULTE_API const std::string fbGeneratorTypeToA(FeedbackGeneratorType type);
SIMULTE_API LteSubFrameType aToSubFrameType(std::string s);
SIMULTE_API const std::string SubFrameTypeToA(const LteSubFrameType r);
SIMULTE_API const std::string DeploymentScenarioToA(DeploymentScenario type);
SIMULTE_API DeploymentScenario aToDeploymentScenario(std::string s);
SIMULTE_API bool isMulticastConnection(LteControlInfo* lteInfo);

/**
 * Utility function that reads the parameters of an XML element
 * and stores them in the passed ParameterMap reference.
 *
 * @param xmlData XML parameters config element related to a specific section
 * @param[output] outputMap map to store read parameters
 */
SIMULTE_API void getParametersFromXML(omnetpp::cXMLElement* xmlData, ParameterMap& outputMap);

/**
 * Parses a CSV string parameter into an int array.
 *
 * The string must be in the format "v1,v2,..,vi,...,vN;" and if dim > N,
 * the values[i] are filled with zeros from when i = N + 1.
 * Warning messages are issued if the string has less or more values than dim.
 *
 * @param str string to be parsed
 * @param values pointer to the int array
 * @param dim dimension of the values array
 * @param pad default value to be used when the string has less than dim values
 */
SIMULTE_API void parseStringToIntArray(std::string str, int* values, int dim, int pad);

/**
 * Initializes module's channels
 *
 * A dinamically created node needs its channels to be initialized, this method
 * runs through all a module's and its submodules' channels recursively and
 * initializes all channels.
 *
 * @param mod module whose channels needs initialization
 */
SIMULTE_API void initializeAllChannels(omnetpp::cModule *mod);

SIMULTE_API void removeAllSimuLteTags(inet::Packet *pkt);


template <typename T>
  bool checkIfHeaderType(const inet::Packet *pkt, bool checkFirst = false) {

    auto pktAux = pkt->dup();

    int index = 0;
    while(pktAux->getBitLength() > 0 && pktAux->peekAtFront<inet::Chunk>()) {
        auto chunk = pktAux->popAtFront<inet::Chunk>();
        if (inet::dynamicPtrCast<const T>(chunk)) {
            delete pktAux;
            if (index != 0 && checkFirst)
                throw omnetpp::cRuntimeError("The header is not the top");
            return true;
        }
        index++;
    }
    delete pktAux;
    return false;
}

template <typename T>
std::vector<T> getTagsWithInherit(inet::Packet *pkt)
{
    std::vector<T> t;
    auto tags = pkt->getTags();
    if (tags.getNumTags() == 0)
        return t;

    // check if exist tag of that is derived from this.
    //
    for (unsigned int i = 0; i < tags.getNumTags(); i++) {
        T * temp = dynamic_cast<T *> (tags.getTag(i));
        if (temp != nullptr) {
            t.push_back(*temp);
        }
    }
    return t;
}


#endif

