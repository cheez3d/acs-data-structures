#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

enum Task {
    OUTLIER_REMOVAL         = 0,
    NOISE_REMOVAL_MEDIAN    = 1,
    NOISE_REMOVAL_AVG       = 2,
    TIME_LEVELLING          = 3,
    DATA_FILLING            = 4,
    STATISTICS              = 5,
    
    UNKNOWN                 = -1
};

const char *SWITCHES[] = { "e1", "e2", "e3", "u", "c", "st" };

int comp(const void *left, const void *right) {
    size_t a = *(size_t *)left;
    size_t b = *(size_t *)right;
    
    if (a > b) { return 1; }
    if (a < b) { return -1; }
    
    return 0;
}

void print_func(const void *data) {
    size_t d = *(size_t *)data;
    
    printf("%lu ", d);
}

int main(int argc, const char **argv) {
    struct LinkedList *taskQueue = LinkedList_Create();
    
    for (int i = 1; i < argc; ++i) {
        // verifica daca parametrul nu este switch
        if (argv[i][0] != '-') {
            fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[i]);
            
            return EXIT_FAILURE;
        }
        
        size_t skippedChars = strspn(argv[i], "-");
        if (skippedChars > 2) { skippedChars = 2; }
        
        enum Task task = UNKNOWN;
        
        const char *sw = argv[i] + skippedChars; // switch-ul fara liniile din fata
        
        if (strcmp(sw, SWITCHES[OUTLIER_REMOVAL]) == 0) {
            task = OUTLIER_REMOVAL; // eliminare de exceptii folosind statistici
        }
        
        else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_MEDIAN]) == 0) {
            task = NOISE_REMOVAL_MEDIAN; // eliminare de zgomot prin filtru median
        }
        
        else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_AVG]) == 0) {
            task = NOISE_REMOVAL_AVG; // eliminare de zgomot prin filtru bazat pe media aritmetica
        }
        
        else if (strcmp(sw, SWITCHES[TIME_LEVELLING]) == 0) {
            task = TIME_LEVELLING; // uniformizarea frecventei
        }
        
        else if (strcmp(sw, SWITCHES[DATA_FILLING]) == 0) {
            task = DATA_FILLING; // completarea datelor lipsa
        }
        
        else if (strncmp(sw, SWITCHES[STATISTICS], strlen(SWITCHES[STATISTICS])) == 0) {
            task = STATISTICS; // calcularea statisticilor
            
            // if () // st1000
            // else // st 1000
        }
        
        else {
            fprintf(stderr, "%s: unrecognized task '%s'\n", argv[0], sw);
            
            return EXIT_FAILURE;
        }
        
        LinkedList_AddDataEnd(taskQueue, &task, sizeof task);
        
        // if (strncmp(sw, SWITCHES[STATISTICS], strlen(SWITCHES[STATISTICS])) == 0) {
            // const char *interval = sw + strlen(SWITCHES[STATISTICS]);
            
            // // LinkedList_AddDataEnd(taskQueue, &task, sizeof task);
        // }
    }
    
    while (!LinkedList_IsEmpty(taskQueue)) {
        enum Task *p_task = (enum Task *)LinkedList_RemoveBegin(taskQueue);
        
        enum Task task = *p_task;
        
        printf("%d\n", task);
        
        LinkedListNode_DestroyData(p_task);
    }
    
    LinkedList_Destroy(taskQueue);
    
    
    
    struct LinkedList *testList = LinkedList_Create();
    
    for (size_t i = 3 + 1; i-- > 0; ) {
        LinkedList_AddDataBegin(testList, &i, sizeof i);
    }
    
    for (size_t i = 4; i <= 8; ++i) {
        LinkedList_AddDataEnd(testList, &i, sizeof i);
    }
    
    LinkedList_Print(testList, print_func);
    printf("\n");
    
    
    
    return EXIT_SUCCESS;
}