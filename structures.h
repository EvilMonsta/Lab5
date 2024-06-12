typedef struct equipment {
    char bodyType[100];
    float engineCapacity;
    int startReleaseYear;
    int endReleaseYear;
} Equipment;

typedef struct car {
    char *model;
    Equipment *equipments;
    int equipmentsCount;
} Car;

typedef struct treeNode {
    Car* carModel;
    struct treeNode* leftNode;
    struct treeNode* rightNode;
} TreeNode;

typedef struct bodyType {
    char name[100];
    int allLifeCycles;
    int repeats;
    float typicalLifeCycle;
}BodyType;

typedef struct node {
    Car* carModel;
    struct node* next;
} Node;


typedef struct hashMap {
    unsigned int bucketCount;
    unsigned int modelsNumber;
    Node **nodes;
} HashMap;
