                                    


#include "Types.h"
#include "CoordinatorActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "GroundStationActor.h"
#include "SatelliteActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define NEXTSLOT 1


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<CoordinatorActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_s1 (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[1])]))
    	#define _ref_s2 (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[2])]))
    	#define _ref_s3 (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[3])]))
    
CoordinatorActor::CoordinatorActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[1 + 1];
    messagePriorities = new int[1 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"NEXTSLOT";
			messagePriorities[1] = std::numeric_limits<int>::max();








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

CoordinatorActor::~CoordinatorActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void CoordinatorActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case NEXTSLOT: {
                out << "nextSlot("
                << ")";
                break;
               }
    	}
}


void CoordinatorActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void CoordinatorActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"Coordinator.slot\" type=\"int\">" <<  
			    		((int)_ref_slot) << "</variable>" << endl;
}


char* CoordinatorActor::getClassName() {
    return (char*)"Coordinator";
}
void CoordinatorActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_slot, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

    
	
}
void CoordinatorActor::marshalActorToArray(byte* array) {
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

void CoordinatorActor::unmarshalActorFromArray(byte* array) {

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

void CoordinatorActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
                	memcpy(&_ref_slot, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
    
	
}

/*#ifdef DEBUG
void CoordinatorActor::printStateContent(long stateNo) {
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
long CoordinatorActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case NEXTSLOT:
                {
                    int i = 0;
                    return msgsrvnextSlot(
                    );
                }
    }
	return -1;
}

#define _RETURN_ return 0
    long CoordinatorActor::constructorCoordinator (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;CoordinatorActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Coordinator";
		#endif

	#ifdef TTS
	#endif

		{
			(_ref_slot=0);
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 70"), temp0)->_timed_msg_nextSlot(myID, _ref_now, MAX_TIME);
		}
		shift = 0;
		queueTail = 0;
		while(messageQueue[queueTail]) {applyPolicy(false);queueTail++;}
		return 0;

	}

#define _RETURN_ return


		void CoordinatorActor::_msg_nextSlot (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, NEXTSLOT);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** CoordinatorActor::table = (byte**)0xFFFFFFFF;

	byte** CoordinatorActor::getTable() {
		return table;
	}








	vector<TimeFrame>** CoordinatorActor::hashtableTimeExtension = (vector<TimeFrame>**)0xFFFFFFFF;
	
	vector<TimeFrame>** CoordinatorActor::getHashtableTimeExtension() {
		return hashtableTimeExtension;
	}








	    void CoordinatorActor::_timed_msg_nextSlot (byte senderId
, TIME_TYPE executionTime, TIME_TYPE deadline){
	    _msg_nextSlot (senderId
);
	    addTimedBundles(senderId, executionTime, deadline);
		}
	void CoordinatorActor::setNow(TIME_TYPE now) {
	
		AbstractTimedActor::setNow(now);
	}	
	

	#define _RETURN_ goto _RETURN_POINT_NEXTSLOT
	    long CoordinatorActor::msgsrvnextSlot (
    ) {
    			long arrayIndexChecker = 0;SatelliteActor *temp0;SatelliteActor *temp1;SatelliteActor *temp2;SatelliteActor *temp3;SatelliteActor *temp4;SatelliteActor *temp5;CoordinatorActor *temp6;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "nextSlot";
		#endif

	#ifdef TTS
	#endif

		{
			if ((_ref_slot==0)) {
				(temp0=_ref_s1, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 74"), temp0)->_timed_msg_grantGround(myID, _ref_now, MAX_TIME);}
			else {
				if ((_ref_slot==1)) {
					(temp1=_ref_s2, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 75"), temp1)->_timed_msg_grantGround(myID, _ref_now, MAX_TIME);}
				else {
					if ((_ref_slot==2)) {
						(temp2=_ref_s3, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 76"), temp2)->_timed_msg_grantGround(myID, _ref_now, MAX_TIME);}
					else {
						if ((_ref_slot==4)) {
							(temp3=_ref_s1, assertion(temp3!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 79"), temp3)->_timed_msg_grantISL(myID, _ref_s2, _ref_now, MAX_TIME);}
						else {
							if ((_ref_slot==5)) {
								(temp4=_ref_s2, assertion(temp4!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 80"), temp4)->_timed_msg_grantISL(myID, _ref_s3, _ref_now, MAX_TIME);}
							else {
								if ((_ref_slot==6)) {
									(temp5=_ref_s1, assertion(temp5!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 81"), temp5)->_timed_msg_grantISL(myID, _ref_s3, _ref_now, MAX_TIME);}}}}}}
			(_ref_slot=((_ref_slot+1)%8));
			(temp6=_ref_self, assertion(temp6!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 85"), temp6)->_timed_msg_nextSlot(myID, 1 + _ref_now, MAX_TIME);
		}
	_RETURN_POINT_NEXTSLOT:
		return 0;

}
