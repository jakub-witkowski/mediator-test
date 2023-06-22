#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "load_status.h"
#include "load_orders.h"

#define MAP_SIZE_X 32 // horizontal dimension of the map
#define MAP_SIZE_Y 32 // vertical dimension of the map
#define MAX_NUMBER_OF_UNITS 1002 // defines the maximum number of units that may be produced in 2000 rounds
#define MAX_NUMBER_OF_ROUNDS 10 // defines the total maximum number of rounds allowed

/* Variables holding the current round number for each player */
int player1_round_counter = 1;
int player2_round_counter = 1;

long gold = 0; // holds the amount of gold
au active_units[MAX_NUMBER_OF_UNITS]; // holds information on player and enemy active units;
int units_on_the_map_counter = 0; // holds the number of units currently present on the map

int main()
{
    /* analyze the results */
    load_status("status.txt", &units_on_the_map_counter, &gold, active_units); // read data from status.txt
    load_orders("rozkazy.txt", active_units); // read data from rozkazy.txt

    return 0;
}