//
//  smm_object.h
//  SMMarble object
//

#ifndef smm_object_h
#define smm_object_h

/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/

#define SMMNODE_TYPE_LECTURE            0
#define SMMNODE_TYPE_RESTAURANT         1
#define SMMNODE_TYPE_LABORATORY         2
#define SMMNODE_TYPE_HOME               3
#define SMMNODE_TYPE_GOTOLAB            4
#define SMMNODE_TYPE_FOODCHANCE         5
#define SMMNODE_TYPE_FESTIVAL           6

/* grade :
    A+,
    A0,
    A-,
    B+,
    B0,
    B-,
    C+,
    C0,
    C-
*/

#define SMMMODE_MAX_GRADE       13

#define SMMMODE_OBJTYPE_BOARD   0
#define SMMMODE_OBJTYPE_GRADE   1
#define SMMMODE_OBJTYPE_FOOD    2
#define SMMMODE_OBJTYPE_FEST    3

//object generation
void* smmObj_genObject(char* name, int objType, int type, int credit, int energy, int grade);
void* smmObj_genNode(char* name, int objType, int type, int credit, int energy, int grade);


// member retrieving
// [중요] 모든 함수 인자를 int node_nr에서 void* obj로 변경했습니다.

char* smmObj_getNodeName(void* obj);      // main.c 호출 대응
char* smmObj_getObjectName(void* obj);    // 오타 수정 (gen -> get)

int smmObj_getNodeType(void* obj);        // main.c 호출 대응
int smmObj_getObjectType(void* obj);

int smmObj_getNodeCredit(void* obj);      // main.c 호출 대응
int smmObj_getObjectCredit(void* obj);

int smmObj_getNodeEnergy(void* obj);      // main.c 호출 대응
int smmObj_getObjectEnergy(void* obj);
int smmObj_getObject_Energy(void* obj);   // main.c 초기화 부분 대응

int smmObj_getNodeGrade(void* obj);

// element to string
char* smmObj_getTypeName(int node_type);  // 타입 이름은 int로 유지

#endif /* smm_object_h */
