#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include <stdio.h>

int serial_init(const char *);

int serial_content_list(FILE *);

int serial_record_find(FILE *, const char *, RECORD *);

int serial_record_add(FILE *, RECORD);

#endif