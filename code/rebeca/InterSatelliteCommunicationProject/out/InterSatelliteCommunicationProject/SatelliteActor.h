












#ifndef __SATELLITE_ACTOR_H__
#define __SATELLITE_ACTOR_H__

class GroundStationActor;
class CoordinatorActor;

#include "AbstractTimedActor.h"

class SatelliteActor :
	public virtual AbstractTimedActor
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

    	int _ref_msgCount;


		long constructorSatellite(byte creatorID
	        ,
    		int
	         _ref_myId
		);

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
	    static vector<TimeFrame>** hashtableTimeExtension;
		virtual vector<TimeFrame>** getHashtableTimeExtension();







public:
      	  void _timed_msg_receiveISL(byte senderId
	        ,
    		int
	         _ref_type
	        , 
    		int
	         _ref_sId
	        , 
    		int
	         _ref_payload
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_grantGround(byte senderId
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
      	  void _timed_msg_grantISL(byte senderId
	        ,
    		SatelliteActor*
	         _ref_target
        	, TIME_TYPE executionTime, TIME_TYPE deadline)
        		;
    virtual void setNow(TIME_TYPE now);
    
protected:
};

#endif