#ifndef GUARD_BIKE_H
#define GUARD_BIKE_H

// Player speeds
enum
{
    PLAYER_SPEED_STANDING,
    PLAYER_SPEED_NORMAL,
    PLAYER_SPEED_FAST,
    PLAYER_SPEED_FASTER,
    PLAYER_SPEED_FASTEST,
};

// mach bike transitions enum
enum
{
    MACH_TRANS_FACE_DIRECTION,
    MACH_TRANS_TURN_DIRECTION,
    MACH_TRANS_KEEP_MOVING,
    MACH_TRANS_START_MOVING
};

// Acro bike states
enum
{
    ACRO_STATE_NORMAL,
    ACRO_STATE_TURNING,
    ACRO_STATE_WHEELIE_STANDING,
    ACRO_STATE_BUNNY_HOP,
    ACRO_STATE_WHEELIE_MOVING,
    ACRO_STATE_SIDE_JUMP,
    ACRO_STATE_TURN_JUMP,
};

// Acro bike transitions
enum
{
    ACRO_TRANS_FACE_DIRECTION,
    ACRO_TRANS_TURN_DIRECTION,
    ACRO_TRANS_MOVING,
    ACRO_TRANS_NORMAL_TO_WHEELIE,
    ACRO_TRANS_WHEELIE_TO_NORMAL,
    ACRO_TRANS_WHEELIE_IDLE,
    ACRO_TRANS_WHEELIE_HOPPING_STANDING,
    ACRO_TRANS_WHEELIE_HOPPING_MOVING,
    ACRO_TRANS_SIDE_JUMP,
    ACRO_TRANS_TURN_JUMP,
    ACRO_TRANS_WHEELIE_MOVING,
    ACRO_TRANS_WHEELIE_RISING_MOVING,
    ACRO_TRANS_WHEELIE_LOWERING_MOVING,
};

enum
{
    BIKE_TRANS_FACE_DIRECTION,
    BIKE_TRANS_TURNING,
    BIKE_TRANS_MOVE,
    BIKE_TRANS_DOWNHILL,
    BIKE_TRANS_UPHILL,
};

enum
{
    BIKE_STATE_NORMAL,
    BIKE_STATE_TURNING,
    BIKE_STATE_SLOPE,
};

// Exported RAM declarations
extern bool8 gUnusedBikeCameraAheadPanback;

// Exported ROM declarations
void MovePlayerOnBike(u8 direction, u16 newKeys, u16 heldKeys);
void Bike_TryAcroBikeHistoryUpdate(u16 newKeys, u16 heldKeys);
bool8 IsBikingDisallowedByPlayer(void);
bool8 IsPlayerNotUsingAcroBikeOnBumpySlope(void);
void BikeClearState(int newDirHistory, int newAbStartHistory);
void Bike_UpdateBikeCounterSpeed(u8 counter);
s16 GetPlayerSpeed(void);
void Bike_HandleBumpySlopeJump(void);
bool32 IsRunningDisallowed(u8 metatile);

#endif // GUARD_BIKE_H
