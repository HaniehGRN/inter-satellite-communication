
//  .............define constant values............. 

#define N 8
#define buffer_cap 5
#define satellite_num 3

//  .............define enumerations.............  

mtype = {IMAGE, COMMAND, TELEMETRY, ACK};

//  .............define custom types.............

typedef MESSAGE {
    mtype message_type;
    int sender_satellite_id, receiver_satellite_id, payload;
}

//  .............define channels.............

chan time_signal = [1] of {int};
chan grant_ground[satellite_num] = [1] of {int};
chan grant_isl[satellite_num] = [1] of {int};
//chan ISL[satellite_num] = [1] of MESSAGE;

//  .............define variables.............

int current_slot;
int message_counter[4] = {0, 0, 0, 0};
int slot = -1;


proctype timekeeper()
{
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
        :: slot == 7; printf("done8\n");
        fi
    fi
}

proctype satellite1()
{
    //MESSAGE buff[1] = {IMAGE, 1, 3, 222};
    //MESSAGE temp_message;
    int tail = 1;
    int head = 0;
    bool is_turn_send_ground = false;
    bool is_turn_send_isl = false;

    do
    :: tail != head -> 
        if 
        :: grant_ground[0] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> goto send_ground1
            fi
        :: grant_isl[0] ? is_turn_send_isl ->
            if
            :: is_turn_send_isl == 12 -> goto send_two
            :: is_turn_send_isl == 13 -> goto send_three
            fi
        fi
    :: tail == head -> run timekeeper(); run coordinator();
    od

    //do
    //:: ISL[0] ? temp_message ->
send_two:
    printf("one is sending to two\n");

send_three:
printf("one is sending to three\n");

send_ground1:
printf("one is sending to the ground\n");

}

proctype satellite2()
{
    //MESSAGE buff[1] = {IMAGE, 1, 3, 222};
    //MESSAGE temp_message;
    int tail = 1;
    int head = 0;
    bool is_turn_send_ground = false;
    bool is_turn_send_isl = false;

    do
    :: tail != head -> 
        if 
        :: grant_ground[1] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> goto send_ground2
            fi
        :: grant_isl[1] ? is_turn_send_isl ->
            if
            :: is_turn_send_isl == 12 -> goto send_one
            :: is_turn_send_isl == 23 -> goto send_three
            fi
        fi
    :: tail == head -> run timekeeper(); run coordinator();
    od

    //do
    //:: ISL[0] ? temp_message ->
send_one:
    printf("two is sending to one\n");

send_three:
printf("two is sending to three\n");

send_ground2:
printf("two is sending to the ground\n");

}

proctype satellite3()
{
    //MESSAGE buff[1] = {IMAGE, 1, 3, 222};
    //MESSAGE temp_message;
    int tail = 1;
    int head = 0;
    bool is_turn_send_ground = false;
    bool is_turn_send_isl = false;

    do
    :: tail != head -> 
        if 
        :: grant_ground[2] ? is_turn_send_ground -> 
            if
            :: is_turn_send_ground -> goto send_ground3
            fi
        :: grant_isl[2] ? is_turn_send_isl ->
            if
            :: is_turn_send_isl == 13 -> goto send_one
            :: is_turn_send_isl == 23 -> goto send_two
            fi
        fi
    :: tail == head -> run timekeeper(); run coordinator();
    od

    //do
    //:: ISL[0] ? temp_message ->
send_one:
    printf("three is sending to one\n");

send_two:
printf("three is sending to two\n");

send_ground3:
printf("three is sending to the ground\n");

}

//proctype groundStation() {}


init {
    //run satellite(1);
    //run satellite(2);
    //run satellite(3);
    run timekeeper();
    run coordinator();
    run satellite1();
    run timekeeper();
    run coordinator();
    run satellite2();
    run timekeeper();
    run coordinator();
    run satellite3();

}

