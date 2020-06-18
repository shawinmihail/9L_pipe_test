#pragma once

#include <array>

//#define HIMU_COMM_DIR "/home/misha/tmp/himu/"
#define HIMU_COMM_DIR "/tmp/himu/"
#define HIMU_COMM_BASE "base"
#define HIMU_COMM_LEFT "left"
#define HIMU_COMM_RIGHT "right"

int init_pipe(const char* name, int flags);

int init_pipe_at(int dir_fd, const char* name, int flags);

int base_read(int fd, unsigned char* buffer, struct RcvTime& rt, struct PosVel& Pv);

std::array<int, 3> init_ninelive_pipes(const char* himu_dir, int flags);

/*
int base_read(int fd, unsigned char* buffer, struct RcvTime& rt, struct PosVel& Pv);

int position_read(int fd, struct rover_pos_full &pos);
*/

/* -------------- */
#pragma pack(push, 1)
struct RcvTime
{
    uint32_t tod;                       // Tr modulo 1 day (86400000 ms) [ms]
    uint8_t cs;                         // Checksum
};
#pragma pack(pop)

#define RCV_TIME_LENGTH 5

#pragma pack(push, 1)
struct PosVel
{
    double x, y, z;                    // Cartesian coordinates [m]
    float pSigma;                      // Position MRSE [m]
    float vx, vy, vz;                  // Cartesian velocities [m/s]
    float vSigma;                      // Velocity MRSE [m/s]
    uint8_t solType;                   // Solution type
    uint8_t cs;                        // Checksum
};
#pragma pack(pop)

#define POS_VEL_LENGTH 46


#pragma pack(push, 1)
struct rover_pos {
    int8_t type;
    float x;
    float y;
    float z;
};

struct rover_pos_full {
    struct {
        int8_t type;
        float x;
        float y;
        float z;
        int enabled;
    } ant1;

    struct {
        int8_t type;
        float x;
        float y;
        float z;
        int enabled;
    } ant2;
};
#pragma pack(pop)

