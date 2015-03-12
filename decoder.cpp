#include <iostream>
#include <cstdio>
#include "decoder.h"
#include "errorHandler.h"
#include "simulator.h"

void decodeData(FILE* F){


    int i=0;
    int status;
    char data_temp;

    status = fread(&data_temp,sizeof(char),1,F);
    sim.reg[29] = data_temp;

    status =fread(&data_temp,sizeof(char),1,F);
    sim.D_number = data_temp;

    while(!feof(F)){

        status = fread(&data_temp,sizeof(char),1,F);
        if(!status){
            sim.D_mem[i++] = data_temp;
        }

    }


}

void decodeInstructions(FILE* F){

    int i=0;
    int status;
    unsigned int ins_temp;

    status = fread(&ins_temp,sizeof(unsigned int),1,F);
    sim.PC = ins_temp;

    status =fread(&ins_temp,sizeof(unsigned int),1,F);
    sim.I_number = ins_temp;

    while(!feof(F)){

        status = fread(&ins_temp,sizeof(unsigned int),1,F);
        if(!status){
            sim.I_mem[i++] = ins_temp;
        }

    }
}


