












#ifndef __COORDINATOR_ACTOR_H__
#define __COORDINATOR_ACTOR_H__

class GroundStationActor;
class SatelliteActor;

#include "AbstractActor.h"

class CoordinatorActor :
	public virtual AbstractActor
{

public:
    CoordinatorActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
    );
	~CoordinatorActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_slot;

    	boolean _ref_slotS1Ground;

    	boolean _ref_slotS2Ground;

    	boolean _ref_slotS3Ground;

    	boolean _ref_slotS1toS2;

    	boolean _ref_slotS2toS3;

    	boolean _ref_slotS1toS3;


		long constructorCoordinator(byte creatorID
		);

       virtual void _msg_nextSlot(byte senderId
        )
        ;
       virtual long msgsrvnextSlot(
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();
protected:
    	void _synchmethod_updateSlotFlags(
        )
        ;
};

#endif