#include "Data.h"
#include "List.h"
#include "Point.h"

#include <error.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Task {
	OUTLIER_REMOVAL, // eliminare de exceptii folosind statistici
	
	NOISE_REMOVAL_MEDIAN, // eliminare de zgomot prin filtru median
	
	NOISE_REMOVAL_ARITHMETIC_MEAN, // eliminare de zgomot prin filtru bazat
	                               // pe media aritmetica
	
	TIME_LEVELLING, // uniformizarea frecventei
	
	DATA_FILLING, // completarea datelor lipsa
	
	STATISTICS, // calculul statisticilor
	
	UNKNOWN = -1,
};

static const char *SWITCHES[] = {
	"e1", // OUTLIER_REMOVAL
	"e2", // NOISE_REMOVAL_MEDIAN
	"e3", // NOISE_REMOVAL_ARITHMETIC_MEAN
	"u",  // TIME_LEVELLING
	"c",  // DATA_FILLING
	"st", // STATISTICS
};

struct List * outlier_removal(struct List *pointList,
                              Data_print_func_t *out_print_func);

struct List * noise_removal_median(struct List *pointList,
                                   Data_print_func_t *out_print_func);

struct List * noise_removal_arithmetic_mean(struct List *pointList,
                                            Data_print_func_t *out_print_func);

struct List * time_levelling(struct List *pointList,
                             Data_print_func_t *out_print_func);

struct List * data_filling(struct List *pointList,
                           Data_print_func_t *out_print_func);

struct List * statistics(struct List *pointList,
                         unsigned delta,
                         Data_print_func_t *out_print_func);

struct List * process_switches(int argc, const char *argv[]) {
	struct List *taskList = List_Create();
	
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] != '-') { // verifica daca parametrul nu este switch
			error(EXIT_FAILURE, errno, "Unrecognized option '%s'", argv[i]);
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
		
		else if (strcmp(sw, SWITCHES[NOISE_REMOVAL_ARITHMETIC_MEAN]) == 0)
		{ task = NOISE_REMOVAL_ARITHMETIC_MEAN; }
		
		else if (strcmp(sw, SWITCHES[TIME_LEVELLING]) == 0)
		{ task = TIME_LEVELLING; }
		
		else if (strcmp(sw, SWITCHES[DATA_FILLING]) == 0)
		{ task = DATA_FILLING; }
		
		else if (strncmp(sw,
		                 SWITCHES[STATISTICS],
		                 strlen(SWITCHES[STATISTICS])) == 0)
		{ task = STATISTICS; }
		
		else {
			error(EXIT_FAILURE, errno, "Unrecognized task '%s'", sw);
		}
		
		const struct Data taskWrapper =
			(struct Data){ &task, sizeof task };
		
		List_AddDataLast(taskList, &taskWrapper);
		
		switch (task) {
			case OUTLIER_REMOVAL:
				break;
			
			case NOISE_REMOVAL_MEDIAN:
				break;
			
			case NOISE_REMOVAL_ARITHMETIC_MEAN:
				break;
			
			case TIME_LEVELLING:
				break;
			
			case DATA_FILLING:
				break;
			
			case STATISTICS:
				skippedChars += strlen(SWITCHES[STATISTICS]);
				
				unsigned delta;
				
				// daca nu este de forma '{sw}{delta}'
				if (sscanf(argv[i] + skippedChars, "%u", &delta) <= 0) {
					// incearca de forma '{sw} {delta}'
					if ((i + 1 < argc) &&
					    (sscanf(argv[i + 1], "%u", &delta) > 0)) { ++i; }
				}
				
				const struct Data deltaWrapper =
					(struct Data){ &delta, sizeof delta };
				
				List_AddDataLast(taskList, &deltaWrapper);
				
				break;
			
			default:
				error(EXIT_FAILURE, errno, "Unknown task '%d'", task);
		}
	}
	
	return taskList;
}

struct List * read_points() {
	struct List *pointList = List_Create();
	
	size_t pointCount;
	
	if (fscanf(stdin, "%zu", &pointCount) != 1) {
		error(EXIT_FAILURE, errno, "Could not read point count");
	}
	
	for (size_t i = 0; i < pointCount; ++i) {
		struct Point point;
		
		if (fscanf(stdin, "%u %lf", &point.timestamp, &point.value) != 2) {
			error(EXIT_FAILURE, errno, "Could not read point");
		}
		
		const struct Data pointWrapper =
			(struct Data){ &point, sizeof point };
		
		List_AddDataLast(pointList, &pointWrapper);
	}
	
	return pointList;
}

int main(int argc, const char *argv[]) {
	struct List *taskList = process_switches(argc, argv);
	
	struct List *pointList = read_points();
	
	struct List *prevOutputList = List_Copy(pointList);
	struct List *outputList = prevOutputList;
	Data_print_func_t outputList_print_func = Point_Data_print_func;
	
	while (!List_IsEmpty(taskList)) {
		prevOutputList = outputList;
		
		struct Data *taskWrapper =
			List_RemoveFirstNode(taskList, true);
		
		enum Task task = **(enum Task **)taskWrapper;
		
		switch (task) {
			case OUTLIER_REMOVAL:
				outputList =
					outlier_removal(outputList, &outputList_print_func);
				
				break;
			
			case NOISE_REMOVAL_MEDIAN:
				outputList =
					noise_removal_median(outputList, &outputList_print_func);
				
				break;
			
			case NOISE_REMOVAL_ARITHMETIC_MEAN:
				outputList =
					noise_removal_arithmetic_mean(outputList,
					                              &outputList_print_func);
				
				break;
			
			case TIME_LEVELLING:
				outputList =
					time_levelling(outputList, &outputList_print_func);
				
				break;
			
			case DATA_FILLING:
				outputList = data_filling(outputList, &outputList_print_func);
				
				break;
			
			case STATISTICS:
				struct Data *deltaWrapper =
					List_RemoveFirstNode(taskList, true);
				
				unsigned delta = **(unsigned **)deltaWrapper;
				
				outputList = statistics(outputList,
				                        delta,
				                        &outputList_print_func);
				
				Data_Destroy(deltaWrapper);
				
				break;
			
			default:
				error(EXIT_FAILURE, errno, "Unknown task '%d'", task);
		}
		
		if (prevOutputList != outputList) {
			List_Destroy(prevOutputList);
		}
		
		Data_Destroy(taskWrapper);
	}
	
	if (outputList_print_func == Point_Data_print_func) {
		fprintf(stdout, "%zu\n", List_GetSize(outputList));
	}
	
	List_Print(outputList, outputList_print_func, "\n", "\n", stdout);
	
	List_Destroy(outputList);
	
	List_Destroy(pointList);
	
	List_Destroy(taskList);
	
	return EXIT_SUCCESS;
}
