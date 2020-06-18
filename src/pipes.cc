#include "pipes.h"
#include "debug.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <system_error>

int init_pipe(const char* name, int flags)
{
    int errsv = 0;
    int fifofd = -1;

    if(access(name, F_OK) != 0) {
        if(errno != ENOENT) {
            errsv = errno;
            goto quit;
        }
        if(mkfifo(name, 0600) != 0) {
            errsv = errno;
            goto quit;
        }
    }

    fifofd = open(name, flags);
    errsv = errno;

    quit:
    errno = errsv;
    return fifofd;
}

int init_pipe_at(int dir_fd, const char* name, int flags)
{
    int errsv = 0;
    int fifofd = -1;

    if(faccessat(dir_fd, name, F_OK, 0) != 0) {

        if(errno != ENOENT) {
            errsv = errno;
            goto fail;
        }

        if(mkfifoat(dir_fd, name, 0600) != 0) {
            errsv = errno;
            goto fail;
        }
    }

    fifofd = openat(dir_fd, name, flags);
    errsv = errno;

    fail:
    errno = errsv;
    return fifofd;
}

std::array<int, 3> init_ninelive_pipes(const char* himu_dir, int flags)
{
    int errsv = 0;
    int ret = 0;
    std::array<int, 3> result = { {-1,-1,-1} };
    int basefd, leftfd, rightfd;

    int dir_fd = open(HIMU_COMM_DIR, O_PATH | O_DIRECTORY); // O_DIRECTORY
    errsv = errno;

    if(dir_fd == -1)
        goto fail;

    basefd = init_pipe_at(dir_fd, HIMU_COMM_BASE, flags);
    errsv = errno;
    if(basefd == -1) {
        debug_printf_n("failed opening pipe %s with %d:%s", HIMU_COMM_DIR, errsv, strerror(errsv));
        goto fail;
    }

    /* init leftfd */
    leftfd = init_pipe_at(dir_fd, HIMU_COMM_LEFT, flags);
    errsv = errno;
    if(leftfd == -1) {
        debug_printf_n("failed opening pipe %s with %d:%s", HIMU_COMM_DIR, errsv, strerror(errsv));
        goto fail;
    }

    /* init rightfd */
    rightfd = init_pipe_at(dir_fd, HIMU_COMM_RIGHT, flags);
    errsv = errno;
    if(rightfd == -1) {
        debug_printf_n("failed opening pipe %s with %d:%s", HIMU_COMM_DIR, errsv, strerror(errsv));
        goto fail;
    }

    result[0] = basefd;
    result[1] = leftfd;
    result[2] = rightfd;

    return result;
    fail:
    errno = errsv;
    return result;
}



static const int rcv_time_full_len = RCV_TIME_LENGTH + 5 + 1;
static const int pos_vel_full_len = POS_VEL_LENGTH + 5 + 1;
static const int full_packet_size = rcv_time_full_len + pos_vel_full_len;


int base_read(int fd, unsigned char* buffer, struct RcvTime& rt, struct PosVel& Pv)
{
    int len = 0;
    int errsv = 0;
    struct RcvTime *rt_ = 0;
    struct PosVel *pv = 0;

    len = read(fd, buffer, full_packet_size);
    errsv = errno;

    if(len != full_packet_size)
        goto fail;

    rt_ = (struct RcvTime*)(buffer + 5);
		rt = *rt_;
//		print_rcv_time(stderr, rt_);
		
    pv = (struct PosVel *)(buffer + rcv_time_full_len + 5);
    Pv = *pv;
//    print_pos_vel(stderr, pv);

    return len;

    fail:
    errno = errsv;
    return -1;
}

int position_read(int fd, struct rover_pos_full &pos)
{
    int len = 0;
    int errsv = 0;

    len = read(fd, &pos, sizeof(struct rover_pos_full));
    errsv = errno;

    if(len != sizeof(struct rover_pos_full))
        goto fail;

    return len;

    fail:
    errno = errsv;
    return -1;
}
