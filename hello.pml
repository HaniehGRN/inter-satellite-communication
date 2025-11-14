
//  define constant values 

#define N 8
#define buffer_cap 5
#define satellite_num 3

//  define enumerations  

MessageType = {IMAGE, COMMAND, TELEMETRY, ACK}
turn = { onetwo, twothree, onethree };

//  define custom types

typedef MESSAGE {
    MessageType message_type,
    int sender_satellite_id, 
    int receiver_satellite_id,
    int payload;
}

//  define channels

chan time_signal[satellite_num] = [1] of {int};
chan grant_ground[satellite_num] = [1] of {int};
chan grant_isl = [0] of {turn};
chan ISL = [4] of {MESSAGE};

//  define variables

int current_slot;  
int message_counter[4];

//grant_isl[0] = onetwo;

proctype timekeeper()
{
    atomic {
        current_slot = current_slot + 1;
        printf("Process %d started. Total instances: %d\n", _pid, current_slot);
    }
}

proctype coordinator()
{
    atomic {
        do
        :: current_slot = 0 -> grant_ground[0] ! 1;
        :: current_slot = 1 -> grant_ground[1] ! 1;
        :: current_slot = 2 -> grant_ground[2] ! 1;
        :: current_slot = 3;
        :: current_slot = 4 -> grant_isl ! onetwo;
        :: current_slot = 5 -> grant_isl ! twothree;
        :: current_slot = 6 -> grant_isl ! onethree;
        :: current_slot = 7;
        od
    }
}

proctype satellite(int id)
{
    MESSAGE buff[buffer_cap];
    int tail = 0;
    int head = 0;
    bool is_turn = false;
    MESSAGE temp_message;

    if 
    :: tail > -1 -> goto wait
    fi

    if 
    :: head > 
    :: (ISL[id] ? temp_message) ->
        if 
        :: (tail < buffer_cap ) -> buff[tail]; tail = tail + 1;
        :: else -> printf("Warning\n");
    fi 


wait:
    if 
    :: 
    fi

}

proctype groundStation()
{
    
}


init {

    run satellite(1);
    run satellite(2);
    run satellite(3);

}
















