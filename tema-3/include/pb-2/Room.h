#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>
#include <stddef.h>

struct Room {
	size_t id;
	
	bool isExit;
	
	unsigned *guardTimes;
	size_t guardTimeCount;
};

struct Room * Room_Create(size_t id,
                          bool isExit,
                          unsigned *guardTimes,
                          size_t guardTimeCount);

bool Room_IsExit(const struct Room *room);

bool Room_ContainsGuardAtTime(const struct Room *room, unsigned time);

void Room_Destroy(struct Room *room);

void Room_print_func_id(const struct Room *room, FILE *stream);

struct Room * Room_Data_Unwrap(const struct Data *data);

void Room_Data_proc_func_Destroy(struct Data *data);

void Room_Data_print_func_id(const struct Data *data, FILE *stream);

#endif // ROOM_H
