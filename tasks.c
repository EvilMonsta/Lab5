#include <stdio.h>
#include"functions.h"
#include<malloc.h>
#include<string.h>

void taskOne(){
    int choice,typesNum = 0;
    TreeNode *root = writeDataFromFile();
    BodyType *list  = NULL;
    do{
        printf("\nEnter 1 to print tree\nEnter 2 to add new element to tree\nEnter 3 to delete element\nEnter 4 to create list of typical life cycles\nEnter 5 to print list\nEnter 6 to delete models with min life cycle\nEnter 7 to print data\nEnter 8 to find car by model\nEnter 0 to exit task\nChoice:");
        choice = checkInt();
        if(choice == 1){
            printTree(root,0);
        }
        if(choice == 2) {
            root = addNewCar(root);
            writeToFile(root);
        }
        if(choice == 3) {
            root = deleteCar(root);
            writeToFile(root);
        }
        if(choice == 4) {
           list = createList(root,list, &typesNum);
           printf("\nSuccess!\n");
        }
        if(choice == 5) {
            if(list == NULL) {
                printf("\nList is Empty!\n");
            } else {
                printList(list, typesNum);
            }
        }
        if(choice == 6) {
            root = deleteWithMinCycle(root);
            writeToFile(root);
            if(root != NULL){
                printf("\nSuccess!\n");
            }
        }
        if(choice == 7) {
            printTreeList(root);
        }
        if(choice == 8) {
            findCarByName(root);
        }
    }while(choice !=0);
    freeTree(root);
}

void taskTwo(){
    int choice ,typesNum = 0;
    HashMap* hashMap = createHashMap(10);
    readDataFromFile(hashMap);
    BodyType *list = malloc(sizeof(BodyType));
    list = NULL;
    do{
        printf("\nEnter 1 to print hash table\nEnter 2 to add new element to hash table\nEnter 3 to delete element\nEnter 4 to create list of typical life cycles\nEnter 5 to print list\nEnter 6 to delete models with min life cycle\nEnter 7 to print list with average cars engine capacity\nEnter 8 to find car by average capacity\nEnter 0 to exit task\nChoice:");
        choice = checkInt();
        if(choice == 1){
            printHashMap(hashMap);
        }
        if(choice == 2) {
            addNewModel(hashMap);
            writeDataToFile(hashMap);
        }
        if(choice == 3) {
            destroyCar(hashMap);
            writeDataToFile(hashMap);
        }
        if(choice == 4) {
            list = createListHash(hashMap,list, &typesNum);
            printf("\nSuccess!\n");
        }
        if(choice == 5) {
            if(list == NULL) {
                printf("\nList is Empty!\n");
            } else {
                printList(list, typesNum);
            }
        }
        if(choice == 6) {
            deleteWithMinCycleHash(hashMap);
            writeDataToFile(hashMap);
            printf("\nSuccess!\n");
        }
        if(choice == 7) {
            printAverageCapList(hashMap);
        }
        if(choice == 8) {
            findByAverageCapacity(hashMap);
        }
    }while(choice !=0);
    clearHashMap(hashMap);
}