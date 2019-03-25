#include "Data.h"
#include "List.h"
#include "Point.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Task {
    OUTLIER_REMOVAL,      // eliminare de exceptii folosind statistici
    
    NOISE_REMOVAL_MEDIAN, // eliminare de zgomot prin filtru median
    
    NOISE_REMOVAL_A_MEAN, // eliminare de zgomot prin filtru bazat
                          // pe media aritmetica
                          
    TIME_LEVELLING,       // uniformizarea frecventei
    
    DATA_FILLING,         // completarea datelor lipsa
    
    STATISTICS,           // calculul statisticilor
    
    UNKNOWN = -1,
};

static const char *SWITCHES[] = {
    "e1", // OUTLIER_REMOVAL
    "e2", // NOISE_REMOVAL_MEDIAN
    "e3", // NOISE_REMOVAL_A_MEAN
    "u",  // TIME_LEVELLING
    "c",  // DATA_FILLING
    "st", // STATISTICS
};

struct List * outlier_removal(struct List *pointList,
                              List_print_func_t *out_print_func);

struct List * noise_removal_median(struct List *pointList,
                                   List_print_func_t *out_print_func);

struct List * noise_removal_a_mean(struct List *pointList,
                                   List_print_func_t *out_print_func);

struct List * time_levelling(struct List *pointList,
                             List_print_func_t *out_print_func);

struct List * data_filling(struct List *pointList,
                           List_print_func_t *out_print_func);

struct List * statistics(struct List *pointList,
                         unsigned delta,
                         List_print_func_t *out_print_func);

int main(int argc, const char *argv[]) {
    struct List *taskList = List_Create();
    
    for (int i = 1; i < argc; ++i) {
        // verifica daca parametrul nu este switch
        if (argv[i][0] != '-') {
            fprintf(stderr,
                    "%s: unrecognized option '%s'\n",
                    argv[0],
                    argv[i]);
            
            return EXIT_FAILURE;
        }
        
        size_t skippedChars = strspn(argv[i], "-");
        if (skippedChars > 2) { skippedChars = 2; }
        
        enum Task task = UNKNOWN;
        
        // switch-ul fara liniile din fata
        const char *sw = argv[i] + skippedChars;
        
        if (strcmp(sw, SWITCHES[OUTLIER_REMOVAL]) == 0)
        { task = OUTLIER_REMOVAL; }
        
        else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_MEDIAN]) == 0)
        { task = NOISE_REMOVAL_MEDIAN; }
        
        else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_A_MEAN]) == 0)
        { task = NOISE_REMOVAL_A_MEAN; }
        
        else if (strcmp(sw, SWITCHES[TIME_LEVELLING]) == 0)
        { task = TIME_LEVELLING; }
        
        else if (strcmp(sw, SWITCHES[DATA_FILLING]) == 0)
        { task = DATA_FILLING; }
        
        else if (strncmp(sw,
                         SWITCHES[STATISTICS],
                         strlen(SWITCHES[STATISTICS])) == 0)
        { task = STATISTICS; }
        
        else {
            fprintf(stderr,
                    "%s: unrecognized task '%s'\n",
                    argv[0],
                    sw);
            
            return EXIT_FAILURE;
        }
        
        List_AddDataLast(taskList, &(struct Data){ &task, sizeof task });
        
        switch (task) {
            case STATISTICS: {
                skippedChars += strlen(SWITCHES[STATISTICS]);
                
                unsigned delta = 0;
                
                // daca nu este de forma '{sw}{delta}'
                if (sscanf(argv[i] + skippedChars, "%u", &delta) <= 0) {
                    // incearca de forma '{sw} {delta}'
                    if ((i + 1 < argc) &&
                        (sscanf(argv[i + 1], "%u", &delta) > 0)) { ++i; }
                }
                
                List_AddDataLast(taskList,
                                 &(struct Data){ &delta, sizeof delta });
                
                break;
            }
            
            default: {}
        }
    }
    
    // citeste lista de puncte
    
    struct List *pointList = List_Create();
    
    size_t pointCount = 0;
    if (!scanf("%zu", &pointCount)) { return EXIT_FAILURE; }
    
    for (size_t i = 0; i < pointCount; ++i) {
        struct Point point;
        point.timestamp = 0;
        point.value = 0.0;
        
        if (!scanf("%u %lf", &point.timestamp, &point.value)) {
            return EXIT_FAILURE;
        }
        
        List_AddDataLast(pointList,
                         &(struct Data){ &point, sizeof point });
    }
    
    struct List *prevOutputList = List_Copy(pointList);
    struct List *outputList = prevOutputList;
    List_print_func_t outputList_print_func = Point_List_print_func;
    
    while (!List_IsEmpty(taskList)) {
        prevOutputList = outputList;
        
        struct Data *taskContainer = List_RemoveFirstNode(taskList,
                                                          true);
        enum Task task = **(enum Task **)taskContainer;
        
        switch (task) {
            case OUTLIER_REMOVAL: {
                outputList = outlier_removal(outputList,
                                             &outputList_print_func);
                
                break;
            }
            
            case NOISE_REMOVAL_MEDIAN: {
                outputList = noise_removal_median(outputList,
                                                  &outputList_print_func);
                
                break;
            }
            
            case NOISE_REMOVAL_A_MEAN: {
                outputList = noise_removal_a_mean(outputList,
                                                  &outputList_print_func);
                
                break;
            }
            
            case TIME_LEVELLING: {
                outputList = time_levelling(outputList,
                                            &outputList_print_func);
                
                break;
            }
            
            case DATA_FILLING: {
                outputList = data_filling(outputList,
                                          &outputList_print_func);
                
                break;
            }
            
            case STATISTICS: {
                struct Data *deltaContainer = List_RemoveFirstNode(taskList,
                                                                   true);
                unsigned delta = **(unsigned **)deltaContainer;
                
                outputList = statistics(outputList,
                                        delta,
                                        &outputList_print_func);
                
                Data_Destroy(deltaContainer);
                
                break;
            }
            
            default: {}
        }
        
        if (prevOutputList != outputList) {
            List_Destroy(prevOutputList);
        }
        
        Data_Destroy(taskContainer);
    }
    
    if (outputList_print_func == Point_List_print_func) {
        printf("%zu\n", List_GetSize(outputList));
    }
    
    List_Print(outputList, outputList_print_func);
    
    List_Destroy(outputList);
    
    List_Destroy(pointList);
    
    List_Destroy(taskList);
    
    return EXIT_SUCCESS;
}
