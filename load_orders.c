#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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

void load_orders(char fname[], au a[])
{
    /* variables used to store data read from rozkazy.txt */
    int letters = 0;
    int spaces = 0;
    int count = 0;
    int id = -1;
    const int length = 24;
    char order[length];
    char* action;
    char* type = "";
    int* attacker;
    //int attacker[8];
    int versus = 0;
    int x = -1;
    int y = -1;
    int target_id;

    FILE * fptr;

    fptr = fopen(fname, "r");

    while (fgets(order, length, fptr) != NULL)
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
            letters = 0;
            spaces = 0;
        }
        
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
            
            if (a[target_id].current_stamina <= 0)
            {
                a[target_id].current_stamina = -1;
                printf("Unit %d defeated by unit %d.\n", target_id, id);
            }
            //a[target_id].current_stamina -= attacker[versus];
            printf("%s %s %d %d %d %d\n", a[target_id].affiliation, a[target_id].unit_type, a[target_id].unit_id, a[target_id].x_coord, a[target_id].y_coord, a[target_id].current_stamina);
        }    
        
        //letters = 0;
        //spaces = 0;
        count++;
    }
    printf("Count: %d\n", count);

fclose(fptr);
}