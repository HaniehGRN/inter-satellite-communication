#define N 8
#define buffer_cap 5
#define satellite_num 3
int current_slot = 0;   
chan time_signal = [4] of {int};
chan grant_ground = [0] of {int};
grant_ground[0] = 0;
turn = { onetwo, twothree, onethree };
chan grant_isl = [0] of {turn};
grant_isl[0] = onetwo;
typedef MESSAGE {
    MessageType message_type,
    int sender_satellite_id, 
    int receiver_satellite_id,
    int payload;
}
chan ISL = [4] of {MESSAGE};
int message_counter[4] = 0;
MessageType = {IMAGE, COMMAND, TELEMETRY, ACK}

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
        :: current_slot = 1 -> grant_ground[0] ! 2;
        :: current_slot = 2 -> grant_ground[0] ! 3;
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
    bool is_turn = false;
    MESSAGE temp_message;

    if 
    :: tail > -1 -> goto wait

    fi
    if 
    :: (ISL[id] ? temp_message) && (tail < buffer_cap ) -> buff[tail]; tail = tail + 1;
    :: 


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
















