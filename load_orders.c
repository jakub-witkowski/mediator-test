#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//#define MAP_SIZE_X 32
//#define MAP_SIZE_Y 32

#define OPPONENTS 8

/* attack arrays */
int knight[OPPONENTS] = { 35, 35, 35, 35, 35, 35, 35, 35 };
int swordsman[OPPONENTS] = { 30, 30, 30, 20, 20, 30, 30, 30 };
int archer[OPPONENTS] = { 15, 15, 15, 15, 10, 10, 15, 15 };
int pikeman[OPPONENTS] = { 35, 15, 15, 15, 15, 10, 15, 10 };
int catapult[OPPONENTS] = { 40, 40, 40, 40, 40, 40, 40, 50 };
int ram[OPPONENTS] = { 10, 10, 10, 10, 10, 10, 10, 50 };
int worker[OPPONENTS] = { 5, 5, 5, 5, 5, 5, 5, 1 };

/* aliases for array indices  */
const int vs_K = 0;
const int vs_S = 1;
const int vs_A = 2;
const int vs_P = 3;
const int vs_C = 4;
const int vs_R = 5;
const int vs_W = 6;
const int vs_B = 7;

typedef struct {
    char affiliation[2];
    char unit_type[2];
    int unit_id;
    int x_coord;
    int y_coord;
    int current_stamina;
    int remaining_movement;
    int attack_count;
    char is_base_busy[2];
    int training_time;
} au;

void load_orders(char fname[], au a[], char fname1[], char fname2[], int *u)
{
    /* variables used to store data read from rozkazy.txt */
    int letters = 0;
    int spaces = 0;
    int digits = 0;
    int count = 0;
    int id;
    const int length = 24;
    char order[length];
    char player_input[length];
    long gold = 0;
    char training_unit_affilitation[2];
    char training_unit_type[2];
    int training_unit_id;
    int training_unit_x;
    int training_unit_y;
    int training_unit_stamina;
    int training_time_left;
    int base_busy = 0;
    char* action;
    char type[2];
    char message[12];
    int* attacker;
    //int attacker[8];
    //int versus = 0;
    int x;
    int y;
    int target_id;

    FILE * fptr; // input: player file
    FILE * fptr1; // input file: rozkazy.txt
    FILE * fptr2; // directing output into status.txt

    fptr = fopen(fname, "r");
    if (!fptr)
        printf("Cannot find %s\n", fname);

    while (fgets(player_input, length, fptr) != NULL)
    {
        for (int i = 0; player_input[i] != '\n'; i++)  
        {
            if (isupper(player_input[i]))
            {
                letters++;
            }
            if (isdigit(player_input[i]))
            {
                digits++;
            }
            if (isblank(player_input[i]))
            {
                spaces++;
            }         
        }

        if (letters == 0 && digits >= 1 && spaces == 0)
        {
            sscanf(player_input, "%ld", &gold);
            printf("Read gold: %ld\n", gold);
            letters = 0;
            digits = 0;
            spaces = 0;
        }

        if (spaces == 6)
        {
            sscanf(player_input, "%s %s %d %d %d %d %d", training_unit_affilitation, training_unit_type, &training_unit_id, &training_unit_x, &training_unit_y, &training_unit_stamina, &training_time_left);
            printf("Read aff: %s, type: %s, id: %d, x: %d, y: %d, stamina: %d, training time left: %d\n", training_unit_affilitation, training_unit_type, training_unit_id, training_unit_x, training_unit_y, training_unit_stamina, training_time_left);
            letters = 0;
            spaces = 0;

            if ((strcmp(training_unit_affilitation, "0") == 0) && (strcmp(training_unit_type, "0") == 0))
            {
                continue;
            }
            if (((strcmp(training_unit_affilitation, "P") == 0) || (strcmp(training_unit_affilitation, "E") == 0)) && training_time_left > 0)
            {
                base_busy = 1;
            }
            if (((strcmp(training_unit_affilitation, "P") == 0) || (strcmp(training_unit_affilitation, "E") == 0)) && training_time_left == 0)
            {
                strcpy(a[training_unit_id].affiliation, training_unit_affilitation);
                strcpy(a[training_unit_id].unit_type, training_unit_type);
                a[training_unit_id].unit_id = training_unit_id;
                a[training_unit_id].x_coord = training_unit_x;
                a[training_unit_id].y_coord = training_unit_y;
                a[training_unit_id].current_stamina = training_unit_stamina;
                a[training_unit_id].training_time = training_time_left;
                base_busy = 0;
            }
        }
    }

    fclose(fptr);

    fptr1 = fopen(fname1, "r");
    if (!fptr1)
        printf("Cannot find %s\n", fname1);

    while (fgets(order, length, fptr1) != NULL)
    {
        printf("%s", order);

        for (int i = 0; order[i] != '\n'; i++)
        {
            if (isupper(order[i]))
            {
                letters++;
            }
            if (isblank(order[i]))
            {
                spaces++;
            }         
        }

        if (letters == 2)
        {
            sscanf(order, "%d B %s", &id, type);
            printf("Read base id: %d and training unit type: %s\n", id, type);
            printf("Czy to tu?\n");
            letters = 0;
            spaces = 0;
        }
        
        printf("Po bazach\n");

        if (letters == 1 && spaces == 3)
        {
            sscanf(order, "%d M %d %d", &id, &x, &y);
            printf("Read id: %d and coords: x: %d y: %d\n", id, x, y);
            letters = 0;
            spaces = 0;

            a[id].x_coord = x;
            a[id].y_coord = y;

            printf("%s %s %d %d %d %d\n", a[id].affiliation, a[id].unit_type, a[id].unit_id, a[id].x_coord, a[id].y_coord, a[id].current_stamina);
        }
        
        printf("Po ruchach\n");

        if (letters == 1 && spaces == 2)
        {
            sscanf(order, "%d A %d", &id, &target_id);
            printf("Read id: %d and target id: %d\n", id, target_id);
            letters = 0;
            spaces = 0;

            if (strcmp(a[id].unit_type, "K") == 0)
            {
                //int attacker[] = { 35, 35, 35, 35, 35, 35, 35, 35 };
                attacker = knight;
            }
            else if (strcmp(a[id].unit_type, "S") == 0)
            {
                //int attacker[] = { 30, 30, 30, 20, 20, 30, 30, 30 };
                attacker = swordsman;
            }
            else if (strcmp(a[id].unit_type, "A") == 0)
            {
                //int attacker[] = { 15, 15, 15, 15, 10, 10, 15, 15 };
                attacker = archer;
            }
            else if (strcmp(a[id].unit_type, "P") == 0)
            {
                //int attacker[] = { 35, 15, 15, 15, 15, 10, 15, 10 };
                attacker = pikeman;
            }
            else if (strcmp(a[id].unit_type, "C") == 0)
            {
                //int attacker[] = { 40, 40, 40, 40, 40, 40, 40, 50 };
                attacker = catapult;
            }
            else if (strcmp(a[id].unit_type, "R") == 0)
            {
                //int attacker[] = { 10, 10, 10, 10, 10, 10, 10, 50 };
                attacker = ram;
            }
            else if (strcmp(a[id].unit_type, "W") == 0)
            {
                //int attacker[] = { 5, 5, 5, 5, 5, 5, 5, 1 };
                attacker = worker;
            }

            if (strcmp(a[target_id].unit_type, "K") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_K]);
                a[target_id].current_stamina -= attacker[vs_K];
            }
            else if (strcmp(a[target_id].unit_type, "S") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_S]);
                a[target_id].current_stamina -= attacker[vs_S];
            }
            else if (strcmp(a[target_id].unit_type, "A") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_A]);
                a[target_id].current_stamina -= attacker[vs_A];
            }
            else if (strcmp(a[target_id].unit_type, "P") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_P]);
                a[target_id].current_stamina -= attacker[vs_P];
            }
            else if (strcmp(a[target_id].unit_type, "C") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_C]);
                a[target_id].current_stamina -= attacker[vs_C];
            }
            else if (strcmp(a[target_id].unit_type, "R") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_R]);
                a[target_id].current_stamina -= attacker[vs_R];
            }
            else if (strcmp(a[target_id].unit_type, "W") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_W]);
                a[target_id].current_stamina -= attacker[vs_W];
            }
            else if (strcmp(a[target_id].unit_type, "B") == 0)
            {
                //printf("Damage dealt: %d \n", attacker[vs_B]);
                a[target_id].current_stamina -= attacker[vs_B];
            }
            
            if ((strcmp(a[target_id].unit_type, "B") == 0) && a[target_id].current_stamina <= 0)
            {
                if (strcmp(a[target_id].affiliation, "P") == 0)
                {
                    printf("Player 1 loses the base, game over.\n");
                    exit(0);
                }
                    //strcpy(message, "Player 1");
                if (strcmp(a[target_id].affiliation, "E") == 0)
                {
                    printf("Player 2 loses the base, game over.\n");
                    exit(0);
                }
                    //strcpy(message, "Player 2");
            }

            if (a[target_id].current_stamina <= 0)
            {
                a[target_id].current_stamina = -1;
                //printf("Unit %d defeated by unit %d.\n", target_id, id);
            }
            //a[target_id].current_stamina -= attacker[versus];
            //printf("%s %s %d %d %d %d\n", a[target_id].affiliation, a[target_id].unit_type, a[target_id].unit_id, a[target_id].x_coord, a[target_id].y_coord, a[target_id].current_stamina);
        }    
        
        printf("Po walkach\n");

        //letters = 0;
        //spaces = 0;
        count++;
    }
    printf("Count: %d\n", count);

    fclose(fptr1);

    fptr2 = fopen(fname2, "w");
    if (!fptr1)
        printf("Cannot find %s\n", fname1);

    fclose(fptr2);

    fptr2 = fopen(fname2, "a");

    /* Save the amount of gold */
	if (fprintf(fptr2, "%ld\n", gold) < 0)
    {
        fprintf(stderr, "unable to write bank status\n");
    }
    printf("Gold saved.\n");

    /* Save base and unit data */
    for (int i = 0; i < *u; i++)
    {
        if (a[i].current_stamina == -1)
        {
            continue;
        }
        if (strcmp(a[i].unit_type, "B") == 0)
        {
            if (fprintf(fptr2, "%s %s %d %d %d %d %s\n", a[i].affiliation, a[i].unit_type, a[i].unit_id, a[i].x_coord, a[i].y_coord, a[i].current_stamina, a[i].is_base_busy) < 0)
            {
                fprintf(stderr, "unable to write base data\n");
            }
        }
        else
        {
            if (fprintf(fptr2, "%s %s %d %d %d %d\n", a[i].affiliation, a[i].unit_type, a[i].unit_id, a[i].x_coord, a[i].y_coord, a[i].current_stamina) < 0)
            {
                fprintf(stderr, "unable to write unit data\n");
            }
        }
    }
    printf("Units saved.\n");
    fclose(fptr2);
}