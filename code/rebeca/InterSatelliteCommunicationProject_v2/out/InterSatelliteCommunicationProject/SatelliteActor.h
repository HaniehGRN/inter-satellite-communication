












#ifndef __SATELLITE_ACTOR_H__
#define __SATELLITE_ACTOR_H__

class GroundStationActor;
class CoordinatorActor;

#include "AbstractActor.h"

class SatelliteActor :
	public virtual AbstractActor
{

public:
    SatelliteActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
    );
	~SatelliteActor();
	
	long methodSelectorAndExecutor();
	
    char* getClassName();
    
	virtual void exportStateVariablesInXML(ostream &out, string tab);
	virtual void exportAMessageInXML(ostream &out, string tab, int cnt);

	virtual void exportInsideStateVariablesTagInXML(ostream &out, string tab);

    virtual void marshalActorToArray(byte* array);
    void marshalVariablesToArray(byte * array);


    virtual void unmarshalActorFromArray(byte* array);
    void unmarshalVariablesFromArray(byte * array);


    	int _ref_id;

    	std::array<int, 5> _ref_qType;

    	std::array<int, 5> _ref_qSender;

    	std::array<int, 5> _ref_qPayload;

    	int _ref_head;

    	int _ref_tail;

    	int _ref_size;

    	int _ref_typeCounter;

    	int _ref_droppedCount;

    	int _ref_energy;

    	boolean _ref_safeMode;

    	int _ref_SAFE_TH;

    	int _ref_CRIT_TH;

    	int _ref_COST_RX;

    	int _ref_COST_ISL_TX;

    	int _ref_COST_GS_TX;

    	int _ref_CHARGE_PER_SLOT;

    	int _ref_MAX_ENERGY;

    	boolean _ref_energyOK;

    	boolean _ref_pending;

    	boolean _ref_bufOK;

    	boolean _ref_droppedEver;

    	boolean _ref_sentGroundEver;

    	boolean _ref_sentISLEver;

    	int _ref_BUF_CAP;


		long constructorSatellite(byte creatorID
	        ,
    		int
	         _ref_myId
		);

       virtual void _msg_startSystem(byte senderId
        )
        ;
       virtual long msgsrvstartSystem(
        )
        ;
       virtual void _msg_generateTraffic(byte senderId
	        ,
    		int
	         _ref_limit
        )
        ;
       virtual long msgsrvgenerateTraffic(
    		int
	         _ref_limit
        )
        ;
       virtual void _msg_chargeSlot(byte senderId
        )
        ;
       virtual long msgsrvchargeSlot(
        )
        ;
       virtual void _msg_receiveISL(byte senderId
	        ,
    		int
	         _ref_type
	        , 
    		int
	         _ref_sId
	        , 
    		int
	         _ref_payload
        )
        ;
       virtual long msgsrvreceiveISL(
    		int
	         _ref_type
	        , 
    		int
	         _ref_sId
	        , 
    		int
	         _ref_payload
        )
        ;
       virtual void _msg_grantGround(byte senderId
        )
        ;
       virtual long msgsrvgrantGround(
        )
        ;
       virtual void _msg_grantISL(byte senderId
	        ,
    		SatelliteActor*
	         _ref_target
        )
        ;
       virtual long msgsrvgrantISL(
    		SatelliteActor*
	         _ref_target
        )
        ;








	protected:
	    static byte** table;
		virtual byte** getTable();
protected:
    	void _synchmethod_enqueue(
    		int
	         _ref_type
	        , 
    		int
	         _ref_senderId
	        , 
    		int
	         _ref_payload
        )
        ;
    	void _synchmethod_updateEnergyOK(
        )
        ;
    	void _synchmethod_updateLocalFlags(
        )
        ;
    	void _synchmethod_updateSafeMode(
        )
        ;
};

#endif