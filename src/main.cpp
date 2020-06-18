#include <iostream>
#include <chrono>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <system_error>
#include <array>
#include <string.h>

#include "pipes.h"
#include "debug.h"


int main(int argc, char** argv)
{
	std::cout << "Started" << std::endl;

    int basefd, leftfd, rightfd;
    int errsv = 0;
    std::array<int, 3> pipes = init_ninelive_pipes(HIMU_COMM_DIR, O_RDWR);
    errsv = errno;

    if(pipes[0] == -1) {
        fprintf(stderr, "failed opening pipes with %d:%s\n", errsv, strerror(errsv));
        exit(EXIT_FAILURE);
    }
    
    basefd = pipes[0];
    leftfd = pipes[1];
    rightfd = pipes[2];
      
    int basefd_read = -1;
    int dir_fd = open(HIMU_COMM_DIR, O_PATH | O_DIRECTORY);
    errsv = errno;
    if(dir_fd == -1)
    {
        debug_printf_n("failed opening directory %s with %d:%s", HIMU_COMM_DIR, errsv, strerror(errsv));
        return -1;
    }
    
    int ret = init_pipe_at(dir_fd, HIMU_COMM_BASE, O_RDONLY | O_NONBLOCK);
    if(ret == -1)
    {
        debug_printf_n("failed opening pipe %s with %d:%s", HIMU_COMM_DIR, errsv, strerror(errsv));
        return -1;
    }
    else
    {
        basefd_read = ret;
    }
    
    int readen = 0;
    unsigned char buffer[256] = {0};
    PosVel pv;
    RcvTime rt;
    
    while(true){
        readen = base_read(basefd_read, buffer, rt, pv);
        if(readen == -1)
        {
            continue;
        }
        std::cout << "rt: " << rt.tod << std::endl;
        std::cout << "pv: " << pv.vx << std::endl;
    }

  
  /*
    while(true)
    {
        
        //PosVel pv = { .x = 1, .y = 2, .z = 3 , .pSigma = 0, .vx = 1, .vy = 2, .vz = 3, .vSigma = 100, .solType = 1, .cs = 13 };
        //std::cout << "!!!!" << std::endl;
        //write(basefd, &pv, sizeof(pv));
        //std::cout << "write" << std::endl;
        
        unsigned char buffer[256] = {0};
        int len = read(basefd_read, buffer, sizeof(pv));
        debug_printf_n("L: %s\n", len);
        std::cout << "read " << len << std::endl;
    }
    */

    
    
	return 0;
}
