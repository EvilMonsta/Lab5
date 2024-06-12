#include <stdio.h>
#include"structures.h"
#include<string.h>
#include<malloc.h>
#include <limits.h>
#include <stdint.h>

void freeModel(Car *carModel) {
    free(carModel->model);
    free(carModel->equipments);
    free(carModel);
}

void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->leftNode);
    freeTree(root->rightNode);
    freeModel(root->carModel);
    free(root);
}

int TaskNumCheck(int tasknum) {
    if (tasknum != 1 && tasknum != 2) {
        printf("ERROR");
        return 0;
    }
    return tasknum;
}

int checkInt() {
    int number;
    while (scanf("%d", &number) == 0 || getchar() != '\n') {
        printf("\nincorrect input!");
        rewind(stdin);
    }
    return number;
}

float checkFloat() {
    float number;
    while (scanf("%f", &number) == 0 || getchar() != '\n') {
        printf("\nincorrect input!");
        rewind(stdin);
    }
    return number;
}

int ContinueWish(int tasknumber) {
    int wish = tasknumber;
    if (tasknumber == 0) {
        return wish;
    }
    printf("\nWant to continue work with program?\n1 - Yes\n0 - No\nYour choice: ");
    scanf("%d", &wish);
    if (wish != 0 && wish != 1) {
        printf("ERROR");
        return 0;
    }
    return wish;
}

void WordCancellation(char *string, int length) {
    for (int i = 0; i < length; i++) {
        string[i] = '\0';
    }
}

int findStringNumber(FILE *taskFile, char *buffer) {
    int stringCounter = 0;
    while ((fgets(buffer, 100, taskFile)) != NULL) {
        stringCounter++;
    }
    rewind(taskFile);
    return stringCounter;
}

void readFromStrToStruct(char *string, Car *newModel) {
    int flag = 1, wordCounter = 0, wordLength = 0;
    char word[100];
    newModel->equipments = malloc(sizeof(Equipment));
    newModel->equipmentsCount = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == ' ' || string[i] == '\0' || string[i] == '\n') {
            wordCounter++;
            int j = (wordCounter - 1) / 4;
            word[wordLength] = '\0';
            if (flag == 1) {
                newModel->equipmentsCount++;
                newModel->equipments = realloc(newModel->equipments, newModel->equipmentsCount * sizeof(Equipment));
                strcpy(newModel->equipments[j].bodyType, word);
            }
            if (flag == 2) newModel->equipments[j].engineCapacity = strtof(word, NULL);
            if (flag == 3) newModel->equipments[j].startReleaseYear = atoi(word);
            if (flag == 4) {
                newModel->equipments[j].endReleaseYear = atoi(word);
            }
            if (flag != 4) flag++;
            else flag = 1;
            WordCancellation(word, wordLength);
            wordLength = 0;
        } else {
            wordLength++;
            word[wordLength - 1] = string[i];
        }
    }

}

TreeNode *insertNode(TreeNode *root, Car *newModel) {
    if (root == NULL) {
        TreeNode *newNode = malloc(sizeof(TreeNode));
        newNode->carModel = newModel;
        newNode->leftNode = NULL;
        newNode->rightNode = NULL;
        return newNode;
    }
    int cmp = strcmp(newModel->model, root->carModel->model);
    if (cmp < 0) {
        root->leftNode = insertNode(root->leftNode, newModel);
    } else if (cmp >= 0) {
        root->rightNode = insertNode(root->rightNode, newModel);
    }
    return root;
}

TreeNode *writeDataFromFile() {
    TreeNode *rootCar = NULL;
    char buffer[100], currentString[100], currentModel[100];
    FILE *file = fopen("taskfile.txt", "rb");
    int stringLength = findStringNumber(file, buffer), currentCar = 0;
    for (int i = 0; i < stringLength / 2; i++) {
        Car *newModel = malloc(sizeof(Car));
        fgets(currentString, 100, file);
        for (int j = 0; j < (int) (strlen(currentString) - 1); j++)
            currentModel[j] = currentString[j];
        currentModel[(int) (strlen(currentString) - 1)] = '\0';
        newModel->model = strdup(currentModel);
        fgets(currentString, 100, file);
        readFromStrToStruct(currentString, newModel);
        rootCar = insertNode(rootCar, newModel);
    }
    return rootCar;
}

TreeNode *search(TreeNode *rootNode, char *searchingModel) {
    if (rootNode == NULL) {
        return NULL;
    }

    while (rootNode != NULL) {
        int cmp = strcmp(rootNode->carModel->model, searchingModel);
        if (cmp > 0) {
            rootNode = rootNode->leftNode;
        } else if (cmp < 0) {
            rootNode = rootNode->rightNode;
        } else return rootNode;

    }
    return NULL;
}

Car *findCar(TreeNode *rootNode, char *model) {
    TreeNode *fModel = NULL;
    if (rootNode != NULL) {
        fModel = search(rootNode, model);
        if (fModel != NULL) {
            return fModel->carModel;
        }
    } else {
        return NULL;
    }
}

void enterEq(Car *carModel, int oldCount) {
    for (int i = oldCount; i < carModel->equipmentsCount; i++) {
        printf("Enter type of model body:\n");
        scanf("%s", carModel->equipments[i].bodyType);
        printf("Enter engine capacity:\n");
        scanf("%f", &carModel->equipments[i].engineCapacity);
        printf("Enter year of start production:\n");
        scanf("%d", &carModel->equipments[i].startReleaseYear);
        printf("Enter end year of production:\n");
        scanf("%d", &carModel->equipments[i].endReleaseYear);
    }
}

TreeNode *addNewCar(TreeNode *rootNode) {
    char buf, buffer[100];
    int size = 0, oldCount;
    Car *newCar = malloc(sizeof(Car)), *exCar;
    printf("Enter model name:\n");
    fflush(stdin);
    while ((buf = getchar()) != '\n') {
        size++;
        buffer[size - 1] = buf;
    }
    buffer[size] = '\0';
    newCar->model = strdup(buffer);
    printf("Enter equipments number:\n");
    scanf("%d", &newCar->equipmentsCount);
    exCar = findCar(rootNode, buffer);
    if (exCar == NULL) {
        newCar->equipments = malloc(newCar->equipmentsCount * sizeof(Equipment));
        enterEq(newCar, 0);
        rootNode = insertNode(rootNode, newCar);
    } else {
        oldCount = exCar->equipmentsCount;
        exCar->equipmentsCount += newCar->equipmentsCount;
        exCar->equipments = realloc(exCar->equipments, exCar->equipmentsCount * sizeof(Equipment));
        enterEq(exCar, oldCount);
        free(newCar);
    }
    return rootNode;
}

TreeNode *delete(TreeNode *rootNode) {
    TreeNode *tempNode;

    if (rootNode->leftNode == NULL && rootNode->rightNode == NULL) {
        freeModel(rootNode->carModel);
        free(rootNode);
        return NULL;
    } else if (rootNode->leftNode == NULL) {
        tempNode = rootNode->rightNode;
        freeModel(rootNode->carModel);
        free(rootNode);
        return tempNode;
    } else if (rootNode->rightNode == NULL) {
        tempNode = rootNode->leftNode;
        freeModel(rootNode->carModel);
        free(rootNode);
        return tempNode;
    } else {
        tempNode = rootNode->rightNode;
        while (tempNode->leftNode != NULL) {
            tempNode = tempNode->leftNode;
        }
        tempNode->leftNode = rootNode->leftNode;
        tempNode = rootNode->rightNode;
        freeModel(rootNode->carModel);
        free(rootNode);
        return tempNode;
    }
}

char *modelInput() {
    char *model = malloc(sizeof(char)), ch, size = 0;
    printf("Enter model:");
    fflush(stdin);
    while ((ch = getchar()) != '\n') {
        size++;
        model = realloc(model, (size + 1));
        model[size - 1] = ch;
    }
    model[size] = '\0';
    return model;
}

TreeNode *findAndDelete(TreeNode *rootNode, char *searchingModel) {
    if (rootNode == NULL) {
        return NULL;
    }
    TreeNode *root = rootNode;
    while (rootNode != NULL) {
        int cmp = strcmp(rootNode->carModel->model, searchingModel);
        if (rootNode->leftNode != NULL) {
            int cmpl = strcmp(rootNode->leftNode->carModel->model, searchingModel);
            if (!cmpl) {
                rootNode->leftNode = delete(rootNode->leftNode);
                return root;
            }
        }
        if (rootNode->rightNode != NULL) {

            int cmpr = strcmp(rootNode->rightNode->carModel->model, searchingModel);
            if (!cmpr) {
                rootNode->rightNode = delete(rootNode->rightNode);
                return root;
            }
        }
        if (cmp > 0) {
            rootNode = rootNode->leftNode;
        } else if (cmp < 0) {
            rootNode = rootNode->rightNode;
        } else {
            rootNode = delete(rootNode);
            return rootNode;
        }
    }
    printf("\nNot found\n");
    return root;
}

void printTree(TreeNode *root, int space) {
    if (root == NULL) {
        return;
    }
    space += 15;
    printTree(root->rightNode, space);
    for (int i = 10; i < space; i++) {
        printf(" ");
    }
    printf("%s\n", root->carModel->model);
    printTree(root->leftNode, space);
}

void printTreeList(TreeNode *root) {

    if (root == NULL) {
        return;
    }

    printf("model: %s\n", root->carModel->model);
    printf("equipments:\n");
    for (int j = 0; j < root->carModel->equipmentsCount; j++) {
        printf("  type: %s\n", root->carModel->equipments[j].bodyType);
        printf("  engine capacity: %.2f\n", root->carModel->equipments[j].engineCapacity);
        printf("  release year: %d\n", root->carModel->equipments[j].startReleaseYear);
        printf("  produce end year: %d\n", root->carModel->equipments[j].endReleaseYear);
        printf("\n");
    }

    printTreeList(root->rightNode);

    printTreeList(root->leftNode);

}

TreeNode *deleteCar(TreeNode *root) {

    char *carToDelete;
    if (root != NULL) {
        carToDelete = modelInput();
        if (root != NULL) {
            root = findAndDelete(root, carToDelete);
            printf("\nDELETED\n");
        }
        free(carToDelete);
    } else {
        printf("\nNo cars left\n");
    }
    return root;
}

BodyType *fillList(TreeNode *rootNode, BodyType *list, int *typesNum) {

    int flag;
    if (rootNode == NULL) {
        return list;
    }
    for (int i = 0; i < rootNode->carModel->equipmentsCount; i++) {
        flag = 0;
        for (int j = 0; j < *typesNum; j++) {
            if (!strcmp(rootNode->carModel->equipments[i].bodyType, list[j].name)) {
                list[j].repeats++;
                list[j].allLifeCycles += (rootNode->carModel->equipments[i].endReleaseYear -
                                          rootNode->carModel->equipments[i].startReleaseYear);
                flag = 1;
            }
        }
        if (flag == 0) {
            (*typesNum)++;
            list = realloc(list, (*typesNum) * sizeof(BodyType));
            strcpy(list[*typesNum - 1].name, rootNode->carModel->equipments[i].bodyType);
            list[*typesNum - 1].repeats = 1;
            list[*typesNum - 1].allLifeCycles = (rootNode->carModel->equipments[i].endReleaseYear -
                                                 rootNode->carModel->equipments[i].startReleaseYear);
        }
    }
    if (rootNode->leftNode != NULL) {
        list = fillList(rootNode->leftNode, list, typesNum);
    }
    if (rootNode->rightNode != NULL) {
        list = fillList(rootNode->rightNode, list, typesNum);
    }
    return list;
}

void bubbleSort(BodyType *list, int typesNum) {
    int tempRepeats, tempAllLifeCycles;
    float tempLifeCycle;
    char tempType[100];
    for (int i = typesNum - 1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            if (list[j].typicalLifeCycle < list[j + 1].typicalLifeCycle) {
                tempAllLifeCycles = list[j].allLifeCycles;
                tempLifeCycle = list[j].typicalLifeCycle;
                tempRepeats = list[j].repeats;
                strcpy(tempType, list[j].name);
                list[j].allLifeCycles = list[j + 1].allLifeCycles;
                list[j].typicalLifeCycle = list[j + 1].typicalLifeCycle;
                list[j].repeats = list[j + 1].repeats;
                strcpy(list[j].name, list[j + 1].name);
                list[j + 1].allLifeCycles = tempAllLifeCycles;
                list[j + 1].typicalLifeCycle = tempLifeCycle;
                list[j + 1].repeats = tempRepeats;
                strcpy(list[j + 1].name, tempType);
            }
        }
    }
}

void printList(BodyType *list, int typesNum) {
    printf("\n");
    for (int i = 0; i < typesNum; i++) {
        printf("%d. %s - %0.1f\n", i, list[i].name, list[i].typicalLifeCycle);
    }
}

BodyType *createList(TreeNode *rootNode, BodyType *list, int *typesNum) {

    list = fillList(rootNode, list, typesNum);
    for (int i = 0; i < *typesNum; i++) {
        list[i].typicalLifeCycle = (float) list[i].allLifeCycles / (float) list[i].repeats;
    }
    bubbleSort(list, *typesNum);
    return list;
}

void writeTreeToFile(TreeNode *rootNode, FILE *file) {
    if (rootNode == NULL) {
        return;
    }

    fprintf(file, "%s%c", rootNode->carModel->model, '\n');
    for (int i = 0; i < rootNode->carModel->equipmentsCount; i++) {
        fprintf(file, "%s %.1f %d %d", rootNode->carModel->equipments[i].bodyType,
                rootNode->carModel->equipments[i].engineCapacity, rootNode->carModel->equipments[i].startReleaseYear,
                rootNode->carModel->equipments[i].endReleaseYear);
        if (i != rootNode->carModel->equipmentsCount - 1) {
            fprintf(file, "%c", ' ');
        }
    }
    fprintf(file, "%c", '\n');

    if (rootNode->leftNode != NULL) {
        writeTreeToFile(rootNode->leftNode, file);
    }
    if (rootNode->rightNode != NULL) {
        writeTreeToFile(rootNode->rightNode, file);
    }
}

void writeToFile(TreeNode *rootNode) {
    FILE *file = fopen("taskfile.txt", "wb");
    if (rootNode != NULL) {
        writeTreeToFile(rootNode, file);
    }
    fclose(file);
}

int findMinLifeCycle(TreeNode *rootNode, int *minLifeCycle) {

    for (int i = 0; i < rootNode->carModel->equipmentsCount; i++) {
        if (rootNode->carModel->equipments[i].endReleaseYear - rootNode->carModel->equipments[i].startReleaseYear <
            *minLifeCycle) {
            *minLifeCycle = rootNode->carModel->equipments[i].endReleaseYear -
                            rootNode->carModel->equipments[i].startReleaseYear;
        }
    }

    if (rootNode->leftNode != NULL) {
        findMinLifeCycle(rootNode->leftNode, minLifeCycle);
    }
    if (rootNode->rightNode != NULL) {
        findMinLifeCycle(rootNode->rightNode, minLifeCycle);
    }
    return *minLifeCycle;
}

TreeNode *findAndDeleteByMinCycle(TreeNode *rootNode, int minLifeCycle) {

    if (rootNode->leftNode != NULL) {
        rootNode->leftNode = findAndDeleteByMinCycle(rootNode->leftNode, minLifeCycle);
    }
    if (rootNode->rightNode != NULL) {
        rootNode->rightNode = findAndDeleteByMinCycle(rootNode->rightNode, minLifeCycle);
    }


    for (int i = 0; i < rootNode->carModel->equipmentsCount; i++) {
        if (rootNode->carModel->equipments[i].endReleaseYear - rootNode->carModel->equipments[i].startReleaseYear ==
            minLifeCycle) {
            rootNode = delete(rootNode);
            break;
        }
    }
    return rootNode;
}

TreeNode *deleteWithMinCycle(TreeNode *rootNode) {
    int minLifeCycle = INT_MAX;
    if (rootNode == NULL) {
        printf("\nNo cars left!\n");
        return NULL;
    }
    minLifeCycle = findMinLifeCycle(rootNode, &minLifeCycle);
    rootNode = findAndDeleteByMinCycle(rootNode, minLifeCycle);
    return rootNode;
}

void printCar(TreeNode *model) {

    printf("\n");
    for (int i = 0; i < model->carModel->equipmentsCount; i++) {
        printf("%d.\nBody type: %s \nEngine capacity: %.1f \nRelease year: %d \nYear of production end: %d\n", i + 1,
               model->carModel->equipments[i].bodyType, model->carModel->equipments[i].engineCapacity,
               model->carModel->equipments[i].startReleaseYear, model->carModel->equipments[i].endReleaseYear);
    }

}

void findCarByName(TreeNode *rootNode) {
    TreeNode *fModel = NULL;
    char *searchingNode;
    if (rootNode != NULL) {
        searchingNode = modelInput();
        fModel = search(rootNode, searchingNode);
        if (fModel != NULL) {
            printCar(fModel);
        } else {
            printf("\nNot found\n");
        }
        free(searchingNode);
    } else {
        printf("\nNo cars left\n");
    }
}


float findAverageCapacity(Car *carModel) {
    float avCap = 0;

    for (int j = 0; j < carModel->equipmentsCount; j++) {
        avCap += carModel->equipments[j].engineCapacity;
    }
    avCap = avCap / (float) carModel->equipmentsCount;

    return avCap;
}

unsigned int hash(float value) {

    unsigned int hash = (unsigned int) ((value * 100) / 5 + value * 100 + 17 + value / 2);

    return hash;

}

HashMap *createHashMap(int startSize) {
    HashMap *hashMap = (HashMap *) malloc(sizeof(HashMap));
    hashMap->nodes = (Node **) calloc(startSize, sizeof(Node *));
    hashMap->bucketCount = startSize;
    hashMap->modelsNumber = 0;
    return hashMap;
}

void resizeHashMap(HashMap *hashMap) {
    unsigned int oldSize = hashMap->bucketCount;
    Node **oldNodes = hashMap->nodes;

    hashMap->bucketCount *= 2;
    hashMap->nodes = (Node **) calloc(hashMap->bucketCount, sizeof(Node *));

    for (int i = 0; i < oldSize; i++) {
        Node *node = oldNodes[i];
        while (node != NULL) {
            Node *nextNode = node->next;
            unsigned int index = hash(findAverageCapacity(node->carModel)) % hashMap->bucketCount;
            node->next = hashMap->nodes[index];
            hashMap->nodes[index] = node;
            node = nextNode;
        }
    }

    free(oldNodes);
}

void addModel(HashMap *hashMap, Car *carModel) {

    if (hashMap->modelsNumber == hashMap->bucketCount) {
        resizeHashMap(hashMap);
    }

    unsigned int index = hash(findAverageCapacity(carModel)) % hashMap->bucketCount;

    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->carModel = carModel;
    newNode->next = hashMap->nodes[index];
    hashMap->nodes[index] = newNode;
    hashMap->modelsNumber++;
}

void addNewModel(HashMap *hashMap) {
    char buf, buffer[100];
    int size = 0;
    Car *newCar = malloc(sizeof(Car));
    printf("Enter model name:\n");
    fflush(stdin);
    while ((buf = getchar()) != '\n') {
        size++;
        buffer[size - 1] = buf;
    }
    buffer[size] = '\0';
    newCar->model = strdup(buffer);
    printf("Enter equipments number:\n");
    scanf("%d", &newCar->equipmentsCount);
    newCar->equipments = malloc(newCar->equipmentsCount * sizeof(Equipment));
    for (int i = 0; i < newCar->equipmentsCount; i++) {
        printf("Enter type of model body:\n");
        scanf("%s", newCar->equipments[i].bodyType);
        printf("Enter engine capacity:\n");
        scanf("%f", &newCar->equipments[i].engineCapacity);
        printf("Enter year of start production:\n");
        scanf("%d", &newCar->equipments[i].startReleaseYear);
        printf("Enter end year of production:\n");
        scanf("%d", &newCar->equipments[i].endReleaseYear);
    }
    addModel(hashMap, newCar);
}


void deleteModel(HashMap *hashMap, float avCap) {
    unsigned int index = hash(avCap) % hashMap->bucketCount;
    Node *node = hashMap->nodes[index];
    Node *prev = NULL;
    while (node != NULL) {
        if (findAverageCapacity(node->carModel) == avCap) {
            if (prev == NULL) {
                hashMap->nodes[index] = node->next;
            } else {
                prev->next = node->next;
            }
            node = NULL;
            free(node);
            hashMap->modelsNumber--;
            printf("\nDELETED\n");
            return;
        }
        prev = node;
        node = node->next;
    }
    printf("\nNot found\n");
}

void printAverageCapList(HashMap *hashMap) {

    printf("\n");

    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {
            printf("model: %s - average capacity: %0.3f\n", node->carModel->model, findAverageCapacity(node->carModel));
            node = node->next;
        }
    }

    printf("\n");

}

void destroyCar(HashMap *hashMap) {

    float carToDelete;
    if (hashMap->modelsNumber != 0) {
        printAverageCapList(hashMap);
        printf("Enter average capacity to delete:");
        carToDelete = checkFloat();
        deleteModel(hashMap, carToDelete);
    } else {
        printf("\nNo cars left\n");
    }
}

void readDataFromFile(HashMap *hashMap) {
    char buffer[100], currentString[100], currentModel[100];
    FILE *file = fopen("taskfile.txt", "rb");

    int stringLength = findStringNumber(file, buffer);
    for (int i = 0; i < stringLength / 2; i++) {
        Car *newModel = malloc(sizeof(Car));
        fgets(currentString, 100, file);
        for (int j = 0; j < (int) (strlen(currentString) - 1); j++)
            currentModel[j] = currentString[j];
        currentModel[(int) (strlen(currentString) - 1)] = '\0';
        newModel->model = strdup(currentModel);
        fgets(currentString, 100, file);
        readFromStrToStruct(currentString, newModel);
        addModel(hashMap, newModel);
    }
    fclose(file);
}

void printHashMap(HashMap *hashMap) {
    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {
            printf("[%d]. ", i);
            printf("model: %s\n", node->carModel->model);
            printf("equipments:\n");
            for (int j = 0; j < node->carModel->equipmentsCount; j++) {
                printf("  type: %s\n", node->carModel->equipments[j].bodyType);
                printf("  engine capacity: %.2f\n", node->carModel->equipments[j].engineCapacity);
                printf("  release year: %d\n", node->carModel->equipments[j].startReleaseYear);
                printf("  produce end year: %d\n", node->carModel->equipments[j].endReleaseYear);
                printf("-----------------------------\n", i);
            }
            node = node->next;
        }
    }
}

void clearHashMap(HashMap *hashMap) {
    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {
            Node *nextNode = node->next;
            freeModel(node->carModel);
            free(node);
            node = nextNode;
        }
    }
    free(hashMap->nodes);
    hashMap->nodes = NULL;
    hashMap->bucketCount = 0;
    hashMap->modelsNumber = 0;
}

BodyType *fillListHash(HashMap *hashMap, BodyType *list, int *typesNum) {

    int flag;
    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {

            for (int j = 0; j < node->carModel->equipmentsCount; j++) {
                flag = 0;
                for (int k = 0; k < *typesNum; k++) {
                    if (!strcmp(node->carModel->equipments[j].bodyType, list[k].name)) {
                        list[k].repeats++;
                        list[k].allLifeCycles += (node->carModel->equipments[j].endReleaseYear -
                                                  node->carModel->equipments[j].startReleaseYear);
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    (*typesNum)++;
                    list = realloc(list, (*typesNum) * sizeof(BodyType));
                    strcpy(list[*typesNum - 1].name, node->carModel->equipments[j].bodyType);
                    list[*typesNum - 1].repeats = 1;
                    list[*typesNum - 1].allLifeCycles = (node->carModel->equipments[j].endReleaseYear -
                                                         node->carModel->equipments[j].startReleaseYear);
                }
            }

            node = node->next;
        }
    }

    return list;
}

BodyType *createListHash(HashMap *hashMap, BodyType *list, int *typesNum) {

    list = fillListHash(hashMap, list, typesNum);
    for (int i = 0; i < *typesNum; i++) {
        list[i].typicalLifeCycle = (float) list[i].allLifeCycles / (float) list[i].repeats;
    }
    bubbleSort(list, *typesNum);
    return list;
}


int findMinLifeCycleHash(HashMap *hashMap, int *minLifeCycle) {
    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {

            for (int j = 0; j < node->carModel->equipmentsCount; j++) {
                if (node->carModel->equipments[j].endReleaseYear - node->carModel->equipments[j].startReleaseYear <
                    *minLifeCycle) {
                    *minLifeCycle = node->carModel->equipments[j].endReleaseYear -
                                    node->carModel->equipments[j].startReleaseYear;
                }
            }
            node = node->next;
        }
    }
    return *minLifeCycle;
}

void findAndDelMin(HashMap *hashMap, int minLifeCycle) {
    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {
            for (int j = 0; j < node->carModel->equipmentsCount; j++) {
                if (node->carModel->equipments[j].endReleaseYear - node->carModel->equipments[j].startReleaseYear ==
                    minLifeCycle) {
                    deleteModel(hashMap, findAverageCapacity(node->carModel));
                    break;
                }
            }
            node = node->next;
        }
    }
}

void deleteWithMinCycleHash(HashMap *hashMap) {
    int minLifeCycle = INT_MAX;
    if (hashMap->bucketCount == 0) {
        printf("\nNo cars left!\n");
        return;
    }
    minLifeCycle = findMinLifeCycleHash(hashMap, &minLifeCycle);
    findAndDelMin(hashMap, minLifeCycle);
}


void printModel(Node *nodeToPrint) {

    printf("\nmodel: %s\n", nodeToPrint->carModel->model);
    printf("equipments:\n");
    for (int j = 0; j < nodeToPrint->carModel->equipmentsCount; j++) {
        printf("  type: %s\n", nodeToPrint->carModel->equipments[j].bodyType);
        printf("  engine capacity: %.2f\n", nodeToPrint->carModel->equipments[j].engineCapacity);
        printf("  release year: %d\n", nodeToPrint->carModel->equipments[j].startReleaseYear);
        printf("  produce end year: %d\n", nodeToPrint->carModel->equipments[j].endReleaseYear);
        printf("-----------------------------\n");

    }
}

void findAndPrintModel(HashMap *hashMap, float avCap) {
    float mean, flag = 0;
    unsigned int index = hash(avCap) % hashMap->bucketCount;

    Node *node = hashMap->nodes[index];

    while (node != NULL) {
        if (findAverageCapacity(node->carModel) == avCap) {
            printModel(node);
            return;
        }
        node = node->next;
    }
    printf("Not found\n");
}

void findByAverageCapacity(HashMap *hashMap) {
    float avCap;
    printf("Enter average engine capacity:\n");
    avCap = checkFloat();
    findAndPrintModel(hashMap, avCap);
}


void writeHashToFile(HashMap *hashMap, FILE *file) {

    for (int i = 0; i < hashMap->bucketCount; i++) {
        Node *node = hashMap->nodes[i];
        while (node != NULL) {
            fprintf(file, "%s%c", node->carModel->model, '\n');

            for (int j = 0; j < node->carModel->equipmentsCount; j++) {
                fprintf(file, "%s %.1f %d %d", node->carModel->equipments[j].bodyType,
                        node->carModel->equipments[j].engineCapacity, node->carModel->equipments[j].startReleaseYear,
                        node->carModel->equipments[j].endReleaseYear);
                if (j != node->carModel->equipmentsCount - 1) {
                    fprintf(file, "%c", ' ');
                }
            }
            fprintf(file, "%c", '\n');

            node = node->next;
        }
    }
}


void writeDataToFile(HashMap *hashMap) {
    FILE *file = fopen("taskfile.txt", "wb");
    writeHashToFile(hashMap, file);
    fclose(file);
}

//audi
//sedan 1.8 1980 2000
//lada
//SUV 1.9 2000 2010 sedan 3.0 2019 2029
//honda
//coupe 3.1 2020 2031 crossover 1.6 1800 2000
//reno
//crossover 1.2 1980 2020
//Lada grande
//SUV 1.2 2005 2015
//Lada somara
//sedan 1.5 2005 2015
//reno ne predumal
//body 1.6 1980 2070
//da
//net 2.0 1000 2000 human 3.0 0 2024 cat 100.0 0 2000
//