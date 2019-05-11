#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define NDEBUG
#include <assert.h>

#define LINE_BUFFER_LENGTH 128 // TODO: also use #define in Tema 1 Task 4

static_assert(LINE_BUFFER_LENGTH > 1,
              "LINE_BUFFER_LENGTH must be greater than 1");

char * fgetl(FILE *stream) {
	assert(stream);
	
	static char lineBuffer[LINE_BUFFER_LENGTH];
	
	char *line = calloc(1, sizeof *line);
	size_t lineLength = 0;
	
	do {
		if (!fgets(lineBuffer, LINE_BUFFER_LENGTH, stream)) {
			goto error;
		}
		
		size_t lineChunkLength = strlen(lineBuffer);
		
		if (lineBuffer[lineChunkLength - 1] == '\n') {
			--lineChunkLength;
		}
		
		line = realloc(line, lineLength + lineChunkLength + 1);
		
		strncpy(line + lineLength,
		        lineBuffer,
		        lineChunkLength);
		
		lineLength += lineChunkLength;
	} while ((strlen(lineBuffer) == LINE_BUFFER_LENGTH - 1) &&
	         (lineBuffer[LINE_BUFFER_LENGTH - 2] != '\n'));
	
	line[lineLength] = '\0';
	
	return line;
	
error:
	free(line);
	
	return NULL;
}
