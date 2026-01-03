#include "AbstractCoreRebecaAnalyzer.h"

		#define _ref_gs (dynamic_cast<GroundStationActor*>(rebecs[0]))
		#define _ref_coord (dynamic_cast<CoordinatorActor*>(rebecs[1]))
		#define _ref_s1 (dynamic_cast<SatelliteActor*>(rebecs[2]))
		#define _ref_s2 (dynamic_cast<SatelliteActor*>(rebecs[3]))
		#define _ref_s3 (dynamic_cast<SatelliteActor*>(rebecs[4]))
	
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1GroundSlot () {
			return _ref_coord->_ref_slotS1Ground;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2GroundSlot () {
			return _ref_coord->_ref_slotS2Ground;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3GroundSlot () {
			return _ref_coord->_ref_slotS3Ground;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1toS2Slot () {
			return _ref_coord->_ref_slotS1toS2;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2toS3Slot () {
			return _ref_coord->_ref_slotS2toS3;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1toS3Slot () {
			return _ref_coord->_ref_slotS1toS3;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1EnergyOK () {
			return _ref_s1->_ref_energyOK;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2EnergyOK () {
			return _ref_s2->_ref_energyOK;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3EnergyOK () {
			return _ref_s3->_ref_energyOK;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1Pending () {
			return _ref_s1->_ref_pending;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2Pending () {
			return _ref_s2->_ref_pending;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3Pending () {
			return _ref_s3->_ref_pending;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1BufOK () {
			return _ref_s1->_ref_bufOK;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2BufOK () {
			return _ref_s2->_ref_bufOK;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3BufOK () {
			return _ref_s3->_ref_bufOK;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1Dropped () {
			return _ref_s1->_ref_droppedEver;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2Dropped () {
			return _ref_s2->_ref_droppedEver;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3Dropped () {
			return _ref_s3->_ref_droppedEver;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1DeliveredToGS () {
			return _ref_gs->_ref_deliveredS1;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2DeliveredToGS () {
			return _ref_gs->_ref_deliveredS2;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3DeliveredToGS () {
			return _ref_gs->_ref_deliveredS3;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1ActualGroundSend () {
			return _ref_s1->_ref_sentGroundEver;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2ActualGroundSend () {
			return _ref_s2->_ref_sentGroundEver;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s3ActualGroundSend () {
			return _ref_s3->_ref_sentGroundEver;
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1ActualSendToS2 () {
			return (_ref_s1->_ref_sentISLEver&&_ref_coord->_ref_slotS1toS2);
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s2ActualSendToS3 () {
			return (_ref_s2->_ref_sentISLEver&&_ref_coord->_ref_slotS2toS3);
		}
		boolean AbstractCoreRebecaAnalyzer::_propDef_s1ActualSendToS3 () {
			return (_ref_s1->_ref_sentISLEver&&_ref_coord->_ref_slotS1toS3);
		}
	
		#define _ref_s1GroundSlot _propDef_s1GroundSlot()
		#define _ref_s2GroundSlot _propDef_s2GroundSlot()
		#define _ref_s3GroundSlot _propDef_s3GroundSlot()
		#define _ref_s1toS2Slot _propDef_s1toS2Slot()
		#define _ref_s2toS3Slot _propDef_s2toS3Slot()
		#define _ref_s1toS3Slot _propDef_s1toS3Slot()
		#define _ref_s1EnergyOK _propDef_s1EnergyOK()
		#define _ref_s2EnergyOK _propDef_s2EnergyOK()
		#define _ref_s3EnergyOK _propDef_s3EnergyOK()
		#define _ref_s1Pending _propDef_s1Pending()
		#define _ref_s2Pending _propDef_s2Pending()
		#define _ref_s3Pending _propDef_s3Pending()
		#define _ref_s1BufOK _propDef_s1BufOK()
		#define _ref_s2BufOK _propDef_s2BufOK()
		#define _ref_s3BufOK _propDef_s3BufOK()
		#define _ref_s1Dropped _propDef_s1Dropped()
		#define _ref_s2Dropped _propDef_s2Dropped()
		#define _ref_s3Dropped _propDef_s3Dropped()
		#define _ref_s1DeliveredToGS _propDef_s1DeliveredToGS()
		#define _ref_s2DeliveredToGS _propDef_s2DeliveredToGS()
		#define _ref_s3DeliveredToGS _propDef_s3DeliveredToGS()
		#define _ref_s1ActualGroundSend _propDef_s1ActualGroundSend()
		#define _ref_s2ActualGroundSend _propDef_s2ActualGroundSend()
		#define _ref_s3ActualGroundSend _propDef_s3ActualGroundSend()
		#define _ref_s1ActualSendToS2 _propDef_s1ActualSendToS2()
		#define _ref_s2ActualSendToS3 _propDef_s2ActualSendToS3()
		#define _ref_s1ActualSendToS3 _propDef_s1ActualSendToS3()
		#undef _ref_s1GroundSlot
		#undef _ref_s2GroundSlot
		#undef _ref_s3GroundSlot
		#undef _ref_s1toS2Slot
		#undef _ref_s2toS3Slot
		#undef _ref_s1toS3Slot
		#undef _ref_s1EnergyOK
		#undef _ref_s2EnergyOK
		#undef _ref_s3EnergyOK
		#undef _ref_s1Pending
		#undef _ref_s2Pending
		#undef _ref_s3Pending
		#undef _ref_s1BufOK
		#undef _ref_s2BufOK
		#undef _ref_s3BufOK
		#undef _ref_s1Dropped
		#undef _ref_s2Dropped
		#undef _ref_s3Dropped
		#undef _ref_s1DeliveredToGS
		#undef _ref_s2DeliveredToGS
		#undef _ref_s3DeliveredToGS
		#undef _ref_s1ActualGroundSend
		#undef _ref_s2ActualGroundSend
		#undef _ref_s3ActualGroundSend
		#undef _ref_s1ActualSendToS2
		#undef _ref_s2ActualSendToS3
		#undef _ref_s1ActualSendToS3

		#undef _ref_gs
		#undef _ref_coord
		#undef _ref_s1
		#undef _ref_s2
		#undef _ref_s3

AbstractCoreRebecaAnalyzer::AbstractCoreRebecaAnalyzer(ostream &outStream, int hashmapSize) : analysisOutputStream(outStream){
	this->hashmapSize = hashmapSize;
	this->numberOfTransitions = 0;
	this->numberOfStates = 0;
}

void AbstractCoreRebecaAnalyzer::unmarshalRebecs(long* node) {
	int i;
	for (i = 0; i < REBEC_COUNT; i++) {
        rebecs[i]->unmarshalActor(node[i]);   // uncompress all states of all rebecs
    }
}

void AbstractCoreRebecaAnalyzer::marshalRebecs(long* node) {
	int i;
	for (i=0; i<REBEC_COUNT; i++){
		node[i] = rebecs[i]->marshalActor();
	}
}

State* AbstractCoreRebecaAnalyzer::storeRecentlyCreatedState (byte &result, short int claim, short int fairnessCounter) {

    numberOfTransitions++;
    State *st;
    long sys[REBEC_COUNT];
	marshalRebecs(sys);
	result = NEW_STATE;
	if (st = storage->exists (sys, claim)){
		if (st->isOnStack()){
			result |= ON_STACK;	// used in detecting cycle & stack proviso
		}
		if (st->isVisited(fairnessCounter)){
			result &= ~NEW_STATE;
		}
	}
	else
	{
		st = storage->put(sys, claim);	//create the new State
		if (st == NULL) {
	        throw Exception("Not enough memory for continuing the state space generation.");
		}
	    st->stateID = numberOfStates++;
	    #ifdef DEBUG_LEVEL_2
			cout << "State stored successfully with id: " << st->stateID << "." << endl;
		#endif
	}
	return st;
}

void AbstractCoreRebecaAnalyzer::instantiationPart() {
	vector<AbstractActor*>* referenceToRebecs = new vector<AbstractActor*>();
	byte maxRebecID = 0;
    	
    	GroundStationActor* _ref_gs = new GroundStationActor(maxRebecID++, "gs", 15, 4, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 103
    											, hashmapSize
    											);
        rebecs.push_back(_ref_gs);
        referenceToRebecs->push_back(_ref_gs);
    	
    	CoordinatorActor* _ref_coord = new CoordinatorActor(maxRebecID++, "coord", 15, 0, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 35
    											, hashmapSize
    											);
        rebecs.push_back(_ref_coord);
        referenceToRebecs->push_back(_ref_coord);
    	
    	SatelliteActor* _ref_s1 = new SatelliteActor(maxRebecID++, "s1", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 401
    											, hashmapSize
    											);
        rebecs.push_back(_ref_s1);
        referenceToRebecs->push_back(_ref_s1);
    	
    	SatelliteActor* _ref_s2 = new SatelliteActor(maxRebecID++, "s2", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 401
    											, hashmapSize
    											);
        rebecs.push_back(_ref_s2);
        referenceToRebecs->push_back(_ref_s2);
    	
    	SatelliteActor* _ref_s3 = new SatelliteActor(maxRebecID++, "s3", 20, 12, *referenceToRebecs 
    											#ifdef DEBUG_LEVEL_2 
    											, analysisOutputStream 
    											#endif
    											, 401
    											, hashmapSize
    											);
        rebecs.push_back(_ref_s3);
        referenceToRebecs->push_back(_ref_s3);

    	_ref_gs->known = new byte[MAX_KNOWN];
    	_ref_gs->known[0] = _ref_gs->myID;

		_ref_gs->constructorGroundStation(INITIAL_SENDER
		);
    	_ref_coord->known = new byte[MAX_KNOWN];
    	_ref_coord->known[0] = _ref_coord->myID;
 _ref_coord->known[1] = _ref_s1->myID;
 _ref_coord->known[2] = _ref_s2->myID;
 _ref_coord->known[3] = _ref_s3->myID;

		_ref_coord->constructorCoordinator(INITIAL_SENDER
		);
    	_ref_s1->known = new byte[MAX_KNOWN];
    	_ref_s1->known[0] = _ref_s1->myID;
 _ref_s1->known[1] = _ref_gs->myID;
 _ref_s1->known[2] = _ref_s2->myID;
 _ref_s1->known[3] = _ref_s3->myID;
 _ref_s1->known[4] = _ref_coord->myID;

		_ref_s1->constructorSatellite(INITIAL_SENDER
	        , 1
		);
    	_ref_s2->known = new byte[MAX_KNOWN];
    	_ref_s2->known[0] = _ref_s2->myID;
 _ref_s2->known[1] = _ref_gs->myID;
 _ref_s2->known[2] = _ref_s1->myID;
 _ref_s2->known[3] = _ref_s3->myID;
 _ref_s2->known[4] = _ref_coord->myID;

		_ref_s2->constructorSatellite(INITIAL_SENDER
	        , 2
		);
    	_ref_s3->known = new byte[MAX_KNOWN];
    	_ref_s3->known[0] = _ref_s3->myID;
 _ref_s3->known[1] = _ref_gs->myID;
 _ref_s3->known[2] = _ref_s1->myID;
 _ref_s3->known[3] = _ref_s2->myID;
 _ref_s3->known[4] = _ref_coord->myID;

		_ref_s3->constructorSatellite(INITIAL_SENDER
	        , 3
		);

	numberOfStates = 0;
	numberOfTransitions = 0;

    #ifdef DEBUG
    	analysisOutputStream << "Rebecs are instanciated successfully." << endl;
    #endif

}

void AbstractCoreRebecaAnalyzer::exportState(State* current, ostream& outStream) {
	outStream << "<state id=\"" << current->stateID << "\" "; 
	outStream << "atomicpropositions=\""; 
		if (_propDef_s1GroundSlot ())
			outStream << "s1GroundSlot,"; 
		if (_propDef_s2GroundSlot ())
			outStream << "s2GroundSlot,"; 
		if (_propDef_s3GroundSlot ())
			outStream << "s3GroundSlot,"; 
		if (_propDef_s1toS2Slot ())
			outStream << "s1toS2Slot,"; 
		if (_propDef_s2toS3Slot ())
			outStream << "s2toS3Slot,"; 
		if (_propDef_s1toS3Slot ())
			outStream << "s1toS3Slot,"; 
		if (_propDef_s1EnergyOK ())
			outStream << "s1EnergyOK,"; 
		if (_propDef_s2EnergyOK ())
			outStream << "s2EnergyOK,"; 
		if (_propDef_s3EnergyOK ())
			outStream << "s3EnergyOK,"; 
		if (_propDef_s1Pending ())
			outStream << "s1Pending,"; 
		if (_propDef_s2Pending ())
			outStream << "s2Pending,"; 
		if (_propDef_s3Pending ())
			outStream << "s3Pending,"; 
		if (_propDef_s1BufOK ())
			outStream << "s1BufOK,"; 
		if (_propDef_s2BufOK ())
			outStream << "s2BufOK,"; 
		if (_propDef_s3BufOK ())
			outStream << "s3BufOK,"; 
		if (_propDef_s1Dropped ())
			outStream << "s1Dropped,"; 
		if (_propDef_s2Dropped ())
			outStream << "s2Dropped,"; 
		if (_propDef_s3Dropped ())
			outStream << "s3Dropped,"; 
		if (_propDef_s1DeliveredToGS ())
			outStream << "s1DeliveredToGS,"; 
		if (_propDef_s2DeliveredToGS ())
			outStream << "s2DeliveredToGS,"; 
		if (_propDef_s3DeliveredToGS ())
			outStream << "s3DeliveredToGS,"; 
		if (_propDef_s1ActualGroundSend ())
			outStream << "s1ActualGroundSend,"; 
		if (_propDef_s2ActualGroundSend ())
			outStream << "s2ActualGroundSend,"; 
		if (_propDef_s3ActualGroundSend ())
			outStream << "s3ActualGroundSend,"; 
		if (_propDef_s1ActualSendToS2 ())
			outStream << "s1ActualSendToS2,"; 
		if (_propDef_s2ActualSendToS3 ())
			outStream << "s2ActualSendToS3,"; 
		if (_propDef_s1ActualSendToS3 ())
			outStream << "s1ActualSendToS3,"; 
	outStream << "\" ";
	outStream << ">" << endl;
	for (int cnt = 0; cnt < REBEC_COUNT; cnt++)
		rebecs[cnt]->exportStateInXML(outStream, "\t");
	outStream << "</state>" << endl;
}

void AbstractCoreRebecaAnalyzer::exportTransition(State* source, State* destination, 
		string sender, string owner, string label, ostream& outStream) {
	exportTransition(to_string(source->stateID), to_string(destination->stateID), sender, owner, label, outStream);  
}

void AbstractCoreRebecaAnalyzer::exportTransition(string source, string destination, 
		string sender, string owner, string label, ostream& outStream) {
    outStream << "<transition source=\"" << source << "\" destination=\"" << 
		destination << "\"> <messageserver sender=\"" << sender << 
		"\" owner=\"" << owner << "\" title=\"" << label << "\"/></transition>" << endl;
}
