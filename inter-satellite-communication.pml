
//  .............define constant values............. 

#define N 8
#define buffer_cap 5
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
int message_num_per_satellite[satellite_num];


proctype timekeeper()
{
    atomic {
        if
        :: time_signal ! current_slot -> 
        current_slot = (current_slot + 1) % N;
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
    int tail = 0;
    int head = 0;
    bool is_turn_send_ground = false;
    int is_turn_send_isl12 = 0;
    int is_turn_send_isl13 = 0;

//  .............receiving phase.............

    if
    :: ISL[0] ? temp_message_receive -> 
        buff[tail].message_type = temp_message_receive.message_type;
        buff[tail].sender_satellite_id = temp_message_receive.sender_satellite_id;
        buff[tail].receiver_satellite_id = temp_message_receive.receiver_satellite_id;
        buff[tail].payload = temp_message_receive.payload;
        printf("satellite(1) buffered message {type: %d, sender : %d, receiver: %d, payload: %d}\n", buff[tail].message_type, buff[tail].sender_satellite_id, buff[tail].receiver_satellite_id, buff[tail].payload);
        tail = (tail + 1) % buffer_cap;
    fi

//  .............sending phase.............
    
    
        if
        :: tail != head -> 
            temp_message_send.message_type = buff[head].message_type;
            temp_message_send.sender_satellite_id = buff[head].sender_satellite_id;
            temp_message_send.receiver_satellite_id = buff[head].receiver_satellite_id;
            temp_message_send.payload = buff[head].payload;
            if 
            :: grant_ground[0] ? is_turn_send_ground -> 
                if
                :: is_turn_send_ground -> 
                    printf("satellite(1) is sending to the ground\n");
                    buff[head].message_type = NONE;
                    buff[head].sender_satellite_id = -1;
                    buff[head].receiver_satellite_id = -1;
                    buff[head].payload = -1;
                    head = (head + 1) % buffer_cap;
                    if
                    :: message_sent_to_ground ! 1 -> 
                        printf("satellite(1) sent to the ground \n");
                    :: else -> printf("satellite(1) unable to send to the ground\n");
                    fi
                fi
            ::  grant_isl[0] ? is_turn_send_isl12 ->
                if 
                :: is_turn_send_isl12 == 12 -> 
                    if
                    :: ISL[1] ! temp_message_send -> 
                        printf("satellite(1) sent message to satellite(2) \n");
                        head = (head + 1) % buffer_cap;
                    :: else -> printf("satellite(1) unable to send message to satellite(2). Full buffer \n");
                    fi
                fi
            ::  grant_isl[2] ? is_turn_send_isl13 -> 
                if
                :: is_turn_send_isl13 == 13 ->
                    if
                    :: ISL[2] ! temp_message_send -> 
                        printf("satellite(1) sent message to satellite(3) \n");
                        head = (head + 1) % buffer_cap;
                    :: else -> printf("satellite(1) unable to send message to satellite(3). Full buffer \n");
                    fi
                fi
            :: else -> printf("satellite(1) unable to send message -- all channels blocked \n");
            fi
        :: tail == head -> 
            printf("skip slot\n");
            run timekeeper();
            run coordinator();
        fi
}

proctype satellite2()
{
    MESSAGE buff[buffer_cap], temp_message_receive, temp_message_send;
    int tail = 0;
    int head = 0;
    bool is_turn_send_ground = false;
    int is_turn_send_isl12 = 0;
    int is_turn_send_isl23 = 0;

//  .............receiving phase.............

    do
    :: ISL[1] ? temp_message_receive -> 
        buff[tail].message_type = temp_message_receive.message_type;
        buff[tail].sender_satellite_id = temp_message_receive.sender_satellite_id;
        buff[tail].receiver_satellite_id = temp_message_receive.receiver_satellite_id;
        buff[tail].payload = temp_message_receive.payload;
        printf("satellite(2) buffered message {type: %d, sender : %d, receiver: %d, payload: %d}\n", buff[tail].message_type, buff[tail].sender_satellite_id, buff[tail].receiver_satellite_id, buff[tail].payload);
        tail = (tail + 1) % buffer_cap;
    od


//  .............sending phase.............

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
    int tail = 0;
    int head = 0;
    bool is_turn_send_ground = false;
    int is_turn_send_isl13 = 0;
    int is_turn_send_isl23 = 0;

//  .............receiving phase.............

    do
    :: ISL[2] ? temp_message_receive -> 
        buff[tail].message_type = temp_message_receive.message_type;
        buff[tail].sender_satellite_id = temp_message_receive.sender_satellite_id;
        buff[tail].receiver_satellite_id = temp_message_receive.receiver_satellite_id;
        buff[tail].payload = temp_message_receive.payload;
        printf("satellite(3) buffered message {type: %d, sender : %d, receiver: %d, payload: %d}\n", buff[tail].message_type, buff[tail].sender_satellite_id, buff[tail].receiver_satellite_id, buff[tail].payload);
        tail = (tail + 1) % buffer_cap;
    od

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
                    printf("satellite(3) sent to the ground, %d\n", head);
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
    //run satellite(1);
    //run satellite(2);
    //run satellite(3);
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

