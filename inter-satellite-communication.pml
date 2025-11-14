
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
chan ISL = [4] of {int, int, int, int};

//  .............define variables.............

int current_slot;  
int message_counter[4] = {0, 0, 0, 0};


proctype timekeeper()
{
    printf("current slot before write : %d\t", current_slot);
    atomic {
        if
        :: time_signal ! current_slot -> 
        current_slot = (current_slot + 1) % N;
        fi
        printf("current slot after write :%d\n", current_slot);
    }
}

proctype coordinator()
{
    int slot = -1;
    if 
    :: time_signal ? slot -> printf("slot : %d\n", slot);
        //do
        //:: turn == 0 -> grant_ground[0] ! 1;
        //:: turn == 1 -> grant_ground[1] ! 1;
        //:: turn == 2 -> grant_ground[2] ! 1;
        //:: turn == 3;
        //:: turn == 4 -> grant_isl[0] ! 12;
        //:: turn == 5 -> grant_isl[1] ! 23;
        //:: turn == 6 -> grant_isl[2] ! 13;
        //:: turn == 7;
        //od
    fi
}

proctype satellite(int id)
{
    MESSAGE buff[buffer_cap];
    int tail = 0;
    int head = 0;
    bool is_turn = false;
    //MESSAGE temp_message;

    if 
    :: tail > -1
    fi

    //if 
    //:: head > 0
    //:: (ISL[id] ? temp_message) ->
        //if 
        //:: (tail < buffer_cap ) -> buff[tail]; tail = tail + 1;
        //:: else -> printf("Warning\n");
    //fi 


//wait:
  //  if 
    //:: 
    //fi

}

//proctype groundStation() {}


init {
    //run satellite(1);
    //run satellite(2);
    //run satellite(3);
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
    run timekeeper();
    run coordinator();
}

