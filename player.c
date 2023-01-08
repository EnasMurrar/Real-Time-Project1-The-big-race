#include "local.h"

int readData(int, char[]);
void startRunning(int);
float probability_of_falling();

pid_t nextpid;
int team;
int location;
int speed;
int main(int argc, char * argv[]) {
    if (argc != 6) {
        perror("We need more arguments");
        exit(1);
    }

    int pipefds[2];
    pipefds[0] = atoi(argv[1]); // read_pipe_number1
    pipefds[1] = atoi(argv[2]); // write_pipe_number1
    team = atoi(argv[3]);
    location = atoi(argv[4]);
    speed = atoi(argv[5]);
    char readmessage[10];

    //=====================================================================================================================//
    //                                Making the player process sensitive to SIGUSR1                                       //
    //=====================================================================================================================//
    if (sigset(SIGUSR1, startRunning) == SIG_ERR) {
        printf("\033[0;31m"); // set the color to red 
        perror("Sigset can not set SIGUSR1");
        printf("\033[0m"); // reset the color to the default
        exit(2);
    }

    for (int z = 0; z < NUMBER_OF_PLAYERS / 2; z++) {
        read(pipefds[0], readmessage, sizeof(readmessage));
        //printf("Child Process from team %d- Reading from pipe – Message is %s\n", team, readmessage);
        nextpid = readData((location + 1), readmessage);
        //printf("Child Process from team %d-location %d next pid --> %d\n",team,location,nextpid);
    }

    kill(getppid(), SIGQUIT);
    if (pause() == -1) {
        printf("\033[0;31m"); // set the color to red 
        perror("Pause error");
        printf("\033[0m"); // reset the color to the default
        exit(6);
    } // Sleep until get sigusr1 signal to start runnig 
}
//=====================================================================================================================
// Read Data from pipe

int readData(int Loc, char readmessage[]) {
    int id = -1;
    int newLocation = Loc + 1;
    char * item;
    item = strtok(readmessage, " ");
    //printf("Item = %s \n", item);
    int num = atoi(item);
    if (num == newLocation) {
        item = strtok(NULL, "\n");
        id = atoi(item);
        //printf("Id = %s \n", item);
    }
    return (id);
}

//=========================================================================================================================
void startRunning(int sig_num) {
    float time = DISTANCE / speed;
    if (team == 1) {
        printf("\033[0;31m"); // set the color to red 
        printf("I am player at location %d from team %d, I will start running!!\n", location + 1, team);
        printf("\033[0m"); // reset the color to the default
        fflush(stdout);
    } else {
        printf("\033[0;32m"); // set the color to green 
        printf("I am player at location %d from team %d, I will start running!!\n", location + 1, team);
        printf("\033[0m"); // reset the color to the default
        fflush(stdout);
    }
    float newtime = probability_of_falling();
    sleep(newtime + time); // Running 
    // Make the next player running or tell the parent that the last player arrived to get the score of this round
    if ((nextpid == -1) && (team == 1)) {
        nextpid = getppid();
        if (kill(nextpid, SIGUSR1) == -1) {
            perror("Kill failure !!");
            exit(3);
        }
    } else if ((nextpid == -1) && (team == 2)) {
        nextpid = getppid();
        if (kill(nextpid, SIGUSR2) == -1) {
            perror("Kill failure !!");
            exit(4);
        }
    } else if (kill(nextpid, SIGUSR1) == -1) {
        perror("Kill failure !!");
        exit(5);
    }
    
    if (pause() == -1) {
        printf("\033[0;31m"); // set the color to red 
        perror("Pause error");
        printf("\033[0m"); // reset the color to the default
        exit(6);
    } // Sleep until get killed 
}

//=========================================================================================================================
// Function to see the probability of falling and increase the time 

float probability_of_falling() {
    float value;
    int prand = Drand(0, 10);
    if (prand < 8 && prand > 4 && prand != 0) {
        value = float_rand(0.1, 0.3);
    } else if (prand > 7) {
        value = float_rand(0.4, 0.7);
    } else {
        value = 0.0;
    }
    return value;
}
