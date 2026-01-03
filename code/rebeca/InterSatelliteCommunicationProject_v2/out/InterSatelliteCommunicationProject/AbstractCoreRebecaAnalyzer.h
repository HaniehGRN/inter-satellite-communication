#ifndef __ABSTRACT_CORE_REBECA_ANALYZER_H__
#define __ABSTRACT_CORE_REBECA_ANALYZER_H__







#include "CoreRebecaDFSHashmap.h"
#include "AbstractActor.h"

    #include "GroundStationActor.h"
    #include "SatelliteActor.h"
    #include "CoordinatorActor.h"

#include <iostream>
#include <queue>

#define ON_STACK  2

class AbstractCoreRebecaAnalyzer {

protected:
	vector<AbstractActor*> rebecs;
	CoreRebecaDFSHashmap *storage;
	// This variable is used for initialization of TimedBFSHashmap
	int hashmapSize;
	//Analysis of Timed Rebeca are based on BFS traversal which requires an open border queue
	int numberOfTransitions;
	int numberOfStates;
	
	ostream &analysisOutputStream;

	virtual void exportState(State* current, ostream& outStream);
	virtual void exportTransition(State* source, State* destination, 
		string sender, string owner, string label, ostream& outStream);
	virtual void exportTransition(string source, string destination, 
		string sender, string owner, string label, ostream& outStream);
		
	State* storeRecentlyCreatedState(byte &result, short int claim, short int fairnessCounter);

    virtual void marshalRebecs(long* node);
    virtual void unmarshalRebecs(long* node);

    	boolean _propDef_s1GroundSlot ();
    	boolean _propDef_s2GroundSlot ();
    	boolean _propDef_s3GroundSlot ();
    	boolean _propDef_s1toS2Slot ();
    	boolean _propDef_s2toS3Slot ();
    	boolean _propDef_s1toS3Slot ();
    	boolean _propDef_s1EnergyOK ();
    	boolean _propDef_s2EnergyOK ();
    	boolean _propDef_s3EnergyOK ();
    	boolean _propDef_s1Pending ();
    	boolean _propDef_s2Pending ();
    	boolean _propDef_s3Pending ();
    	boolean _propDef_s1BufOK ();
    	boolean _propDef_s2BufOK ();
    	boolean _propDef_s3BufOK ();
    	boolean _propDef_s1Dropped ();
    	boolean _propDef_s2Dropped ();
    	boolean _propDef_s3Dropped ();
    	boolean _propDef_s1DeliveredToGS ();
    	boolean _propDef_s2DeliveredToGS ();
    	boolean _propDef_s3DeliveredToGS ();
    	boolean _propDef_s1ActualGroundSend ();
    	boolean _propDef_s2ActualGroundSend ();
    	boolean _propDef_s3ActualGroundSend ();
    	boolean _propDef_s1ActualSendToS2 ();
    	boolean _propDef_s2ActualSendToS3 ();
    	boolean _propDef_s1ActualSendToS3 ();
    
public:

	AbstractCoreRebecaAnalyzer(std::ostream &out, int hashmapSize);
	virtual void instantiationPart();
};

#endif