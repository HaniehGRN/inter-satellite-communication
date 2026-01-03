












#ifndef __GROUNDSTATION_ACTOR_H__
#define __GROUNDSTATION_ACTOR_H__

class SatelliteActor;
class CoordinatorActor;

#include "AbstractActor.h"

class GroundStationActor :
	public virtual AbstractActor
{

public:
    GroundStationActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
    );
	~GroundStationActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	std::array<int, 3> _ref_messageCount;

    	boolean _ref_deliveredS1;

    	boolean _ref_deliveredS2;

    	boolean _ref_deliveredS3;


		long constructorGroundStation(byte creatorID
		);

       virtual void _msg_receiveFromSatellite(byte senderId
	        ,
    		int
	         _ref_satId
        )
        ;
       virtual long msgsrvreceiveFromSatellite(
    		int
	         _ref_satId
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();
protected:
};

#endif