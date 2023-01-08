#include "local.h"

// Functions' prototypes
int change_speed(int);
int * shuffle(size_t);
char * writeData(struct player * );
void sig_quit_catcher(int);
void sig_user1_catcher(int);
void sig_user2_catcher(int);
void kill_AllPlayers();
void game_winner();
void Project_interface ();
void sig_int_catcher(int);
void start_game();

// Global variables
int team1_winning = 0;                       // Storing the score of the first team after each round
int team2_winning = 0;                       // Storing the score of the second team after each round
struct player team1[NUMBER_OF_PLAYERS / 2];  // Array of struct to save data about players of the First team 
struct player team2[NUMBER_OF_PLAYERS / 2];  // Array of struct to save data about players of the Second team 
int number_of_ready_players = 0;             // Counter for the number of ready players, reset at the end of every round.
int current_round = 0;                       // Current round
char writemessages[NUMBER_OF_PLAYERS / 2][10];
char writemessages2[NUMBER_OF_PLAYERS / 2][10];

//====================================================================================================================================
int main(int argc, char * argv[]) {
    Project_interface();
    srand((unsigned) getpid());
    int stat;
    int pipefds[NUMBER_OF_PLAYERS][2];
    int returnstatus;
    pid_t pid;
    int * location;
    char read_pipe_number1[20], write_pipe_number1[20];

    //=====================================================================================================================//
    //                      Making the parent process sensitive to SIGQUIT, SIGUSR1, SIGUSR2                               //
    //=====================================================================================================================//     
    if (sigset(SIGQUIT, sig_quit_catcher) == SIG_ERR) { // 
        printf("\033[0;31m"); // set the color to red 
        perror("Sigset can not set SIGQUIT");
        printf("\033[0m"); // reset the color to the default
        exit(-1);
    }
    if (sigset(SIGUSR1, sig_user1_catcher) == SIG_ERR) {
        printf("\033[0;31m"); // set the color to red 
        perror("Sigset can not set SIGQUIT");
        printf("\033[0m"); // reset the color to the default
        exit(-2);
    }

    if (sigset(SIGUSR2, sig_user2_catcher) == SIG_ERR) {
        printf("\033[0;31m"); // set the color to red 
        perror("Sigset can not set SIGQUIT");
        printf("\033[0m"); // reset the color to the default
        exit(-3);
    }

    //=====================================================================================================================//
    //                                                 Prepare locations                                                   //
    //=====================================================================================================================//
    location = shuffle(NUMBER_OF_PLAYERS / 2);

    //=====================================================================================================================//
    //                               Prepare speeds and locations for each player                                          //
    //=====================================================================================================================//
    for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
        team1[i].speed = Drand(20, 30);
        team1[i].location = location[i];
        team1[i].team = 1;
        team2[i].speed = Drand(20, 30);
        team2[i].location = location[i];
        team2[i].team = 2;
    }
    //************************************************************************************************************************
    for (int round = 1; round <= NUMBER_OF_ROUNDS; round++) {
        current_round++;
        number_of_ready_players = 0;
        //=====================================================================================================================//
        //                                                   Create pipes                                                      //
        //=====================================================================================================================// 
        for (int x = 0; x < NUMBER_OF_PLAYERS; x++) {
            returnstatus = pipe(pipefds[x]);
            if (returnstatus == -1) {
                printf("\033[0;31m"); // set the color to red 
                perror("Unable to create pipe\n");
                printf("\033[0m"); // reset the color to the default
                exit(-4);
            }
        }

        //=====================================================================================================================//
        //                                                 Prepare locations                                                   //
        //=====================================================================================================================//
        location = shuffle(NUMBER_OF_PLAYERS / 2);

        //=====================================================================================================================//
        //                               Prepare speeds and locations for each player                                          //
        //=====================================================================================================================//
        if (current_round > 1) {
            for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
                team1[i].speed = change_speed(team1[i].speed);
                team1[i].location = location[i];
                team1[i].team = 1;
                team2[i].speed = change_speed(team2[i].speed);
                team2[i].location = location[i];
                team2[i].team = 2;
            }
        }

        //=====================================================================================================================//
        //                                                Create players for team1                                             //
        //=====================================================================================================================//
        for (int z = 0; z < NUMBER_OF_PLAYERS / 2; z++) {
            pid = fork();
            if (pid == -1) {
                printf("fork failure ... getting out\n");
                exit(-5);
            }
            // Child process
            if (pid == 0) {
                sprintf(read_pipe_number1, "%d", pipefds[z][0]);
                sprintf(write_pipe_number1, "%d", pipefds[z][1]);
                char team[5];
                char index[5];
                char speed[5];
                sprintf(index, "%d", z);
                sprintf(team, "%d", 1);
                sprintf(speed, "%d", team1[z].speed);
                execlp("./player", "./player", read_pipe_number1, write_pipe_number1, team, index, speed, (char * ) NULL);
                printf("\033[0;31m"); // set the color to red 
                perror("exec failure !!");
                printf("\033[0m"); // reset the color to the default
                exit(-6);
            } else {
                team1[z].pid = pid;
            }
        }

        //=====================================================================================================================//
        //                                               Create players for team2                                              //
        //=====================================================================================================================//   
        for (int z = 0; z < NUMBER_OF_PLAYERS / 2; z++) {
            pid = fork();

            // Child process
            if (pid == 0) {
                sprintf(read_pipe_number1, "%d", pipefds[z + (NUMBER_OF_PLAYERS / 2)][0]);
                sprintf(write_pipe_number1, "%d", pipefds[z + (NUMBER_OF_PLAYERS / 2)][1]);
                char team[5];
                char index[5];
                char speed[5];
                sprintf(index, "%d", z);
                sprintf(team, "%d", 2);
                sprintf(speed, "%d", team2[z].speed);
                execlp("./player", "./player", read_pipe_number1, write_pipe_number1, team, index, speed, (char * ) NULL);
                printf("\033[0;31m"); // set the color to red 
                perror("exec failure !!");
                printf("\033[0m"); // reset the color to the default
                exit(-7);
            } else { //Parent process
                team2[z].pid = pid;
            }
        }

        //=====================================================================================================================//
        //                                                  Write on pipe                                                      //
        //=====================================================================================================================// 
        for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
            sprintf(writemessages[i], "%d %d", team1[i].location, team1[i].pid);
            sprintf(writemessages2[i], "%d %d", team2[i].location, team2[i].pid);
        }

        for (int z = 0; z < NUMBER_OF_PLAYERS / 2; z++) {
            for (int j = 0; j < NUMBER_OF_PLAYERS / 2; j++) {
                //printf("Parent Process - Writing to pipe - Message 1 is %s\n", writemessages[j]);
                write(pipefds[z][1], writemessages[j], sizeof(writemessages[j]));

                //printf("Parent Process - Writing to pipe **- Message 1 is %s\n", writemessages2[j]);
                write(pipefds[z + (NUMBER_OF_PLAYERS / 2)][1], writemessages2[j], sizeof(writemessages2[j]));
            }
        }

        //=====================================================================================================================//
        //                                                   Start new round                                                   //
        //=====================================================================================================================// 
        // Wait until all palyers read his pipe and know the next player ID
        while (number_of_ready_players != NUMBER_OF_PLAYERS) {}
        printf("=============================================================================\n");
        printf("Round %d Start\n\n", current_round);
        fflush(stdout);
        // Get the pid of the player in the first location and send SIGUSR1 to start running 
        pid_t player11; // PID for the first player in team 1
        pid_t player21; // PID for the first player in team 2
        for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
            if (team1[i].location == 1) {
                player11 = team1[i].pid;
                break;
            }
        }
        for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
            if (team2[i].location == 1) {
                player21 = team2[i].pid;
                break;
            }
        }
        //printf("The 11 --> %d\n",player11);
        //printf("The 11 --> %d\n",player21);
        if (kill(player11, SIGUSR1) == -1) {
            printf("\033[0;31m"); // set the color to red 
            perror("Kill failure !!");
            printf("\033[0m"); // reset the color to the default
            exit(-8);
        }
        if (kill(player21, SIGUSR1) == -1) {
            printf("\033[0;31m"); // set the color to red 
            perror("Kill failure !!");
            printf("\033[0m"); // reset the color to the default
            exit(-9);
        }
        //=====================================================================================================================//
        //                                         Make the parent wait his children                                           //
        //=====================================================================================================================//  
        for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
            waitpid(team1[i].pid, & stat, 0);
        }

        for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
            waitpid(team2[i].pid, & stat, 0);
        }

        //=====================================================================================================================//
        //                                                        Close pipes                                                  //
        //=====================================================================================================================//     
        for (int q = 0; q < NUMBER_OF_PLAYERS; q++) {
            close(pipefds[q][1]);
            close(pipefds[q][0]);
        }
    }
    //************************************************************************************************************************
    //=====================================================================================================================//
    //                                                   Print winner                                                      //
    //=====================================================================================================================//        
    game_winner();

    printf("DONE\n");
    return (0);
}

//===========================================================================================================================================
// Function to change speed, we will use it to change speed for each player at the beginning of new round

int change_speed(int initialspead) { // the speed for each player depends in both the level of content in the bag and the time.
    int new_speed;
    float factor = float_rand(0.0, 3.0);
    new_speed = initialspead - factor;
    return new_speed;
}

//===========================================================================================================================================
// Function to shuffle the element of array

int * shuffle(size_t n) {
    static int array[NUMBER_OF_PLAYERS / 2];
    for (int x = 0; x < n; x++) {
        array[x] = x + 1;

    }

    if (current_round > 1) {
        if (n > 1) {
            size_t i;
            for (i = 0; i < n; i++) {
                size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
                int t = array[j];
                array[j] = array[i];
                array[i] = t;
            }
        }
    }
    return array;
}

//==============================================================================================================================================
// If a quit signal sent by any player is received by the parent, the number of ready playes is increamented 

void sig_quit_catcher(int sig_num) {
    number_of_ready_players += 1;
}
//==============================================================================================================================================
// If a user1 signal sent by the last player in team 1 received by the parent, the number of round of winnig for team1 is increamented 

void sig_user1_catcher(int sig_num) {
    team1_winning++;
    printf("Team 1 win in the round %d (%d Vs %d)\n\n", current_round, team1_winning, team2_winning);
    kill_AllPlayers();
}
//==============================================================================================================================================
// If a user2 signal sent by the last player in team 2 received by the parent, the number of round of winnig for team2 is increamented 

void sig_user2_catcher(int sig_num) {
    team2_winning++;
    printf("Team 2 win in the round %d (%d Vs %d) \n\n", current_round, team1_winning, team2_winning);
    kill_AllPlayers();
}
//==============================================================================================================================================
// Function to kill all players at the end of the round

void kill_AllPlayers() {

    //kill all players just in case to restart
    for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
        if (kill(team1[i].pid, SIGTERM) == -1) {
            perror("Kill failure !!");
            exit(-11);
        }
    }

    for (int i = 0; i < NUMBER_OF_PLAYERS / 2; i++) {
        if (kill(team2[i].pid, SIGTERM) == -1) {
            perror("Kill failure !!");
            exit(-12);
        }
    }

}
//==============================================================================================================================================
// Function to determine which team win in the game 

void game_winner() {
    printf("%d___%d\n",team1_winning,team2_winning);
    printf("\033[1m");
    printf("\033[5;33m"); // set the color to yellow
    printf("GAME OVER :)\n\n");
    printf("\033[0m");

    if (team1_winning > team2_winning) {
        printf("\n|---------------------------------------------------------------------------------------- ");
        printf("\033[1;31m");
        printf("\n ---> Team1 :) is the winner in the game and they won in (%d) rounds <----", team1_winning);
        printf("\033[0m");
        printf("\n|---------------------------------------------------------------------------------------- \n");
    } else if (team1_winning > team2_winning) {
        printf("\n|----------------------------------------------------------------------------------------|");
        printf("\033[0;32m");
        printf("\n ---> Team2 :) is the winner in the game and they won in (%d) rounds <---", team2_winning);
        printf("\033[0m");
        printf("\n|----------------------------------------------------------------------------------------| \n");
    }
    else if (team1_winning == team2_winning) {
        printf("\n|----------------------------------------------------------------------------------------|");
        printf("\033[0;32m");
        printf("\n ---> Two teams win :) result --> (%d Vs %d) rounds <---",team1_winning, team2_winning);
        printf("\033[0m");
        printf("\n|----------------------------------------------------------------------------------------| \n");
    }
}
//==============================================================================================================================================
void Project_interface (){
printf ("\n|==============================================================================|");
printf(" \n|                           The big race game                                  |");
printf(" \n|==============================================================================|");
printf(" \n|Done by : Enas Murrar 1181390  | 1182012 Aseel Khaseeb                        |");
printf(" \n|          Rand Farhoud 1181567 | Diaa Maali 1180933                           |");
printf(" \n|______________________________________________________________________________|");
printf("\033[1;36m"); //set the color to cyan, and make the text bold
printf(" \n|\t        We Are Starting This INTERESTING Game!                         |\n");
printf("\033[0m");    // reset the color to the default
}

