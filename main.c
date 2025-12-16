//
//  main.c
//  SMMarble
//

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"


//board configuration parameters
static int smm_board_nr;
static int smm_food_nr;
static int smm_festival_nr;
static int smm_player_nr;

typedef struct{
    char name[MAX_CHARNAME];
    int pos;
    int credit;
    int energy;
    int flag_graduated;
    int experiment_fin;
} smm_player_t;

smm_player_t *smm_players;

void generatePlayers(int n, int initEnergy); //generate a new player
void printPlayerStatus(void); //print all player status at the beginning of each turn
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
int isGraduated(void); //check if any player is graduated
//void printGrades(int player); //print grade history of the player
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
//char* smmObj_getObjectName(void *obj);
void printGrades(int player); //print all the grade history of the player


//function prototypes
#if 0
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
#endif

void* findGrade(int player, char *lectureName)
{
    int i;
    int size = smmdb_len(LISTNO_OFFSET_GRADE + player);

    for (i = 0; i<size; i++)
    {
        void *ptr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        if(strcmp(smmObj_getObjectName(ptr), lectureName) == 0)
        {
			      //if ���ǿ� ptr != NULL && strcmp ��¼��  
            return ptr;
        }
    }
    
    return NULL;
}

void printGrades(int player) //print grade history of the player
{
 	int i;
    int size = smmdb_len(LISTNO_OFFSET_GRADE + player);
    printf("[Grade Histroy]");
    for(i=0; i<size; i++){
        void *ptr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        if(ptr != NULL){
            printf("%s(%d) ", smmObj_getObjectName(ptr), smmObj_getNodeGrade(ptr));
        }
    } 
    printf("\n");
}

int isGraduated(void)
{
    int i;

    for(i=0; i<smm_player_nr; i++){
        if(smm_players[i].flag_graduated == 1)
            return 1;
    }

    return 0;
}

void generatePlayers(int n, int initEnergy) //generate a new player
{
    int i;
    smm_players = (smm_player_t*)malloc(n*sizeof(smm_player_t));

    for(i=0; i<n; i++){
        smm_players[i].pos = 0;
        smm_players[i].credit = 0;
        smm_players[i].energy = initEnergy;
        smm_players[i].flag_graduated = 0;
        smm_players[i].experiment_fin = 0;

        printf("Input %i-th player name: ", i+1); //i <- i+1
        scanf("%s", &smm_players[i].name[0]);
        fflush(stdin);
    }
}

void printPlayerStatus(void) //print all player status at the beginning of each turn
{
    int i;

    for(i=0; i<smm_player_nr; i++){
        void *nodePtr = smmdb_getData(LISTNO_NODE, smm_players[i].pos);
		
        printf("%s - position:%i(%s), credit:%i, energy:%i\n", smm_players[i].name, smm_players[i].pos, smmObj_getNodeName(nodePtr), smm_players[i].credit, smm_players[i].energy);
    }   
}

void goForward(int player, int step) //make player go "step" steps on the board (check if player is graduated)
{
    int i;
    void *ptr;
    
    if (smm_players[player].experiment_fin > 0){
        printf("[Experimenting] Goal: %d, Rolled: %d ... ", smm_players[player].experiment_fin, step);

        if (step >= smm_players[player].experiment_fin){
            printf("Success! The experiment was successful!\n");
            smm_players[player].experiment_fin = 0;
        }
        else{
            printf("Fail! We have to do the experiment again...\n");
            return;
        }
    }

    ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
    printf("start from %i(%s)  (%i)", smm_players[player].pos, smmObj_getNodeName(ptr), step);

    for(i=0; i<step; i++){
        smm_players[player].pos = (smm_players[player].pos + 1) % smm_board_nr;
        
        ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
		printf("  => moved to %i(%s)\n", smm_players[player].pos, smmObj_getNodeName(ptr));
    }

}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
    if (c == 'g')
        printGrades(player);

    return (rand()%MAX_DIE + 1);
}


//action code when a player stays at a node
void actionNode(int player)
{
    void *ptr = smmdb_getData(LISTNO_NODE, smm_players[player].pos);
    
    int type = smmObj_getNodeType(ptr);
    int credit = smmObj_getNodeCredit(ptr);
    int energy = smmObj_getNodeEnergy(ptr);
    
    int grade;
    void *gradePtr;
    
    void *foodPtr;
    void *festPtr;
    int rand_idx;
    int food_energy;

    int k;
    void *targetPtr;

    printf(" --> player%i pos: %i, type: %s, credit: %i, energy: %i\n", player, smm_players[player].pos, smmObj_getTypeName(type), credit, energy);

    switch(type)
    {
        case SMMNODE_TYPE_LECTURE:
            if(findGrade(player, smmObj_getObjectName(ptr)) == NULL){
            smm_players[player].credit += credit;
            smm_players[player].energy -= energy;

            grade = rand() % SMMMODE_MAX_GRADE;

            gradePtr = smmObj_genObject(smmObj_getObjectName(ptr), SMMMODE_OBJTYPE_GRADE, type, credit, energy, grade);
            smmdb_addTail(LISTNO_OFFSET_GRADE+player, gradePtr);

            printf("    Type: LECTURE -> Grade acquired! (%d)\n", grade);
            }
            else{
                printf("    Type: LECTURE -> Already took this class.\n");
            }

            break;

        case SMMNODE_TYPE_RESTAURANT:
            smm_players[player].energy += energy;
            printf("    Type: RESTAURANT -> Energy recharged! (+%d)\n", energy);
            break;

        case SMMNODE_TYPE_LABORATORY:
            if(smm_players[player].experiment_fin > 0){
                smm_players[player].energy -= energy;
                printf("    Type: LABORATORY -> Experimenting...\n");
            }
            
            break;

        case SMMNODE_TYPE_HOME:
            smm_players[player].energy += energy;
            printf("    Type: HOME -> Rested. (+%d)\n", energy);
            if(smm_players[player].credit >= GRADUATE_CREDIT){
                smm_players[player].flag_graduated = 1;
                printf("!!! Player %d Graduated !!!\n", player+1);
            }
            break;

        case SMMNODE_TYPE_GOTOLAB:
            for(k=0; k<smm_board_nr; k++){
                targetPtr = smmdb_getData(LISTNO_NODE, k);
                if(smmObj_getNodeType(targetPtr) == SMMNODE_TYPE_LABORATORY){
                    smm_players[player].pos = k;
                    break;
                }
            }

            smm_players[player].experiment_fin = (rand() % MAX_DIE) + 1;

            printf("    Type: GOTO LAB -> Moved to Lab! Experiment Goal: %d\n", smm_players[player].experiment_fin);
            break;

        case SMMNODE_TYPE_FOODCHANCE:
            if(smm_food_nr > 0){
                rand_idx = rand() % smm_food_nr;
                foodPtr = smmdb_getData(LISTNO_FOODCARD, rand_idx);

                food_energy = smmObj_getNodeEnergy(foodPtr);
                //char* foodName = smmObj_getNodeName(foodPtr);

                smm_players[player].energy += food_energy;

                printf("Type: FOOD CHANCE -> Win a %s! (Energy +%d)\n", smmObj_getNodeName(foodPtr), food_energy);            
            }
            break;

        case SMMNODE_TYPE_FESTIVAL:
            if(smm_festival_nr > 0){
                rand_idx = rand() % smm_festival_nr;
                festPtr = smmdb_getData(LISTNO_FESTCARD, rand_idx);

                printf("Type: FESTIVAL -> Mission: %s\n", smmObj_getNodeName(festPtr));
            }
            break;  

        //case lecture:
        default:
            break;
    }
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int turn;

    smm_board_nr = 0;
    smm_food_nr = 0;
    smm_festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        void* ptr;
        printf("%s %i %i %i\n", name, type, credit, energy);
        ptr = smmObj_genNode(name, SMMMODE_OBJTYPE_BOARD, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, ptr);
        smm_board_nr++;
        //store the parameter set
        
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", smm_board_nr);
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        void* ptr;
        ptr = smmObj_genNode(name, SMMMODE_OBJTYPE_FOOD, 0, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, ptr);
        smm_food_nr++;
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", smm_food_nr);
     
    
#if 1
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
        void* ptr;
        ptr = smmObj_genNode(name, SMMMODE_OBJTYPE_FEST, 0, 0, 0, 0);
        smmdb_addTail(LISTNO_FESTCARD, ptr);
        smm_festival_nr++;
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", smm_festival_nr);
#endif

    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        printf("Enter the number of players : "); //check player number
        scanf("%i", &smm_player_nr);
        fflush(stdin);

        if(smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER)
            printf("Invalid playver number!\n");
    }
    while (smm_player_nr <= 0 || smm_player_nr > MAX_PLAYER);
    
    generatePlayers(smm_player_nr, smmObj_getObject_Energy(smmdb_getData(LISTNO_NODE, 0)));

    turn = 0;

    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (isGraduated() == 0) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        
        turn = (turn + 1) % smm_player_nr;
        printf("\n");
    }
    
    free(smm_players);

    system("PAUSE");
    return 0;
}
