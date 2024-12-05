#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primitives.h"
#include "serial.h"

int main(){

    //serial_init("datoteka.bin");

    // THIS IS GIT TEST!!!

    FILE *fp = fopen("datoteka.bin", "rb+");


    RECORD r = {F_USE, "eeg", "fef", "fwe", "hwh", "weff"};

    serial_record_add(fp, r);

    return 0;
}