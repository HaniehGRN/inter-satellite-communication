                                    


#include "Types.h"
#include "GroundStationActor.h"
#include <stdio.h>
#include <string.h>
#include <limits>

#include "SatelliteActor.h"
#include "CoordinatorActor.h"

// location counters (values stored in messageQueue[0])
/* There can be more than one location counter for each msgsrv when we
   want to have non-atomic message servers.
   Notice that values 0 and 1 are reserved for EMPTY and INITIAL, respectively.*/
#define RECEIVEFROMSATELLITE 1


// knownrebecs (those to whom message can be sent)
// the following format allows to use them as variables --> the ID of the rebec
#define _ref_sender (rebecs[senderQueue[0]])
#define _ref_self (dynamic_cast<GroundStationActor*>(rebecs[(int)((unsigned char)myID)]))

    
GroundStationActor::GroundStationActor(byte myID, char* myName, int maxQueueLength, int maxParamLength, vector<AbstractActor*>& rebecsRef
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
    messageNames = new char *[1 + 1];
    messagePriorities = new int[1 + 1];
	messageNames[0] = (char*)"Empty";

	
		
	
		
       		 messageNames[1] = (char*)"RECEIVEFROMSATELLITE";
			messagePriorities[1] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
	}

	
	queueTail = 0;
}

GroundStationActor::~GroundStationActor (){
	delete [] messageNames;
	delete [] messageQueue;
	delete [] senderQueue;
}


void GroundStationActor::exportAMessageInXML(ostream &out, string tab, int cnt) {
        int i = 0;
        switch (messageQueue[cnt]) {
            case RECEIVEFROMSATELLITE: {

            	int _ref_satId;
            
            
                	memcpy(&_ref_satId, &paramQueue[cnt] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                out << "receiveFromSatellite("
                	<< ((int)_ref_satId)
                << ")";
                break;
               }
    	}
}


void GroundStationActor::exportStateVariablesInXML(ostream &out, string tab) {
	out << tab << "<statevariables>" << endl;
	exportInsideStateVariablesTagInXML(out, tab);
	out << tab << "</statevariables>" << endl;
}

void GroundStationActor::exportInsideStateVariablesTagInXML(ostream &out, string tab){


			    	out << tab << '\t' << "<variable name=\"GroundStation.messageCount\" type=\"int[3]\">" <<  
			    		"[" << ((int)_ref_messageCount[0]) << ", " << ((int)_ref_messageCount[1]) << ", " << ((int)_ref_messageCount[2]) << ", " << "]" << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"GroundStation.deliveredS1\" type=\"boolean\">" <<  
			    		(_ref_deliveredS1? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"GroundStation.deliveredS2\" type=\"boolean\">" <<  
			    		(_ref_deliveredS2? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"GroundStation.deliveredS3\" type=\"boolean\">" <<  
			    		(_ref_deliveredS3? "true" : "false") << "</variable>" << endl;
}


char* GroundStationActor::getClassName() {
    return (char*)"GroundStation";
}
void GroundStationActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
					memcpy(&array [marshalIndex], _ref_messageCount.data(), (1 * INT_SIZE) * 3);					
					marshalIndex += (1 * INT_SIZE) * 3;
            
            
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_deliveredS1 ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= (_ref_deliveredS2 ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= (_ref_deliveredS3 ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
++marshalIndex;    
	
}
void GroundStationActor::marshalActorToArray(byte* array) {
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

void GroundStationActor::unmarshalActorFromArray(byte* array) {

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

void GroundStationActor::unmarshalVariablesFromArray(byte * array){

	int index = 0;

            
            
					memcpy(_ref_messageCount.data(), &array [index], (1 * INT_SIZE) * 3);					
					index += (1 * INT_SIZE) * 3;

            
            

            
            

            
            
_ref_deliveredS1 = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_deliveredS2 = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_deliveredS3 = (bool) (array [index] & AbstractActor::coeff[2]);
++index;    
	
}

/*#ifdef DEBUG
void GroundStationActor::printStateContent(long stateNo) {
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
long GroundStationActor::methodSelectorAndExecutor() {

	
    switch (messageQueue[0]) {

            case RECEIVEFROMSATELLITE:
                {
                    int i = 0;

            	int _ref_satId;
            
            
                	memcpy(&_ref_satId, &paramQueue[0] [i], (1 * INT_SIZE));
	                i += (1 * INT_SIZE);
                    return msgsrvreceiveFromSatellite(
            _ref_satId
                    );
                }
    }
	return -1;
}

#define _RETURN_ return 0
    long GroundStationActor::constructorGroundStation (byte creatorId 
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "GroundStation";
		#endif

		{
			(_ref_messageCount[(arrayIndexChecker=0, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 22") , arrayIndexChecker)]=0);
			(_ref_messageCount[(arrayIndexChecker=1, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 23") , arrayIndexChecker)]=0);
			(_ref_messageCount[(arrayIndexChecker=2, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 24") , arrayIndexChecker)]=0);
			(_ref_deliveredS1=false);
			(_ref_deliveredS2=false);
			(_ref_deliveredS3=false);
		}
		shift = 0;
		return 0;

	}

#define _RETURN_ return


		void GroundStationActor::_msg_receiveFromSatellite (byte senderId
	        ,
    		int
	         _ref_satId
  	  ){
        queueTail = 0;
        enqueue(messageQueue, RECEIVEFROMSATELLITE);
        enqueue(senderQueue, senderId);

        if (maxParamLength != 0) {
            int index = 0;
            byte paramsValues[maxParamLength];
            memset(paramsValues, 0, maxParamLength);
    	
            
                	memcpy(&paramsValues [index], &_ref_satId, (1 * INT_SIZE));
	                index += (1 * INT_SIZE);

            enqueue(paramQueue, paramsValues, maxParamLength);
        }
}









	byte** GroundStationActor::table = (byte**)0xFFFFFFFF;

	byte** GroundStationActor::getTable() {
		return table;
	}

	#define _RETURN_ goto _RETURN_POINT_RECEIVEFROMSATELLITE
	    long GroundStationActor::msgsrvreceiveFromSatellite (
    		int
	         _ref_satId
    ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "receiveFromSatellite";
		#endif

		{
			if ((_ref_satId==1)) {
				{
					(_ref_messageCount[(arrayIndexChecker=0, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 34") , arrayIndexChecker)]=(_ref_messageCount[(arrayIndexChecker=0, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 34") , arrayIndexChecker)]+1));
					(_ref_deliveredS1=true);
				}
}
			else {
				if ((_ref_satId==2)) {
					{
						(_ref_messageCount[(arrayIndexChecker=1, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 38") , arrayIndexChecker)]=(_ref_messageCount[(arrayIndexChecker=1, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 38") , arrayIndexChecker)]+1));
						(_ref_deliveredS2=true);
					}
}
				else {
					if ((_ref_satId==3)) {
						{
							(_ref_messageCount[(arrayIndexChecker=2, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 42") , arrayIndexChecker)]=(_ref_messageCount[(arrayIndexChecker=2, assertion(arrayIndexChecker >= 0 && arrayIndexChecker <3, string("Array index out of bound: ") + to_string((long long)arrayIndexChecker) + ", method \"" + reactiveClassName + "." + methodName + "\" line 42") , arrayIndexChecker)]+1));
							(_ref_deliveredS3=true);
						}
}}}
		}
	_RETURN_POINT_RECEIVEFROMSATELLITE:
		return 0;

}
