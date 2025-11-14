
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
    run timekeeper();
    run timekeeper();
    run timekeeper();
    run coordinator();
    run coordinator();
    run coordinator();
    run coordinator();
    run coordinator();
}

