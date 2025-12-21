#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE            0
#define SMMNODE_TYPE_RESTAURANT         1
#define SMMNODE_TYPE_LABORATORY         2
#define SMMNODE_TYPE_HOME               3
#define SMMNODE_TYPE_GOTOLAB            4
#define SMMNODE_TYPE_FOODCHANCE         5
#define SMMNODE_TYPE_FESTIVAL           6

#define SMMMODE_MAX_GRADE       13

#define SMMMODE_OBJTYPE_BOARD   0
#define SMMMODE_OBJTYPE_GRADE   1
#define SMMMODE_OBJTYPE_FOOD    2
#define SMMMODE_OBJTYPE_FEST    3

//object generation
void* smmObj_genObject(char* name, int objType, int type, int credit, int energy, int grade);
void* smmObj_genNode(char* name, int objType, int type, int credit, int energy, int grade);


// member retrieving

//object name
char* smmObj_getNodeName(void* obj);
char* smmObj_getObjectName(void* obj);

//object type
int smmObj_getNodeType(void* obj);
int smmObj_getObjectType(void* obj);

//credit
int smmObj_getNodeCredit(void* obj);
int smmObj_getObjectCredit(void* obj);

//energy
int smmObj_getNodeEnergy(void* obj);    
int smmObj_getObjectEnergy(void* obj);

//grade
int smmObj_getNodeGrade(void* obj);

// element to string
char* smmObj_getTypeName(int node_type);

#endif /* smm_object_h */
