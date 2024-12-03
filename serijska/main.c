#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primitives.h"
#include "serial.h"

void test(){
    FILE *fp = fopen("datoteka.bin", "wb+");
    RECORD r = {0x00,{},{},{},{},{}};
    char letters[10][3] = {
        "aa\0",
        "bb\0",
        "cc\0",
        "dd\0",
        "ee\0",
        "ff\0",
        "gg\0",
        "hh\0",
        "ii\0",
        "jj\0"
    };
    r.flags = F_USE;
    for(int i = 0; i < 10; i++){
        strcpy(r.event_id, letters[i]);
        strcpy(r.date_time, letters[i]);
        strcpy(r.event_type, letters[i]);
        strcpy(r.user_id, letters[i]);
        strcpy(r.event_name, letters[i]);
        fwrite(&r, RECORD_SIZE, 1, fp);
    }
    r.flags = F_EOF;
    fwrite(&r, RECORD_SIZE, 1, fp);
    fwrite(&r, RECORD_SIZE, 1, fp);
    fclose(fp);
}

int main(){
    test();

    FILE *fp = fopen("datoteka.bin", "rb");

    serial_content_list(fp);

    return 0;
}