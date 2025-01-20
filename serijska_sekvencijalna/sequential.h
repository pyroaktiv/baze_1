#ifndef SEQUENTIAL_H_INCLUDED
#define SEQUENTIAL_H_INCLUDED

#include <stdio.h>
#include "primitives.h"

int sequential_record_find(FILE *, const char *, RECORD *);

int sequential_record_add(FILE *, RECORD);

int sequential_record_edit(FILE *, const char *);

int sequential_logical_delete(FILE *, const char *);



#endif