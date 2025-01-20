#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include <stdio.h>
#include "primitives.h"

int serial_init(const char *);

int serial_content_list(FILE *);

int serial_record_find(FILE *, const char *, RECORD *);

int serial_record_add(FILE *, RECORD);

int serial_record_edit(FILE*, const char*);

int serial_logical_delete(FILE *, const char *);

int serial_physical_delete(FILE *, const char *);

#endif