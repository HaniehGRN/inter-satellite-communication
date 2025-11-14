
//  define constant values 

#define N 8
#define buffer_cap 5
#define satellite_num 3

//  define enumerations  

//MessageType = {IMAGE, COMMAND, TELEMETRY, ACK}
//turn = { onetwo, twothree, onethree };

//  define custom types

typedef MESSAGE {
    //MessageType message_type,
    int sender_satellite_id, receiver_satellite_id, payload;
}

//  define channels

chan time_signal = [1] of {int};
chan grant_ground[satellite_num] = [1] of {int};
chan grant_isl[satellite_num] = [1] of {int};

chan ISL = [4] of {int, int, int, int};

//  define variables

int current_slot;  
int message_counter[4] = {0, 0, 0, 0};


proctype timekeeper()
{
    atomic {
        time_signal = current_slot;
        current_slot = current_slot + 1;
        printf("%d\n", current_slot);
    }
}

proctype coordinator()
{
    atomic {
        do
        :: time_signal == 0 -> grant_ground[0] ! 1;
        :: time_signal == 1 -> grant_ground[1] ! 1;
        :: time_signal == 2 -> grant_ground[2] ! 1;
        :: time_signal == 3;
        :: time_signal == 4 -> grant_isl[0] ! 12;
        :: time_signal == 5 -> grant_isl[1] ! 23;
        :: time_signal == 6 -> grant_isl[2] ! 13;
        :: time_signal == 7;
        od
    }
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

}

// chan grant_isl = [0] of {turn};
