#include <iostream>
#include <cstdio>
#include "simulator.h"
#include "decoder.h"
#include "errorHandler.h"
using namespace std;

simulator sim;
extern I_number;
extern D_number;
int main()
{
    int type = 1;
    int temp;

    if(type==1){
        FILE* W = fopen("iimage","wb");
        int i[8] = {0x12345678,0x23456789,0x34567891,0x45678912,0x56789123,0x67891234,0x78912345,0x89123456};
        fwrite(&i,sizeof(int),1,W);
    }
    else{
        FILE* W = fopen("test","rb");
        int status = fread(&temp,sizeof(int),1,W);
        printf("%x \n",temp);
    }
    return 0;




    FILE* iimage =  fopen("iimage","rb");
    FILE* dimage =  fopen("dimage","rb");

    decodeInstructions(iimage);
    decodeData(dimage);
    cout<<sim.PC<<endl;
    cout<<sim.reg[29]<<endl;
    for(int i=0;i<32;i++){
        cout<<"reg"<<i<<" "<<sim.reg[i]<<endl;
    }
    for(i=0;i<I_number;i++){
        cout<<"Instruction"<<i<<" "<<sim.I_mem[i]<<endl;
    }





}

void init(){



}
