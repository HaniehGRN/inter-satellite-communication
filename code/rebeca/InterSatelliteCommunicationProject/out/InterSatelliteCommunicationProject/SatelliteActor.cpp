                                    


#include "Types.h"
#include "SatelliteActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "GroundStationActor.h"
#include "CoordinatorActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define RECEIVEISL 1
#define GRANTGROUND 2
#define GRANTISL 3


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_gs (dynamic_cast<GroundStationActor*>(rebecs[(int)((unsigned char)known[1])]))
    	#define _ref_stA (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[2])]))
    	#define _ref_stB (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[3])]))
    
SatelliteActor::SatelliteActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize














)
	:
		AbstractTimedActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize














		)
	, AbstractActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, out
		#endif







		, stateSize, hashmapSize














	)
{
    messageNames = new char *[3 + 1];
    messagePriorities = new int[3 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"RECEIVEISL";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"GRANTGROUND";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"GRANTISL";
			messagePriorities[3] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
	}








	if (hashtableTimeExtension == (vector<TimeFrame>**)0xFFFFFFFF) {	// avoid reinitialization
        hashtableTimeExtension = new vector<TimeFrame>*[1 << hashmapSize];
        memset (hashtableTimeExtension, 0, sizeof(vector<TimeFrame>*)*(1 << hashmapSize));
	}







	
	
	queueTail = 0;
}

SatelliteActor::~SatelliteActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void SatelliteActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case RECEIVEISL: {

            	int _ref_type;
            
            
                	memcpy(&_ref_type, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_sId;
            
            
                	memcpy(&_ref_sId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_payload;
            
            
                	memcpy(&_ref_payload, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "receiveISL("
                	<< ((int)_ref_type)
					<< ", "
                	<< ((int)_ref_sId)
					<< ", "
                	<< ((int)_ref_payload)
                << ")";
                break;
               }
            case GRANTGROUND: {
                out << "grantGround("
                << ")";
                break;
               }
            case GRANTISL: {

            	SatelliteActor* _ref_target;
            
            
                	memcpy(&_ref_target, &paramQueue[cnt] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);
                out << "grantISL("
                	<< (_ref_target == NULL ? "NULL" : _ref_target->getName())
                << ")";
                break;
               }
    	}
}


void SatelliteActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void SatelliteActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Satellite.id\" type=\"int\">" <<  
			    		((int)_ref_id) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.msgCount\" type=\"int\">" <<  
			    		((int)_ref_msgCount) << "</variable>" << endl;
}


char* SatelliteActor::getClassName() {
    return (char*)"Satellite";
}
void SatelliteActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_msgCount, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void SatelliteActor::marshalActorToArray(byte* array) {
    int i,j, marshalIndex = 0;

	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = messageQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }
    
	for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
        array[marshalIndex++] = senderQueue[i];
    }

    if (shift) {
        array[marshalIndex++] = 0;
    }

	if(maxParamLength != 0) {
        for (i = (shift ? 1 : 0); i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = paramQueue[i][j];
            }
        }
        if (shift) {
            for (j = 0; j < maxParamLength; j++) {
                array[marshalIndex++] = 0;
            }
        }
    }

	 #ifdef NO_ATOMIC_EXECUTION
    	memcpy(&array [marshalIndex], &__pc, 4);
    	marshalIndex += 4;
    	//array[marshalIndex++] = __pc;
    #endif

		marshalVariablesToArray( & array[marshalIndex]);

   

}

void SatelliteActor::unmarshalActorFromArray(byte* array) {

    int index = 0, i, j;
    shift = 0;

	for (i = 0; i < maxQueueLength; i++) {
        messageQueue[i] = array[index++];
    }
	
	for (i = 0; i < maxQueueLength; i++) {
        senderQueue[i] = array[index++];
    }
    
	if(maxParamLength != 0) {
        for (i = 0; i < maxQueueLength; i++) {
            for (j = 0; j < maxParamLength; j++) {
                paramQueue[i][j] = array[index++];
            }
        }
    }
    
	#ifdef NO_ATOMIC_EXECUTION
    	memcpy(&__pc, &array [index], 4);
    	index += 4;
    	//__pc = array[index++];
    #endif
    
	unmarshalVariablesFromArray( & array[index]);
}

void SatelliteActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_id, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_msgCount, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void SatelliteActor::printStateContent(long stateNo) {
	out << myName << "=>[";
	byte* currentValues = getTable()[stateNo];
	for (int cnt = 0; cnt < STATE_SIZE; cnt++)
		out << (int)currentValues[cnt] << ", ";
	out << "]";
}
#endif
*/

//---------------------------------------------------------
/**
  Checks which action of this rebec is enabled, and executes it. Actions are the
  same as the location-counter defined at the beginning of this file.
  In the case of non-atomic message servers, a location-counter may point to the
  middle of a message server.
  If this action leads to more than one transition (non_det), this method executes
  only one of these transitions. With the help of some static variables, consecutive
  calls to this method execute different transitions caused by this action.
  Local variables must be properly valuated before any call to this method (see marshal()).

  returns: >0 : there are more possible next states (non-det transitions).
		   =0 : no more non_det transitions
		   -1 : no enabled action (empty queue)
		   -2 : Local hash table is full - immediate termination of search
 */
long SatelliteActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case RECEIVEISL:
                {
                    int i = 0;

            	int _ref_type;
            
            
                	memcpy(&_ref_type, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_sId;
            
            
                	memcpy(&_ref_sId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);

            	int _ref_payload;
            
            
                	memcpy(&_ref_payload, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvreceiveISL(
            _ref_type
	        , 
            _ref_sId
	        , 
            _ref_payload
                    );
                }
            case GRANTGROUND:
                {
                    int i = 0;
                    return msgsrvgrantGround(
                    );
                }
            case GRANTISL:
                {
                    int i = 0;

            	SatelliteActor* _ref_target;
            
            
                	memcpy(&_ref_target, &paramQueue[0] [i], (1 * REACTIVE_CLASS_SIZE));
	                i += (1 * REACTIVE_CLASS_SIZE);
                    return msgsrvgrantISL(
            _ref_target
                    );
                }
    }
	return -1;
}

#define _RETURN_ return 0
    long SatelliteActor::constructorSatellite (byte creatorId 
	        ,
    		int
	         _ref_myId
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Satellite";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_id=_ref_myId);
			(_ref_msgCount=1);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}

#define _RETURN_ return


		void SatelliteActor::_msg_receiveISL (byte senderId
	        ,
    		int
	         _ref_type
	        , 
    		int
	         _ref_sId
	        , 
    		int
	         _ref_payload
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RECEIVEISL);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_type, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_sId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
            
                	memcpy(&paramsValues [index], &_ref_payload, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void SatelliteActor::_msg_grantGround (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, GRANTGROUND);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void SatelliteActor::_msg_grantISL (byte senderId
	        ,
    		SatelliteActor*
	         _ref_target
  	  ){
        queueTail = 0;
        enqueue(messageQueue, GRANTISL);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_target, (1 * REACTIVE_CLASS_SIZE));
	                index += (1 * REACTIVE_CLASS_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** SatelliteActor::table = (byte**)0xFFFFFFFF;

	byte** SatelliteActor::getTable() {
		return table;
	}








	vector<TimeFrame>** SatelliteActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** SatelliteActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void SatelliteActor::_timed_msg_receiveISL (byte senderId
	        ,
    		int
	         _ref_type
	        , 
    		int
	         _ref_sId
	        , 
    		int
	         _ref_payload
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_receiveISL (senderId
	        ,
            _ref_type
	        , 
            _ref_sId
	        , 
            _ref_payload
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void SatelliteActor::_timed_msg_grantGround (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_grantGround (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	    void SatelliteActor::_timed_msg_grantISL (byte senderId
	        ,
    		SatelliteActor*
	         _ref_target
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_grantISL (senderId
	        ,
            _ref_target
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void SatelliteActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	#define _RETURN_ goto _RETURN_POINT_RECEIVEISL
	    long SatelliteActor::msgsrvreceiveISL (
    		int
	         _ref_type
	        , 
    		int
	         _ref_sId
	        , 
    		int
	         _ref_payload
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "receiveISL";
		#endif

	#ifdef TTS
	#endif

		{
			if ((_ref_msgCount<5)) {
				{
					(_ref_msgCount=(_ref_msgCount+1));
				}
}
		}
	_RETURN_POINT_RECEIVEISL:
		return 0;

}
	#define _RETURN_ goto _RETURN_POINT_GRANTGROUND
	    long SatelliteActor::msgsrvgrantGround (
    ) {
    			long arrayIndexChecker = 0;GroundStationActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "grantGround";
		#endif

	#ifdef TTS
	#endif

		{
			if ((_ref_msgCount>0)) {
				{
					(temp0=_ref_gs, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 44"), temp0)->_timed_msg_receiveFromSatellite(myID, _ref_id, _ref_now, MAX_TIME);
					(_ref_msgCount=(_ref_msgCount-1));
				}
}
		}
	_RETURN_POINT_GRANTGROUND:
		return 0;

}
	#define _RETURN_ goto _RETURN_POINT_GRANTISL
	    long SatelliteActor::msgsrvgrantISL (
    		SatelliteActor*
	         _ref_target
    ) {
    			long arrayIndexChecker = 0;SatelliteActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "grantISL";
		#endif

	#ifdef TTS
	#endif

		{
			if ((_ref_msgCount>0)) {
				{
					(temp0=_ref_target, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 52"), temp0)->_timed_msg_receiveISL(myID, 0, _ref_id, 100, _ref_now, MAX_TIME);
					(_ref_msgCount=(_ref_msgCount-1));
				}
}
		}
	_RETURN_POINT_GRANTISL:
		return 0;

}
