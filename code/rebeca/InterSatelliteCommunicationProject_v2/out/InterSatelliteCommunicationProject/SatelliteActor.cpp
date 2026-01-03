                                    


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
#define STARTSYSTEM 1
#define GENERATETRAFFIC 2
#define CHARGESLOT 3
#define RECEIVEISL 4
#define GRANTGROUND 5
#define GRANTISL 6


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)myID)]))

    	#define _ref_gs (dynamic_cast<GroundStationActor*>(rebecs[(int)((unsigned char)known[1])]))
    	#define _ref_stA (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[2])]))
    	#define _ref_stB (dynamic_cast<SatelliteActor*>(rebecs[(int)((unsigned char)known[3])]))
    	#define _ref_coord (dynamic_cast<CoordinatorActor*>(rebecs[(int)((unsigned char)known[4])]))
    
SatelliteActor::SatelliteActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
		#ifdef DEBUG_LEVEL_2 
			, ostream &out
		#endif







		,int stateSize, int hashmapSize
)
	:
		AbstractActor (myID, myName, maxQueueLength, maxParamLength, rebecsRef
			#ifdef DEBUG_LEVEL_2 
				, out
			#endif







		, stateSize, hashmapSize
		)
{
    messageNames = new char *[6 + 1];
    messagePriorities = new int[6 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"STARTSYSTEM";
			messagePriorities[1] = std::numeric_limits<int>::max();
       		 messageNames[2] = (char*)"GENERATETRAFFIC";
			messagePriorities[2] = std::numeric_limits<int>::max();
       		 messageNames[3] = (char*)"CHARGESLOT";
			messagePriorities[3] = std::numeric_limits<int>::max();
       		 messageNames[4] = (char*)"RECEIVEISL";
			messagePriorities[4] = std::numeric_limits<int>::max();
       		 messageNames[5] = (char*)"GRANTGROUND";
			messagePriorities[5] = std::numeric_limits<int>::max();
       		 messageNames[6] = (char*)"GRANTISL";
			messagePriorities[6] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
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
            case STARTSYSTEM: {
                out << "startSystem("
                << ")";
                break;
               }
            case GENERATETRAFFIC: {

            	int _ref_limit;
            
            
                	memcpy(&_ref_limit, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "generateTraffic("
                	<< ((int)_ref_limit)
                << ")";
                break;
               }
            case CHARGESLOT: {
                out << "chargeSlot("
                << ")";
                break;
               }
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
			    	out << tab << '\t' << "<variable name=\"Satellite.qType\" type=\"int[5]\">" <<  
			    		"[" << ((int)_ref_qType[0]) << ", " << ((int)_ref_qType[1]) << ", " << ((int)_ref_qType[2]) << ", " << ((int)_ref_qType[3]) << ", " << ((int)_ref_qType[4]) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.qSender\" type=\"int[5]\">" <<  
			    		"[" << ((int)_ref_qSender[0]) << ", " << ((int)_ref_qSender[1]) << ", " << ((int)_ref_qSender[2]) << ", " << ((int)_ref_qSender[3]) << ", " << ((int)_ref_qSender[4]) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.qPayload\" type=\"int[5]\">" <<  
			    		"[" << ((int)_ref_qPayload[0]) << ", " << ((int)_ref_qPayload[1]) << ", " << ((int)_ref_qPayload[2]) << ", " << ((int)_ref_qPayload[3]) << ", " << ((int)_ref_qPayload[4]) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.head\" type=\"int\">" <<  
			    		((int)_ref_head) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.tail\" type=\"int\">" <<  
			    		((int)_ref_tail) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.size\" type=\"int\">" <<  
			    		((int)_ref_size) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.typeCounter\" type=\"int\">" <<  
			    		((int)_ref_typeCounter) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.droppedCount\" type=\"int\">" <<  
			    		((int)_ref_droppedCount) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.energy\" type=\"int\">" <<  
			    		((int)_ref_energy) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.safeMode\" type=\"boolean\">" <<  
			    		(_ref_safeMode? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.SAFE_TH\" type=\"int\">" <<  
			    		((int)_ref_SAFE_TH) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.CRIT_TH\" type=\"int\">" <<  
			    		((int)_ref_CRIT_TH) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.COST_RX\" type=\"int\">" <<  
			    		((int)_ref_COST_RX) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.COST_ISL_TX\" type=\"int\">" <<  
			    		((int)_ref_COST_ISL_TX) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.COST_GS_TX\" type=\"int\">" <<  
			    		((int)_ref_COST_GS_TX) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.CHARGE_PER_SLOT\" type=\"int\">" <<  
			    		((int)_ref_CHARGE_PER_SLOT) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.MAX_ENERGY\" type=\"int\">" <<  
			    		((int)_ref_MAX_ENERGY) << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.energyOK\" type=\"boolean\">" <<  
			    		(_ref_energyOK? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.pending\" type=\"boolean\">" <<  
			    		(_ref_pending? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.bufOK\" type=\"boolean\">" <<  
			    		(_ref_bufOK? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.droppedEver\" type=\"boolean\">" <<  
			    		(_ref_droppedEver? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.sentGroundEver\" type=\"boolean\">" <<  
			    		(_ref_sentGroundEver? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.sentISLEver\" type=\"boolean\">" <<  
			    		(_ref_sentISLEver? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Satellite.BUF_CAP\" type=\"int\">" <<  
			    		((int)_ref_BUF_CAP) << "</variable>" << endl;
}


char* SatelliteActor::getClassName() {
    return (char*)"Satellite";
}
void SatelliteActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_id, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
					memcpy(&array [marshalIndex], _ref_qType.data(), (1 * INT_SIZE) * 5);					
					marshalIndex += (1 * INT_SIZE) * 5;
            
					memcpy(&array [marshalIndex], _ref_qSender.data(), (1 * INT_SIZE) * 5);					
					marshalIndex += (1 * INT_SIZE) * 5;
            
					memcpy(&array [marshalIndex], _ref_qPayload.data(), (1 * INT_SIZE) * 5);					
					marshalIndex += (1 * INT_SIZE) * 5;
            
                	memcpy(&array [marshalIndex], &_ref_head, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_tail, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_size, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_typeCounter, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_droppedCount, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_energy, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
                	memcpy(&array [marshalIndex], &_ref_SAFE_TH, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_CRIT_TH, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_COST_RX, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_COST_ISL_TX, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_COST_GS_TX, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_CHARGE_PER_SLOT, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
                	memcpy(&array [marshalIndex], &_ref_MAX_ENERGY, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
            
            
            
            
            
                	memcpy(&array [marshalIndex], &_ref_BUF_CAP, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_safeMode ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= (_ref_energyOK ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= (_ref_pending ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= (_ref_bufOK ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= (_ref_droppedEver ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= (_ref_sentGroundEver ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
array [marshalIndex] |= (_ref_sentISLEver ? 0xFF : 0x00) & AbstractActor::coeff[6]; 
++marshalIndex;    
	
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

            
            
					memcpy(_ref_qType.data(), &array [index], (1 * INT_SIZE) * 5);					
					index += (1 * INT_SIZE) * 5;

            
            
					memcpy(_ref_qSender.data(), &array [index], (1 * INT_SIZE) * 5);					
					index += (1 * INT_SIZE) * 5;

            
            
					memcpy(_ref_qPayload.data(), &array [index], (1 * INT_SIZE) * 5);					
					index += (1 * INT_SIZE) * 5;

            
            
                	memcpy(&_ref_head, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_tail, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_size, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_typeCounter, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_droppedCount, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_energy, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            

            
            
                	memcpy(&_ref_SAFE_TH, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_CRIT_TH, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_COST_RX, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_COST_ISL_TX, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_COST_GS_TX, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_CHARGE_PER_SLOT, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            
                	memcpy(&_ref_MAX_ENERGY, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            
            

            
            

            
            

            
            

            
            

            
            

            
            
                	memcpy(&_ref_BUF_CAP, &array [index], (1 * INT_SIZE));
	                index += (1 * INT_SIZE);
_ref_safeMode = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_energyOK = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_pending = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_bufOK = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_droppedEver = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_sentGroundEver = (bool) (array [index] & AbstractActor::coeff[5]);
_ref_sentISLEver = (bool) (array [index] & AbstractActor::coeff[6]);
++index;    
	
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

            case STARTSYSTEM:
                {
                    int i = 0;
                    return msgsrvstartSystem(
                    );
                }
            case GENERATETRAFFIC:
                {
                    int i = 0;

            	int _ref_limit;
            
            
                	memcpy(&_ref_limit, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvgenerateTraffic(
            _ref_limit
                    );
                }
            case CHARGESLOT:
                {
                    int i = 0;
                    return msgsrvchargeSlot(
                    );
                }
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
    			long arrayIndexChecker = 0;SatelliteActor *temp0;SatelliteActor *temp1;SatelliteActor *temp2;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Satellite";
		#endif

		{
			(_ref_id=_ref_myId);
			(_ref_head=0);
			(_ref_tail=0);
			(_ref_size=0);
			(_ref_typeCounter=0);
			(_ref_droppedCount=0);
			(_ref_SAFE_TH=20);
			(_ref_CRIT_TH=10);
			(_ref_COST_RX=5);
			(_ref_COST_ISL_TX=10);
			(_ref_COST_GS_TX=15);
			(_ref_CHARGE_PER_SLOT=8);
			(_ref_MAX_ENERGY=100);
			(_ref_energy=100);
			(_ref_safeMode=false);
			(_ref_energyOK=true);
			(_ref_BUF_CAP=5);
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 155"), temp0)->_msg_generateTraffic(myID, 3);
			(temp1=_ref_self, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 158"), temp1)->_synchmethod_updateLocalFlags();
			if ((_ref_id==1)) {
				{
					(temp2=_ref_self, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 162"), temp2)->_msg_startSystem(myID);
				}
}
		}
		shift = 0;
		return 0;

	}

#define _RETURN_ return
 
		void SatelliteActor::_synchmethod_enqueue (
    		int
	         _ref_type
	        , 
    		int
	         _ref_senderId
	        , 
    		int
	         _ref_payload
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "enqueue";
		#endif

		{
			if ((_ref_size<5)) {
				{
					(_ref_qType[(arrayIndexChecker=_ref_tail, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <5, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 191") , arrayIndexChecker)]=_ref_type);
					(_ref_qSender[(arrayIndexChecker=_ref_tail, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <5, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 192") , arrayIndexChecker)]=_ref_senderId);
					(_ref_qPayload[(arrayIndexChecker=_ref_tail, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <5, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 193") , arrayIndexChecker)]=_ref_payload);
					(_ref_tail=((_ref_tail+1)%5));
					(_ref_size=(_ref_size+1));
				}
}
			else {
				{
					(_ref_droppedCount=(_ref_droppedCount+1));
				}
}
			_synchmethod_updateLocalFlags();
		}

		}
 
		void SatelliteActor::_synchmethod_updateEnergyOK (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "updateEnergyOK";
		#endif

		{
			if ((_ref_energy>=_ref_SAFE_TH)) {
				(_ref_energyOK=true);}
			else {
				(_ref_energyOK=false);}
		}

		}
 
		void SatelliteActor::_synchmethod_updateLocalFlags (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "updateLocalFlags";
		#endif

		{
			if ((_ref_size==0)) {
				(_ref_pending=false);}
			else {
				(_ref_pending=true);}
			if ((_ref_size<=_ref_BUF_CAP)) {
				(_ref_bufOK=true);}
			else {
				(_ref_bufOK=false);}
			if ((_ref_droppedCount==0)) {
				(_ref_droppedEver=false);}
			else {
				(_ref_droppedEver=true);}
		}

		}
 
		void SatelliteActor::_synchmethod_updateSafeMode (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "updateSafeMode";
		#endif

		{
			if ((_ref_energy<=_ref_CRIT_TH)) {
				(_ref_safeMode=true);}
			if ((_ref_energy>_ref_SAFE_TH)) {
				(_ref_safeMode=false);}
		}

		}


		void SatelliteActor::_msg_startSystem (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, STARTSYSTEM);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void SatelliteActor::_msg_generateTraffic (byte senderId
	        ,
    		int
	         _ref_limit
  	  ){
        queueTail = 0;
        enqueue(messageQueue, GENERATETRAFFIC);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_limit, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

		void SatelliteActor::_msg_chargeSlot (byte senderId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, CHARGESLOT);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}

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

	#define _RETURN_ goto _RETURN_POINT_STARTSYSTEM
	    long SatelliteActor::msgsrvstartSystem (
    ) {
    			long arrayIndexChecker = 0;CoordinatorActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "startSystem";
		#endif

		{
			(temp0=_ref_coord, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 168"), temp0)->_msg_nextSlot(myID);
		}
	_RETURN_POINT_STARTSYSTEM:
		return 0;

}
	#define _RETURN_ goto _RETURN_POINT_GENERATETRAFFIC
	    long SatelliteActor::msgsrvgenerateTraffic (
    		int
	         _ref_limit
    ) {
    			long arrayIndexChecker = 0;SatelliteActor *temp0;SatelliteActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "generateTraffic";
		#endif

		{
			if ((_ref_limit>0)) {
				{
					(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 176"), temp0)->_synchmethod_enqueue((_ref_typeCounter%4), _ref_id, 100);
					(_ref_typeCounter=(_ref_typeCounter+1));
					(temp1=_ref_self, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 180"), temp1)->_msg_generateTraffic(myID, (_ref_limit-1));
				}
}
		}
	_RETURN_POINT_GENERATETRAFFIC:
		return 0;

}
	#define _RETURN_ goto _RETURN_POINT_CHARGESLOT
	    long SatelliteActor::msgsrvchargeSlot (
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "chargeSlot";
		#endif

		{
			if (((_ref_energy+_ref_CHARGE_PER_SLOT)<=_ref_MAX_ENERGY)) {
				(_ref_energy=(_ref_energy+_ref_CHARGE_PER_SLOT));}
			else {
				(_ref_energy=_ref_MAX_ENERGY);}
			_synchmethod_updateEnergyOK();
			_synchmethod_updateSafeMode();
			_synchmethod_updateLocalFlags();
		}
	_RETURN_POINT_CHARGESLOT:
		return 0;

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
    			long arrayIndexChecker = 0;SatelliteActor *temp0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "receiveISL";
		#endif

		{
			if ((_ref_energy>=_ref_COST_RX)) {
				(_ref_energy=(_ref_energy-_ref_COST_RX));}
			else {
				(_ref_energy=0);}
			_synchmethod_updateEnergyOK();
			_synchmethod_updateSafeMode();
			(temp0=_ref_self, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 267"), temp0)->_synchmethod_enqueue(_ref_type, _ref_sId, _ref_payload);
		}
	_RETURN_POINT_RECEIVEISL:
		return 0;

}
	#define _RETURN_ goto _RETURN_POINT_GRANTGROUND
	    long SatelliteActor::msgsrvgrantGround (
    ) {
    			long arrayIndexChecker = 0;GroundStationActor *temp0;CoordinatorActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "grantGround";
		#endif

		{
			if ((((!_ref_safeMode)&&(_ref_energy>=_ref_SAFE_TH))&&(_ref_size>0))) {
				{
					if ((_ref_energy>=_ref_COST_GS_TX)) {
						(_ref_energy=(_ref_energy-_ref_COST_GS_TX));}
					else {
						(_ref_energy=0);}
					_synchmethod_updateEnergyOK();
					_synchmethod_updateSafeMode();
					(temp0=_ref_gs, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 290"), temp0)->_msg_receiveFromSatellite(myID, _ref_id);
					(_ref_sentGroundEver=true);
					(_ref_head=((_ref_head+1)%5));
					(_ref_size=(_ref_size-1));
					_synchmethod_updateLocalFlags();
				}
}
			(temp1=_ref_coord, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 302"), temp1)->_msg_nextSlot(myID);
		}
	_RETURN_POINT_GRANTGROUND:
		return 0;

}
	#define _RETURN_ goto _RETURN_POINT_GRANTISL
	    long SatelliteActor::msgsrvgrantISL (
    		SatelliteActor*
	         _ref_target
    ) {
    			long arrayIndexChecker = 0;SatelliteActor *temp0;CoordinatorActor *temp1;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "grantISL";
		#endif

		{
			if ((((!_ref_safeMode)&&(_ref_energy>=_ref_SAFE_TH))&&(_ref_size>0))) {
				{
					if ((_ref_energy>=_ref_COST_ISL_TX)) {
						(_ref_energy=(_ref_energy-_ref_COST_ISL_TX));}
					else {
						(_ref_energy=0);}
					_synchmethod_updateEnergyOK();
					_synchmethod_updateSafeMode();
					(temp0=_ref_target, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 323"), temp0)->_msg_receiveISL(myID, _ref_qType[(arrayIndexChecker=_ref_head, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <5, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 323") , arrayIndexChecker)], _ref_id, _ref_qPayload[(arrayIndexChecker=_ref_head, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <5, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 323") , arrayIndexChecker)]);
					(_ref_sentISLEver=true);
					(_ref_head=((_ref_head+1)%5));
					(_ref_size=(_ref_size-1));
					_synchmethod_updateLocalFlags();
				}
}
			(temp1=_ref_coord, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 335"), temp1)->_msg_nextSlot(myID);
		}
	_RETURN_POINT_GRANTISL:
		return 0;

}
