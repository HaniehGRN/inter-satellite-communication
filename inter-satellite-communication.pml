
//  .............define constant values............. 

#define N 8
#define buffer_cap 5
#define satellite_num 3

//  .............define enumerations.............  

mtype = {IMAGE, COMMAND, TELEMETRY, ACK};

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
chan message_sent_to_ground[satellite_num] = [1] of {int};
chan ISL[satellite_num] = [1] of {mtype, int, int, int};

//  .............define variables.............

int current_slot;
int message_counter[4] = {0, 0, 0, 0};
int slot = -1;


proctype timekeeper()
{
    printf("timekeeper ran.\n");
    //printf("current slot before write : %d\t", current_slot);
    atomic {
        if
        :: time_signal ! current_slot -> 
        current_slot = (current_slot + 1) % N;
        fi
        //printf("current slot after write :%d\n", current_slot);
    }
}

proctype coordinator()
{
    printf("coordinator ran.\n");
    if 
    :: time_signal ? slot -> 
        printf("slot :%d\n", slot);
        if
        :: slot == 0 -> grant_ground[0] ! 1; printf("done1\n");
        :: slot == 1 -> grant_ground[1] ! 1; printf("done2\n");
        :: slot == 2 -> grant_ground[2] ! 1;printf("done3\n");
        :: slot == 3 -> printf("done4\n");
        :: slot == 4 -> grant_isl[0] ! 12; printf("done5\n");
        :: slot == 5 -> grant_isl[1] ! 23; printf("done6\n");
        :: slot == 6 -> grant_isl[2] ! 13; printf("done7\n");
        :: slot == 7 -> printf("done8\n");
        fi
    fi
}


proctype satellite1()
{
    MESSAGE buff[buffer_cap];
    //buff[0].message_type = IMAGE;
    //buff[0].sender_satellite_id = 1;
    //buff[0].receiver_satellite_id = 3;
    //buff[0].payload = 25;
    MESSAGE temp_message;
    int tail = 0;
    int head = 0;
    bool is_turn_send_ground = false;
    bool is_turn_send_isl = false;

//  .............receiving phase.............

    do
    :: ISL[0] ? temp_message -> 
        buff[tail].message_type = temp_message.message_type;
        buff[tail].sender_satellite_id = temp_message.sender_satellite_id;
        buff[tail].receiver_satellite_id = temp_message.receiver_satellite_id;
        buff[tail].payload = temp_message.payload;
        printf("type: %d, sender : %d, receiver: %d, payload: %d", buff[tail].message_type, buff[tail].sender_satellite_id, buff[tail].receiver_satellite_id, buff[tail].payload);
        tail = (tail + 1) % buffer_cap;
    od

//  .............sending phase.............

    if
    :: tail != head -> 
        if 
        :: grant_ground[0] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> 
                printf("satellite(1) is sending to the ground\n");
                if
                :: message_sent_to_ground[0] ! 1 -> 
                    printf("satellite(1) sent to the ground, %d\n", head);
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(1) unable to send to the ground\n");
                fi
            fi
        :: grant_isl[0] ? is_turn_send_isl ->
            if
            :: is_turn_send_isl == 12 -> 
                printf("satellite(1) \n");
            :: is_turn_send_isl == 13 ->
                printf("satellite(1) \n");
            fi
        fi
    :: tail == head -> 
        printf("skip slot\n");
        run timekeeper();
        run coordinator();
    fi

}

proctype satellite2()
{
    MESSAGE buff[buffer_cap];
    MESSAGE temp_message;
    int tail = 0;
    int head = 0;
    bool is_turn_send_ground = false;
    bool is_turn_send_isl = false;

//  .............receiving phase.............

    do
    :: ISL[1] ? temp_message -> 
        buff[tail].message_type = temp_message.message_type;
        buff[tail].sender_satellite_id = temp_message.sender_satellite_id;
        buff[tail].receiver_satellite_id = temp_message.receiver_satellite_id;
        buff[tail].payload = temp_message.payload;
        printf("type: %d, sender : %d, receiver: %d, payload: %d", buff[tail].message_type, buff[tail].sender_satellite_id, buff[tail].receiver_satellite_id, buff[tail].payload);
        tail = (tail + 1) % buffer_cap;
    od


//  .............sending phase.............

    if
    :: tail != head -> 
        if 
        :: grant_ground[1] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> 
                printf("satellite(2) is sending to the ground\n");
                
                if
                :: message_sent_to_ground[1] ! 1 -> 
                    printf("satellite(2) sent to the ground, %d\n", head);
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(2) unable to send to the ground\n");
                fi
            fi
        :: grant_isl[1] ? is_turn_send_isl ->
            if
            :: is_turn_send_isl == 12 ->
                printf("satellite(2) \n");
            :: is_turn_send_isl == 23 ->
                printf("satellite(2) \n");
            fi
        fi
    :: tail == head -> 
        printf("skip slot\n");
        run timekeeper();
        run coordinator();
    fi

}

proctype satellite3()
{
    MESSAGE buff[buffer_cap];
    MESSAGE temp_message;
    int tail = 0;
    int head = 0;
    bool is_turn_send_ground = false;
    bool is_turn_send_isl = false;

//  .............receiving phase.............

    do
    :: ISL[1] ? temp_message -> 
        buff[tail].message_type = temp_message.message_type;
        buff[tail].sender_satellite_id = temp_message.sender_satellite_id;
        buff[tail].receiver_satellite_id = temp_message.receiver_satellite_id;
        buff[tail].payload = temp_message.payload;
        printf("type: %d, sender : %d, receiver: %d, payload: %d", buff[tail].message_type, buff[tail].sender_satellite_id, buff[tail].receiver_satellite_id, buff[tail].payload);
        tail = (tail + 1) % buffer_cap;
    od

    if
    :: tail != head -> 
        if 
        :: grant_ground[2] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> 
                printf("satellite(3) is sending to the ground\n");
                
                if
                :: message_sent_to_ground[2] ! 1 -> 
                    printf("satellite(3) sent to the ground, %d\n", head);
                    head = (head + 1) % buffer_cap;
                :: else -> printf("satellite(3) unable to send to the ground\n");
                fi
            fi
        :: grant_isl[2] ? is_turn_send_isl ->
            if
            :: is_turn_send_isl == 13 ->
                printf("satellite(3)\n");
            :: is_turn_send_isl == 23 ->
                printf("satellite(3) \n");
            fi
        fi
    :: tail == head -> 
        printf("skip slot\n");
        run timekeeper();
        run coordinator(); 
    fi
}

//proctype groundStation() {}


init {
    //run satellite(1);
    //run satellite(2);
    //run satellite(3);
    MESSAGE m;
    m.message_type = IMAGE;
    m.sender_satellite_id = 1;
    m.receiver_satellite_id = 3;
    m.payload = 25;
    ISL[0] ! m;
    run timekeeper();
    run coordinator();
    run satellite1();
    ISL[1] ! m;
    run timekeeper();
    run coordinator();
    run satellite2();
}

