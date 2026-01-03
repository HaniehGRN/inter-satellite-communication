                                    


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

	
		
	
		
       		 messageNames[1] = (char*)"NEXTSLOT";
			messagePriorities[1] = std::numeric_limits<int>::max();








	if (table == (byte**)0xFFFFFFFF) {	// avoid reinitialization
		table = new byte* [1 << hashmapSize];
        memset (table, 0, sizeof(byte*)*(1 << hashmapSize));
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
			    	out << tab << '\t' << "<variable name=\"Coordinator.slotS1Ground\" type=\"boolean\">" <<  
			    		(_ref_slotS1Ground? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Coordinator.slotS2Ground\" type=\"boolean\">" <<  
			    		(_ref_slotS2Ground? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Coordinator.slotS3Ground\" type=\"boolean\">" <<  
			    		(_ref_slotS3Ground? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Coordinator.slotS1toS2\" type=\"boolean\">" <<  
			    		(_ref_slotS1toS2? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Coordinator.slotS2toS3\" type=\"boolean\">" <<  
			    		(_ref_slotS2toS3? "true" : "false") << "</variable>" << endl;
			    	out << tab << '\t' << "<variable name=\"Coordinator.slotS1toS3\" type=\"boolean\">" <<  
			    		(_ref_slotS1toS3? "true" : "false") << "</variable>" << endl;
}


char* CoordinatorActor::getClassName() {
    return (char*)"Coordinator";
}
void CoordinatorActor::marshalVariablesToArray(byte * array){
	int marshalIndex = 0;
    	
            
                	memcpy(&array [marshalIndex], &_ref_slot, (1 * INT_SIZE));
	                marshalIndex += (1 * INT_SIZE);
            
            
            
            
            
            

array [marshalIndex] = 0;
array [marshalIndex] |= (_ref_slotS1Ground ? 0xFF : 0x00) & AbstractActor::coeff[0]; 
array [marshalIndex] |= (_ref_slotS2Ground ? 0xFF : 0x00) & AbstractActor::coeff[1]; 
array [marshalIndex] |= (_ref_slotS3Ground ? 0xFF : 0x00) & AbstractActor::coeff[2]; 
array [marshalIndex] |= (_ref_slotS1toS2 ? 0xFF : 0x00) & AbstractActor::coeff[3]; 
array [marshalIndex] |= (_ref_slotS2toS3 ? 0xFF : 0x00) & AbstractActor::coeff[4]; 
array [marshalIndex] |= (_ref_slotS1toS3 ? 0xFF : 0x00) & AbstractActor::coeff[5]; 
++marshalIndex;    
	
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

            
            

            
            

            
            

            
            

            
            

            
            
_ref_slotS1Ground = (bool) (array [index] & AbstractActor::coeff[0]);
_ref_slotS2Ground = (bool) (array [index] & AbstractActor::coeff[1]);
_ref_slotS3Ground = (bool) (array [index] & AbstractActor::coeff[2]);
_ref_slotS1toS2 = (bool) (array [index] & AbstractActor::coeff[3]);
_ref_slotS2toS3 = (bool) (array [index] & AbstractActor::coeff[4]);
_ref_slotS1toS3 = (bool) (array [index] & AbstractActor::coeff[5]);
++index;    
	
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
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "Coordinator";
		#endif

		{
			(_ref_slot=(-1));
			(_ref_slotS1Ground=false);
			(_ref_slotS2Ground=false);
			(_ref_slotS3Ground=false);
			(_ref_slotS1toS2=false);
			(_ref_slotS2toS3=false);
			(_ref_slotS1toS3=false);
		}
		shift = 0;
		return 0;

	}

#define _RETURN_ return
 
		void CoordinatorActor::_synchmethod_updateSlotFlags (
   		 ) {
    			long arrayIndexChecker = 0;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "updateSlotFlags";
		#endif

		{
			(_ref_slotS1Ground=(_ref_slot==0));
			(_ref_slotS2Ground=(_ref_slot==1));
			(_ref_slotS3Ground=(_ref_slot==2));
			(_ref_slotS1toS2=(_ref_slot==4));
			(_ref_slotS2toS3=(_ref_slot==5));
			(_ref_slotS1toS3=(_ref_slot==6));
		}

		}


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

	#define _RETURN_ goto _RETURN_POINT_NEXTSLOT
	    long CoordinatorActor::msgsrvnextSlot (
    ) {
    			long arrayIndexChecker = 0;SatelliteActor *temp0;SatelliteActor *temp1;SatelliteActor *temp2;SatelliteActor *temp3;SatelliteActor *temp4;SatelliteActor *temp5;CoordinatorActor *temp6;SatelliteActor *temp7;SatelliteActor *temp8;SatelliteActor *temp9;CoordinatorActor *temp10;
		shift = 1;
		#ifdef SAFE_MODE
			string reactiveClassName = this->getName();
			string methodName = "nextSlot";
		#endif

		{
			(_ref_slot=((_ref_slot+1)%8));
			_synchmethod_updateSlotFlags();
			(temp0=_ref_s1, assertion(temp0!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 404"), temp0)->_msg_chargeSlot(myID);
			(temp1=_ref_s2, assertion(temp1!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 405"), temp1)->_msg_chargeSlot(myID);
			(temp2=_ref_s3, assertion(temp2!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 406"), temp2)->_msg_chargeSlot(myID);
			if ((_ref_slot==0)) {
				(temp3=_ref_s1, assertion(temp3!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 409"), temp3)->_msg_grantGround(myID);}
			else {
				if ((_ref_slot==1)) {
					(temp4=_ref_s2, assertion(temp4!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 410"), temp4)->_msg_grantGround(myID);}
				else {
					if ((_ref_slot==2)) {
						(temp5=_ref_s3, assertion(temp5!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 411"), temp5)->_msg_grantGround(myID);}
					else {
						if ((_ref_slot==3)) {
							{
								(temp6=_ref_self, assertion(temp6!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 415"), temp6)->_msg_nextSlot(myID);
							}
}
						else {
							if ((_ref_slot==4)) {
								(temp7=_ref_s1, assertion(temp7!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 419"), temp7)->_msg_grantISL(myID, _ref_s2);}
							else {
								if ((_ref_slot==5)) {
									(temp8=_ref_s2, assertion(temp8!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 420"), temp8)->_msg_grantISL(myID, _ref_s3);}
								else {
									if ((_ref_slot==6)) {
										(temp9=_ref_s1, assertion(temp9!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 421"), temp9)->_msg_grantISL(myID, _ref_s3);}
									else {
										if ((_ref_slot==7)) {
											{
												(temp10=_ref_self, assertion(temp10!= null, "Null Pointer Exception in method " + reactiveClassName + "." + methodName + "line 425"), temp10)->_msg_nextSlot(myID);
											}
}}}}}}}}
		}
	_RETURN_POINT_NEXTSLOT:
		return 0;

}
