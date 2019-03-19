#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "Point.h"

enum Task {
    OUTLIER_REMOVAL,      // eliminare de exceptii folosind statistici
    NOISE_REMOVAL_MEDIAN, // eliminare de zgomot prin filtru median
    NOISE_REMOVAL_AVG,    // eliminare de zgomot prin filtru bazat pe media aritmetica
    TIME_LEVELLING,       // uniformizarea frecventei
    DATA_FILLING,         // completarea datelor lipsa
    STATISTICS,           // calcularea statisticilor
    
    UNKNOWN = -1,
};

const char *SWITCHES[] = {
    "e1", // OUTLIER_REMOVAL
    "e2", // NOISE_REMOVAL_MEDIAN
    "e3", // NOISE_REMOVAL_AVG
    "u",  // TIME_LEVELLING
    "c",  // DATA_FILLING
    "st", // STATISTICS
};

void test_func(); // TEMP

void outlier_removal();
void noise_removal_median();
void noise_removal_avg();
void time_levelling();
void data_filling();
void statistics();

int main(int argc, const char **argv) {
    struct LinkedList *taskList = LinkedList_Create();
    
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
        
        if      (strcmp(sw, SWITCHES[OUTLIER_REMOVAL]) == 0)                           { task = OUTLIER_REMOVAL;      }
        else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_MEDIAN]) == 0)                      { task = NOISE_REMOVAL_MEDIAN; }
        else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_AVG]) == 0)                         { task = NOISE_REMOVAL_AVG;    }
        else if (strcmp(sw, SWITCHES[TIME_LEVELLING]) == 0)                            { task = TIME_LEVELLING;       }
        else if (strcmp(sw, SWITCHES[DATA_FILLING]) == 0)                              { task = DATA_FILLING;         }
        else if (strncmp(sw, SWITCHES[STATISTICS], strlen(SWITCHES[STATISTICS])) == 0) { task = STATISTICS;           }
        else {
            fprintf(stderr, "%s: unrecognized task '%s'\n", argv[0], sw);
            
            return EXIT_FAILURE;
        }
        
        LinkedList_AddDataLast(taskList, &(struct LinkedListData){ &task, sizeof task });
        
        switch (task) {
            case STATISTICS: {
                skippedChars += strlen(SWITCHES[STATISTICS]);
                
                unsigned delta = 0;
                if (sscanf(argv[i] + skippedChars, "%u", &delta) <= 0) { // daca nu este de forma '{sw}{delta}'
                    // incearca de forma '{sw} {delta}'
                    if ((i + 1 < argc) && (sscanf(argv[i + 1], "%u", &delta) > 0)) { ++i; }
                }
                
                LinkedList_AddDataLast(taskList, &(struct LinkedListData){ &delta, sizeof delta });
                
                break;
            }
            
            default: {}
        }
    }
    
    struct LinkedList *pointList = LinkedList_Create();
    
    size_t pointCount = 0;
    scanf("%lu", &pointCount);
    
    for (size_t i = 0; i < pointCount; ++i) {
        struct Point point;
        point.timestamp = 0;
        point.value = 0.f;
        
        scanf("%u %f", &point.timestamp, &point.value);
        
        LinkedList_AddDataLast(pointList, &(struct LinkedListData){ &point, sizeof point });
    }
    
    while (!LinkedList_IsEmpty(taskList)) {
        struct LinkedListData *taskContainer = LinkedList_RemoveFirst(taskList, true);
        enum Task task = **(enum Task **)taskContainer;
        
        switch (task) {
            case OUTLIER_REMOVAL: {
                outlier_removal(pointList);
                
                break;
            }
            
            case NOISE_REMOVAL_MEDIAN: {
                
                
                break;
            }
            
            case NOISE_REMOVAL_AVG: {
                
                
                break;
            }
            
            case TIME_LEVELLING: {
                
                
                break;
            }
            
            case DATA_FILLING: {
                
                
                break;
            }
            
            case STATISTICS: {
                struct LinkedListData *deltaContainer = LinkedList_RemoveFirst(taskList, true);
                unsigned delta = **(unsigned **)deltaContainer;
                
                delta = delta;
                
                LinkedListData_Destroy(deltaContainer);
                
                break;
            }
            
            default: {}
        }
        
        LinkedListData_Destroy(taskContainer);
    }
    
    printf("%lu", LinkedList_GetSize(pointList));
    printf("\n");
    
    LinkedList_Print(pointList, Point_LinkedList_print_func);
    
    LinkedList_Destroy(pointList);
    
    LinkedList_Destroy(taskList);
    
    test_func(); // TEMP
    
    return EXIT_SUCCESS;
}

int test_comp_func(const struct LinkedListData *left, const struct LinkedListData *right) {
    size_t a = **(size_t **)left;
    size_t b = **(size_t **)right;
    
    if (a > b) { return 1; }
    if (a < b) { return -1; }
    
    return 0;
}

void test_print_func(const struct LinkedListData *data) {
    size_t d = **(size_t **)data;
    
    printf("%lu ", d);
}

void test_func() {
    // struct LinkedList *testList = LinkedList_Create();
    
    // for (size_t i = 3 + 1; i-- > 0; ) {
        // LinkedList_AddDataFirst(testList, &(struct LinkedListData){ &i, sizeof i });
    // }
    
    // for (size_t i = 4; i <= 8; ++i) {
        // LinkedList_AddDataLast(testList, &(struct LinkedListData){ &i, sizeof i });
    // }
    
    // LinkedList_RemoveData(testList, &(struct LinkedListData){ &(size_t){ 4 }, sizeof(size_t) }, test_comp_func, false);
    
    // LinkedList_Print(testList, test_print_func);
    // printf("\n");
    
    // LinkedList_Destroy(testList);
}
