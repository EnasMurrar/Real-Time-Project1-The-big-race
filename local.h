
/*
 * local header file for pipe client-server
 */

#ifndef __LOCAL_H_
#define __LOCAL_H_
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>


#include <unistd.h>
#include <limits.h>
#include <time.h> 
#include <math.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

//Defining Constants
#define NUMBER_OF_PLAYERS 4
#define NUMBER_OF_ROUNDS 2
#define DISTANCE 5.0

struct player{
pid_t pid;
int team;
int location;
int speed;
};

#endif

//===========================================================================================================================================
// Function to choose a random real number in specific range

float float_rand(float min, float max) {
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min); /* [min, max] */
}
//===========================================================================================================================================
// Function to choose a random intger number in specific range

int Drand(int min_range, int max_range) {
    int value;
    /* Simple check just to make sure the range is not = 0 */
    if (min_range == max_range) {
        printf("Please provide a range that is not equal to 0!\n");
        exit(-10);
    }

    /* Genarate the numbers */
    value = (int)(min_range + (rand() % (max_range - min_range)));
    return value;
}
