#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_F 3

#define STAT_OK 0
#define STAT_EOF 1
#define STAT_ERR 2

#define F_USE 0x01
#define F_EOF 0x02
#define F_DEL 0x04

#define BLOCK_SIZE sizeof(BLOCK)
#define RECORD_SIZE sizeof(RECORD)

#define err(args) \
    fprintf(stderr, "GRESKA: %s\n", args);

typedef struct record_st{
    char flags;
    char event_id[13];
    char date_time[20];
    char event_type[8];
    char user_id[11];
    char event_name[21];
} RECORD;

typedef struct block_st{
    RECORD records[BLOCK_F];
} BLOCK;


int file_init(const char *, FILE **);

BLOCK block_init();

int block_write(FILE *, BLOCK, long);

int block_read(FILE *, BLOCK *, long);

void record_print(RECORD);

#endif