
//  .............define constant values............. 

#define N 8
#define buffer_cap 5
#define MAX_ENERGY 100
#define SAFE_ENERGY 20
#define CRITICAL_ENERGY 10
#define SEND_ISL_COST 10
#define SEND_GROUND_COST 15
#define RECEIVE_COST 5
#define SOLAR_CHARGE 8
#define satellite_num 3


//  .............define enumerations.............  

mtype = {IMAGE, COMMAND, TELEMETRY, ACK, NONE}; // NONE to empty an element

//  .............define custom types.............

typedef MESSAGE {
    mtype message_type;
    int sender_satellite_id;
    int receiver_satellite_id;
    int payload;
}

//  .............define channels.............

chan time_signal = [1] of {int};
chan grant_ground[satellite_num] = [1] of {int};
chan grant_isl[satellite_num] = [1] of {int};
chan message_sent_to_ground = [3] of {int, int, int};
chan ISL[satellite_num] = [2] of {mtype, int, int, int};

//  .............define variables.............

int current_slot;
int message_counter[4] = {0, 0, 0, 0};
int slot = -1;
int message_num_per_satellite[satellite_num] = {0, 0, 0};
int energy_satellite[satellite_num] = {100, 100, 100};
bool safe_mode[satellite_num] = {false, false, false};
int tail_sat1 = 0;
int head_sat1 = 0;
int tail_sat2 = 0;
int head_sat2 = 0;
int tail_sat3 = 0;
int head_sat3 = 0;

//  .............define LTL conditions.............

/* Ensure that whenever there are messages in the buffer (tail != head), 
   eventually all messages are processed (tail catches up to head)*/

// Always: if satellite 1's buffer is not empty, it will eventually be processed and become empty   
ltl sat1_processed { [](tail_sat1 != head_sat1 -> <> (tail_sat1 == head_sat1)) }

// Always: if satellite 2's buffer is not empty, it will eventually be processed and become empty
ltl sat2_processed { [](tail_sat2 != head_sat2 -> <> (tail_sat2 == head_sat2)) }

// Always: if satellite 3's buffer is not empty, it will eventually be processed and become empty
ltl sat3_processed { [](tail_sat3 != head_sat3 -> <> (tail_sat3 == head_sat3)) }


/* Ensure that the ground station grant signal for each satellite is given infinitely often,
   i.e., each satellite periodically gets access to send data to the ground*/

// Always: satellite 1 will eventually receive permission to send to the ground (periodic ground access)   
ltl periodic_access_0 { [](<> grant_ground[0]) }

// Always: satellite 2 will eventually receive permission to send to the ground (periodic ground access)
ltl periodic_access_1 { [](<> grant_ground[1]) }

// Always: satellite 3 will eventually receive permission to send to the ground (periodic ground access)
ltl periodic_access_2 { [](<> grant_ground[2]) }

//  .............define processes.............

proctype timekeeper()
{
    atomic {
        if
        :: time_signal ! current_slot -> 
           current_slot = (current_slot + 1) % N;

            // Recharge satellite energy from solar panel each time slot
            int i = 0;
            do
            :: i < satellite_num ->
                if
                :: (energy_satellite[i] + SOLAR_CHARGE) <= MAX_ENERGY ->
                    energy_satellite[i] = energy_satellite[i] + SOLAR_CHARGE;
                :: else ->
                    energy_satellite[i] = MAX_ENERGY;
                fi;                

                if
                :: safe_mode[i] && energy_satellite[i] >= SAFE_ENERGY ->
                    safe_mode[i] = false;
                    printf("satellite(%d) exiting Safe Mode, energy: %d\n", i+1, energy_satellite[i]);
                :: else -> skip
                fi;
                i = i + 1
            :: else -> break   
            od
        fi
    }
}

proctype coordinator()
{
    if 
    :: time_signal ? slot -> 
        if
        :: slot == 0 -> grant_ground[0] ! 1; 
        :: slot == 1 -> grant_ground[1] ! 1; 
        :: slot == 2 -> grant_ground[2] ! 1;
        :: slot == 3 -> printf("Synchronization slot\n");
        :: slot == 4 -> grant_isl[0] ! 12; 
        :: slot == 5 -> grant_isl[1] ! 23;
        :: slot == 6 -> grant_isl[2] ! 13;
        :: slot == 7 -> printf("Synchronization slot\n");
        fi
    fi
}

proctype satellite1()
{
    MESSAGE buff[buffer_cap], temp_message_receive, temp_message_send;
    tail_sat1 = 0;
    head_sat1 = 0;
    bool is_turn_send_ground = false;
    int is_turn_send_isl12 = 0;
    int is_turn_send_isl13 = 0;

//  .............receiving phase.............
    
        do
    :: ISL[0] ? temp_message_receive -> 
        
        // Ensure buffer is not full before adding a new message
        assert( (tail_sat1 + 1) % buffer_cap != head_sat1 );

        buff[tail_sat1].message_type = temp_message_receive.message_type;
        buff[tail_sat1].sender_satellite_id = temp_message_receive.sender_satellite_id;
        buff[tail_sat1].receiver_satellite_id = temp_message_receive.receiver_satellite_id;
        buff[tail_sat1].payload = temp_message_receive.payload;

        printf("satellite(1) buffered message {type: %d, sender : %d, receiver: %d, payload: %d}\n", 
            buff[tail_sat1].message_type, buff[tail_sat1].sender_satellite_id, 
            buff[tail_sat1].receiver_satellite_id, buff[tail_sat1].payload);

        tail_sat1 = (tail_sat1 + 1) % buffer_cap;

        /* Energy consumption for receiving data */
        if
        :: energy_satellite[0] >= RECEIVE_COST ->
               energy_satellite[0] = energy_satellite[0] - RECEIVE_COST;
               printf("satellite(1) received a message, energy now: %d\n", energy_satellite[0]);
        :: else ->
               safe_mode[0] = true;
               printf("satellite(1) entering Safe Mode due to low energy on receive (%d)\n", energy_satellite[0]);
        fi;
    :: skip -> goto sendingPhase1  
    od

//  .............sending phase.............
    
//  .............sending phase.............
    
sendingPhase1:    
        if
        :: tail_sat1 != head_sat1 -> 
            temp_message_send.message_type = buff[head_sat1].message_type;
            temp_message_send.sender_satellite_id = buff[head_sat1].sender_satellite_id;
            temp_message_send.receiver_satellite_id = buff[head_sat1].receiver_satellite_id;
            temp_message_send.payload = buff[head_sat1].payload;

            // Only attempt to send if energy is above SAFE_ENERGY and not in Safe Mode
            if
            :: energy_satellite[0] >= SAFE_ENERGY && !safe_mode[0] ->
                if 
                :: grant_ground[0] ? is_turn_send_ground -> 
                    if
                    :: is_turn_send_ground -> 
                        printf("satellite(1) is sending to the ground\n");

                        /* Ensure satellite has at least SAFE_ENERGY before sending */                       
                        assert(energy_satellite[0] >= SAFE_ENERGY)

                        if
                        :: message_sent_to_ground ! 1 -> 

                           /* Ensure only one message is in the ground station channel at a time*/
                           assert(len(message_sent_to_ground) <= 1);

                           printf("satellite(1) sent to the ground \n");

                           buff[head_sat1].message_type = NONE;
                           buff[head_sat1].sender_satellite_id = -1;
                           buff[head_sat1].receiver_satellite_id = -1;
                           buff[head_sat1].payload = -1;
                           head_sat1 = (head_sat1 + 1) % buffer_cap;

                           /* Energy consumption for ground sending */
                            if
                            :: energy_satellite[0] >= SEND_GROUND_COST ->
                                energy_satellite[0] = energy_satellite[0] - SEND_GROUND_COST;
                                printf("satellite(1) sent to ground, energy now: %d\n", energy_satellite[0]);
                            :: else ->
                                safe_mode[0] = true;
                                printf("satellite(1) entering Safe Mode -- not enough energy for ground send\n");
                            fi;
                        :: skip -> printf("satellite(1) unable to send to the ground\n"); 
                        fi
                    fi
                ::  grant_isl[0] ? is_turn_send_isl12 ->
                    if 
                    :: is_turn_send_isl12 == 12 -> 
                        /* Ensure satellite has at least SAFE_ENERGY before sending */   
                        assert(energy_satellite[0] >= SAFE_ENERGY);
                        if
                        :: ISL[1] ! temp_message_send -> 
                            printf("satellite(1) sent message to satellite(2)\n");                            
                            head_sat1 = (head_sat1 + 1) % buffer_cap;

                            /* Energy consumption for ISL sending */
                            if
                            :: energy_satellite[0] >= SEND_ISL_COST ->
                                energy_satellite[0] = energy_satellite[0] - SEND_ISL_COST;
                                printf("satellite(1) sent ISL, energy now: %d\n", energy_satellite[0]);
                            :: else ->
                                safe_mode[0] = true;
                                printf("satellite(1) entering Safe Mode -- not enough energy for ISL\n");
                            fi;
                        :: skip ->
                           printf("satellite(1) unable to send message to satellite(2). Full buffer \n");
                        fi
                    fi
                ::  grant_isl[2] ? is_turn_send_isl13 -> 
                    if
                    :: is_turn_send_isl13 == 13 ->
                       /* Ensure satellite has at least SAFE_ENERGY before sending */   
                        assert(energy_satellite[0] >= SAFE_ENERGY);
                        if
                        :: ISL[2] ! temp_message_send -> 
                            printf("satellite(1) sent message to satellite(3) \n");                            
                            head_sat1 = (head_sat1 + 1) % buffer_cap;

                            /* Energy consumption for ISL sending */
                            if
                            :: energy_satellite[0] >= SEND_ISL_COST ->
                                energy_satellite[0] = energy_satellite[0] - SEND_ISL_COST;
                                printf("satellite(1) sent ISL, energy now: %d\n", energy_satellite[0]);
                            :: else ->
                                safe_mode[0] = true;
                                printf("satellite(1) entering Safe Mode -- not enough energy for ISL\n");
                            fi;
                        :: skip -> 
                           printf("satellite(1) unable to send message to satellite(3). Full buffer \n");
                        fi
                    fi
                :: skip -> 
                   printf("satellite(1) unable to send message -- all channels blocked \n");
                fi
            :: else ->
                safe_mode[0] = true;
                printf("satellite(1) entering Safe Mode, energy: %d\n", energy_satellite[0]);
            fi
        :: tail_sat1 == head_sat1 -> 
            printf("skip slot\n");
            run timekeeper();
            run coordinator();
        fi
}

proctype satellite2()
{
    MESSAGE buff[buffer_cap], temp_message_receive, temp_message_send;
    tail_sat2 = 0;
    head_sat2 = 0;
    bool is_turn_send_ground = false;
    int is_turn_send_isl12 = 0;
    int is_turn_send_isl23 = 0;

//  .............receiving phase.............

    do
    :: ISL[1] ? temp_message_receive -> 
        
        // Ensure buffer is not full before adding a new message
        assert( (tail_sat2 + 1) % buffer_cap != head_sat2 );

        buff[tail_sat2].message_type = temp_message_receive.message_type;
        buff[tail_sat2].sender_satellite_id = temp_message_receive.sender_satellite_id;
        buff[tail_sat2].receiver_satellite_id = temp_message_receive.receiver_satellite_id;
        buff[tail_sat2].payload = temp_message_receive.payload;

        printf("satellite(2) buffered message {type: %d, sender : %d, receiver: %d, payload: %d}\n", buff[tail_sat2].message_type, buff[tail_sat2].sender_satellite_id, buff[tail_sat2].receiver_satellite_id, buff[tail_sat2].payload);

        tail_sat2 = (tail_sat2 + 1) % buffer_cap;

        /* Energy consumption for receiving data */
        if
        :: energy_satellite[1] >= RECEIVE_COST ->
               energy_satellite[1] = energy_satellite[1] - RECEIVE_COST;
               printf("satellite(2) received a message, energy now: %d\n", energy_satellite[1]);
        :: else ->
               safe_mode[1] = true;
               printf("satellite(2) entering Safe Mode due to low energy on receive (%d)\n", energy_satellite[1]);
        fi;
    :: skip -> goto sendingPhase2
    od


//  .............sending phase.............

sendingPhase2:
    if
    :: tail != head -> 
        temp_message_send.message_type = buff[head].message_type;
        temp_message_send.sender_satellite_id = buff[head].sender_satellite_id;
        temp_message_send.receiver_satellite_id = buff[head].receiver_satellite_id;
        temp_message_send.payload = buff[head].payload;
        if 
        :: grant_ground[1] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> 
                printf("satellite(2) is sending to the ground\n");
                
                if
                :: message_sent_to_ground ! 2 -> 
                    printf("satellite(2) sent to the ground, %d\n", head);
                    buff[head].message_type = NONE;
                    buff[head].sender_satellite_id = -1;
                    buff[head].receiver_satellite_id = -1;
                    buff[head].payload = -1;
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(2) unable to send to the ground -- blocked\n");
                fi
            fi
        ::  grant_isl[0] ? is_turn_send_isl12 ->
                if 
                :: is_turn_send_isl12 == 12 -> 
                    if
                    :: ISL[0] ! temp_message_send -> 
                        printf("satellite(2) sent message to satellite(1) \n");
                        head = (head + 1) % buffer_cap;
                    :: else -> printf("satellite(2) unable to send message to satellite(1). Full buffer \n");
                    fi
                fi
        ::  grant_isl[2] ? is_turn_send_isl23 -> 
            if
            :: is_turn_send_isl23 == 23 ->
                if
                :: ISL[2] ! temp_message_send -> 
                    printf("satellite(2) sent message to satellite(3) \n");
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(2) unable to send message to satellite(3). Full buffer \n");
                fi
            fi
        :: else -> printf("satellite(2) unable to send message -- all channels blocked \n");
        fi
    :: tail == head -> 
        printf("skip slot\n");
        run timekeeper();
        run coordinator();
    fi

}

proctype satellite3()
{
    MESSAGE buff[buffer_cap], temp_message_receive, temp_message_send;
    tail_sat3 = 0;
    head_sat3 = 0;
    bool is_turn_send_ground = false;
    int is_turn_send_isl13 = 0;
    int is_turn_send_isl23 = 0;

//  .............receiving phase.............

    do
    :: ISL[2] ? temp_message_receive -> 
       
        // Ensure buffer is not full before adding a new message
        assert( (tail_sat3 + 1) % buffer_cap != head_sat3 );

        buff[tail_sat3].message_type = temp_message_receive.message_type;
        buff[tail_sat3].sender_satellite_id = temp_message_receive.sender_satellite_id;
        buff[tail_sat3].receiver_satellite_id = temp_message_receive.receiver_satellite_id;
        buff[tail_sat3].payload = temp_message_receive.payload;

        printf("satellite(3) buffered message {type: %d, sender : %d, receiver: %d, payload: %d}\n", buff[tail_sat3].message_type, buff[tail_sat3].sender_satellite_id, buff[tail_sat3].receiver_satellite_id, buff[tail_sat3].payload);

        tail_sat3 = (tail_sat3 + 1) % buffer_cap;

         /* Energy consumption for receiving data */
        if
        :: energy_satellite[2] >= RECEIVE_COST ->
               energy_satellite[2] = energy_satellite[2] - RECEIVE_COST;
               printf("satellite(3) received a message, energy now: %d\n", energy_satellite[2]);
        :: else ->
               safe_mode[2] = true;
               printf("satellite(3) entering Safe Mode due to low energy on receive (%d)\n", energy_satellite[2]);
        fi;
    :: skip -> goto sendingPhase3
    od

//  .............sending phase.............

sendingPhase3:
    if
    :: tail != head -> 
        temp_message_send.message_type = buff[head].message_type;
        temp_message_send.sender_satellite_id = buff[head].sender_satellite_id;
        temp_message_send.receiver_satellite_id = buff[head].receiver_satellite_id;
        temp_message_send.payload = buff[head].payload;
        if 
        :: grant_ground[2] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> 
                printf("satellite(3) is sending to the ground\n");
                if
                :: message_sent_to_ground ! 3 -> 
                    printf("satellite(3) sent to the ground, %d\n");
                    buff[head].message_type = NONE;
                    buff[head].sender_satellite_id = -1;
                    buff[head].receiver_satellite_id = -1;
                    buff[head].payload = -1;
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(3) unable to send to the ground\n");
                fi
            fi
        ::  grant_isl[2] ? is_turn_send_isl13 ->
                if 
                :: is_turn_send_isl13 == 13 -> 
                    if
                    :: ISL[0] ! temp_message_send -> 
                        printf("satellite(3) sent message to satellite(1) \n");
                        head = (head + 1) % buffer_cap;
                    :: else -> printf("satellite(3) unable to send message to satellite(1). Full buffer \n");
                    fi
                fi
        ::  grant_isl[1] ? is_turn_send_isl23 -> 
            if
            :: is_turn_send_isl23 == 23 ->
                if
                :: ISL[1] ! temp_message_send -> 
                    printf("satellite(3) sent message to satellite(2) \n");
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(3) unable to send message to satellite(2). Full buffer \n");
                fi
            fi
        :: else -> printf("satellite(3) unable to send message -- all channels blocked \n");
        fi
    :: tail == head -> 
        printf("skip slot\n");
        run timekeeper();
        run coordinator(); 
    fi
}

proctype groundStation() {

    int temp_message;

    if
    :: message_sent_to_ground ? temp_message -> 
        if
        :: temp_message == 1 -> 
            message_num_per_satellite[0] = message_num_per_satellite[0] + 1;
            printf("satellite(1) sent %d message(s) to the ground! \n", message_num_per_satellite[0]);
        :: temp_message == 2 -> 
            message_num_per_satellite[1] = message_num_per_satellite[1] + 1;
            printf("satellite(2) sent %d message(s) to the ground! \n", message_num_per_satellite[1]);
        :: temp_message == 3 -> 
            message_num_per_satellite[2] = message_num_per_satellite[2] + 1;
            printf("satellite(3) sent %d message(s) to the ground! \n", message_num_per_satellite[2]);
        :: else -> printf("No buffered message from satellite(1) sent to the ground at the moment! \n");
        fi
    :: else -> printf("ground receiving buffer blocked! \n");
    fi
}


init {

    int max = 0;
    do
    :: max < 3 ->
        max = max + 1
        MESSAGE m, h;
        m.message_type = IMAGE;
        m.sender_satellite_id = 1;
        m.receiver_satellite_id = 3;
        m.payload = 25;
        h.message_type = ACK;
        h.sender_satellite_id = 3;
        h.receiver_satellite_id = 1;
        h.payload = 123;
        ISL[0] ! m;
        run timekeeper();
        run coordinator();
        run satellite1();
        ISL[0] ! h;
        run timekeeper();
        run coordinator();
        run satellite1();
        run timekeeper();
        run coordinator();
        run groundStation();
        run timekeeper();
        run coordinator();
        run satellite2();
        run timekeeper();
        run coordinator();
        run groundStation();
        run timekeeper();
        run coordinator();
        run satellite3();
        run timekeeper();
        run coordinator();
        run groundStation();
    od

}

