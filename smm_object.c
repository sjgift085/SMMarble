//
//  smm_object.c
//  SMMarble
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODENR      100
#define MAX_NODETYPE    7

#define GRADE_Ap        0
#define GRADE_A0        1
#define GRADE_Am        2
#define GRADE_Bp        3
#define GRADE_B0        4
#define GRADE_Bm        5
#define GRADE_Cp        6
#define GRADE_C0        7
#define GRADE_Cm        8
#define GRADE_Dp        9
#define GRADE_D0        10
#define GRADE_Dm        11
#define GRADE_F         12


static char smmObj_NodeName[MAX_NODETYPE][MAX_CHARNAME] = {
    "lecture",
    "restaurant",
    "laboratory",
    "home",
    "gotoLab",
    "foodChance",
    "festival"
};

static char smmObj_gradeName[SMMMODE_MAX_GRADE][MAX_CHARNAME] = {
    "A+",
    "A0",
    "A-",
    "B+",
    "B0",
    "B-",
    "C+",
    "C0",
    "C-",
    "D+",
    "D0",
    "D-",
    "F"
};

//structure type definition
typedef struct{
    char name[MAX_CHARNAME];
    int objType;
    int type;
    int credit;
    int energy;
    int grade;
} smmObj_object_t;


//object generation
void* smmObj_genNode(char* name, int objType, int type, int credit, int energy, int grade)
{
    smmObj_object_t* ptr = (smmObj_object_t*) malloc(sizeof(smmObj_object_t));

    strcpy(ptr->name, name);
    ptr->type = type;
    ptr->objType = objType;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;

    return (void*)ptr;
}

void* smmObj_genObject(char* name, int objType, int type, int credit, int energy, int grade)
{
    return smmObj_genNode(name, objType, type, credit, energy, grade);
}

//member retrieving

//name
char* smmObj_getObjectName(void* obj)
{
    smmObj_object_t* ptr = (smmObj_object_t*)obj;
    return (ptr->name);
}

char* smmObj_getNodeName(void* obj)
{
 	return smmObj_getObjectName(obj);
}

/*char* smmObj_getNodeName(void* obj)
{
    smmObj_object_t* ptr = (smmObj_object_t*)obj;
    return (ptr->name);
}*/

//type
int smmObj_getNodeType(void* obj)
{
 	smmObj_object_t* ptr = (smmObj_object_t*)obj;
 	return (ptr->type);
}

//object
int smmObj_getObjectType(void* obj)
{
 	smmObj_object_t* ptr = (smmObj_object_t*)obj;
    return (ptr->objType);
}

//credit
int smmObj_getNodeCredit(void* obj)
{
 	smmObj_object_t* ptr = (smmObj_object_t*)obj;
    return (ptr->credit);
}
int smmObj_getObjectCredit(void* obj)
{
 	return smmObj_getNodeCredit(obj);
}

//energy
int smmObj_getNodeEnergy(void* obj)
{
 	smmObj_object_t* ptr = (smmObj_object_t*)obj;
 	return (ptr->energy);
}
int smmObj_getObject_Energy(void* obj)
{
    return smmObj_getNodeEnergy(obj);
}

//grade
int smmObj_getNodeGrade(void* obj)
{
 	smmObj_object_t* ptr = (smmObj_object_t*)obj;
 	return (ptr->grade);
}

char* smmObj_getTypeName(int node_type)
{
    return (smmObj_NodeName[node_type]);
}



#if 0
char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}
#endif
