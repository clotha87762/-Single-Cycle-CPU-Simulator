#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "simulator.h"
#include "decoder.h"


using namespace std;
#define RRS (r_instruction.rs)
#define RRT (r_instruction.rt)
#define RRD (r_instruction.rd)
#define IRS (i_instruction.rs)
#define IRT (i_instruction.rt)
#define IIM (i_instruction.immediate)
#define JA (j_instruction.address)


int branch;

int simulator::decodeInstruction(){
        int halt = 0;
        branch = 0;
        unsigned int instruction = (I_mem_byte[PC]<<24)|(I_mem_byte[PC+1]<<16)| (I_mem_byte[PC+2]<<8) | (I_mem_byte[PC+3]);//會不會是變數名字的關係????
        int mask;
        //printf("ins 0x%08X\n",instruction);
        mask = 0xFC000000;
        opcode = (instruction&mask)>>26;
        if(opcode==0x00){

            r_instruction.opcode = opcode;
            r_instruction.rs=(instruction<<6)>>27;
            r_instruction.rt=(instruction<<11)>>27;
            r_instruction.rd= (instruction<<16)>>27;
            r_instruction.shamt = (instruction<<21)>>27;
            r_instruction.funct=(instruction<<26)>>26;
           // cout<<"YOOfunct: "<<r_instruction.funct<<endl;
            nowType = Rtype;
            switch(r_instruction.funct){
                case 0x20:
                    nowOp = ADD;
                    break;
                case 0x22:
                     nowOp = SUB;
                    break;
                case 0x24:
                     nowOp = AND;
                    break;
                case 0x25:
                     nowOp = OR;
                    break;
                case 0x26:
                     nowOp = XOR;
                    break;
                case 0x27:
                     nowOp = NOR;
                    break;
                case 0x28:
                     nowOp = NAND;
                    break;
                case 0x2A:
                     nowOp = SLT;
                    break;
                case 0x00:
                     nowOp = SLL;
                    break;
                case 0x02:
                     nowOp = SRL;
                    break;
                case 0x03:
                     nowOp = SRA;
                    break;
                case 0x08:
                     nowOp = JR;
                    break;
                default:
                    break;

            }


        }
        else if(opcode==0x03||opcode==0x02){
            j_instruction.opcode = opcode;
            j_instruction.address = (instruction<<6)>>6;

            nowType = Jtype;
            if(opcode==0x03)
                nowOp = JAL;
            else if(opcode==0x02)
                nowOp = J;

        }
        else if(opcode==0x3F){
            //Halt
           i_instruction.immediate=0;
           i_instruction.opcode=0;
           i_instruction.rs=0;
           i_instruction.rt=0;
           r_instruction.funct=0;
           r_instruction.opcode=0;
           r_instruction.rd=0;
           r_instruction.rs=0;
           r_instruction.rt=0;
           r_instruction.shamt=0;
           j_instruction.address=0;
           j_instruction.opcode=0;
            halt = 1;
            return halt;

        }
        else if(opcode==0x08||opcode==0x23||opcode==0x21||opcode==0x25||opcode==0x20||opcode==0x24||opcode==0x2B||opcode==0x29||
                opcode==0x28||opcode==0x0F||opcode==0x0D||opcode==0x0E||opcode==0x0C||opcode==0x0A||opcode==0x04||opcode==0x05){

            i_instruction.opcode = opcode;
            i_instruction.rs = (instruction<<6)>>27;
            i_instruction.rt = (instruction<<11)>>27;

            i_instruction.immediate = (signed short)((instruction<<16)>>16);
            //cout<<"yooimmediate: "<<dec<<i_instruction.immediate<<endl;
           /* int signmask=1;
            if(((signmask<<15)&i_instruction.immediate)==1)*/


            nowType = Itype;

            switch(opcode){

                case 0x08:
                    nowOp = ADDI;
                    break;
                case 0x23:
                     nowOp = LW;
                    break;
                case 0x21:
                     nowOp = LH;
                    break;
                case 0x25:
                     nowOp = LHU;
                    break;
                case 0x20:
                     nowOp = LB;
                    break;
                case 0x24:
                     nowOp = LBU;
                    break;
                case 0x2B:
                    nowOp = SW;
                    break;
                case 0x29:
                    nowOp = SH;
                    break;
                case 0x28:
                    nowOp = SB;
                    break;
                case 0x0F:
                    nowOp = LUI;
                    break;
                case 0x0C:
                    nowOp = ADDI;
                    break;
                case 0x0D:
                    nowOp = ORI;
                    break;
                case 0x0E:
                    nowOp = NORI;
                    break;
                case 0x0A:
                    nowOp = SLTI;
                    break;
                case 0x04:
                    nowOp = BEQ;
                    break;
                case 0x05:
                    nowOp = BNE;
                    break;
            }

        }

        return halt;

}



void simulator::afterInstruction(){

        if(!branch)
        PC+=4;

        cycle++;


}


void simulator::PrintReg(FILE* F){

    fprintf(F,"cycle %d\n",cycle);
    for(int i=0;i<32;i++){

        if(i<10)fprintf(F,"$0%d: 0x%08X\n",i,reg[i]);
        else{fprintf(F,"$%d: 0x%08X\n",i,reg[i]); }
    }
    fprintf(F,"PC: 0x%08X\n\n\n",PC);


}


void  simulator::Deal_WritetoReg0(FILE* E){

        if(nowType==Itype){

            if(i_instruction.rt!=0x00)return;

            if(nowOp==SW||nowOp==SH||nowOp==SB||nowOp==BEQ||nowOp==BNE) return;

            fprintf( E,"In cycle %d: Write $0 Error\n", cycle);
            skipCycle = true;
            return;

        }
        else if(nowType==Rtype){

            if(r_instruction.rd!=0x00) return;

            if(nowOp==SLL&&r_instruction.rs==0x00&&r_instruction.rt==0x00) return;

            if(nowOp==JR) return;

            fprintf(E,"In cycle %d: Write $0 Error\n", cycle);

            skipCycle = true;
            return;

        }

}


void  simulator::Deal_NumberOverflow(FILE* E){

    bool isOverflow ;
    isOverflow=false;

    if(!(nowOp==ADD||nowOp==ADDI||nowOp==SUB||nowOp==LW||nowOp==LH||nowOp==LHU||nowOp==LB||nowOp==LBU
       ||nowOp==SW||nowOp==SH||nowOp==SB||nowOp==BEQ||nowOp==BNE))
       return;

    if(nowType==Rtype){

        if(nowOp==ADD){

            if(reg[r_instruction.rs]>=0&&reg[r_instruction.rt]>=0&&(reg[r_instruction.rs]+reg[r_instruction.rt])<0)
                 isOverflow=true;//記得最後要加report喔
            else if(reg[r_instruction.rs]<0&&reg[r_instruction.rt]<0&&(reg[r_instruction.rs]+reg[r_instruction.rt])>=0)
                isOverflow=true;

    }
    else if(nowOp==SUB){


        if(reg[r_instruction.rs]>=0&&reg[r_instruction.rt]<0&&(reg[r_instruction.rs]-reg[r_instruction.rt])<0)
            isOverflow=true; //記得最後要加report喔
        else if(reg[r_instruction.rs]<0&&reg[r_instruction.rt]>=0&&(reg[r_instruction.rs]-reg[r_instruction.rt])>=0)
            isOverflow = true;

        }

    }
    else if(nowType==Itype){

            if(nowOp==BEQ||nowOp==BNE){

                if(PC+4>=0&&i_instruction.immediate>=0&&((int)((PC+4)+4*i_instruction.immediate))<0)
                    isOverflow=true;

            }
            else{

                if(reg[i_instruction.rs]>=0&&i_instruction.immediate>=0&&reg[i_instruction.rs]+i_instruction.immediate<0)
                    isOverflow=true;
                else if(reg[i_instruction.rs]<0&&i_instruction.immediate<0&&reg[i_instruction.rs]+i_instruction.immediate>=0)
                    isOverflow = true;
            }

    }

    if(isOverflow)
        fprintf(E, "In cycle %d: Number Overflow\n", cycle);

}

void  simulator::Deal_MemoryAddOverflow(FILE* E){

    if(nowOp==LW||nowOp==LH||nowOp==LHU||nowOp==LB||nowOp==LBU||nowOp==SW||nowOp==SH||nowOp==SB){

        if(reg[i_instruction.rs]+i_instruction.immediate>=1024||reg[i_instruction.rs]+i_instruction.immediate<0){
            fprintf(E,"In cycle %d: Address Overflow\n", cycle);
            errorHalt = true;
        }

    }
}


void  simulator::Deal_DataMisaligned(FILE* E){

    if(nowOp==LW){

        if((reg[i_instruction.rs]+i_instruction.immediate)%4!=0){
           fprintf(E,"In cycle %d: Misalignment Error\n", cycle);
           errorHalt = true;
        }
    }
    else if(nowOp==LH||nowOp==LHU){

            if((reg[i_instruction.rs]+i_instruction.immediate)%2!=0){
            fprintf(E,"In cycle %d: Misalignment Error\n", cycle);
            errorHalt = true;
            }
    }
    else if(nowOp==SW){
        if((reg[i_instruction.rs]+i_instruction.immediate)%4!=0){
               fprintf(E,"In cycle %d: Misalignment Error\n", cycle);
               errorHalt = true;
            }
    }
    else if(nowOp==SH){
            if((reg[i_instruction.rs]+i_instruction.immediate)%2!=0){
                fprintf(E,"In cycle %d: Misalignment Error\n", cycle);
                errorHalt = true;
             }
    }


}


void simulator::ExecuteInstruction(FILE* E){

        Deal_WritetoReg0(E);
        Deal_NumberOverflow(E);
        Deal_MemoryAddOverflow(E);
        Deal_DataMisaligned(E);


        if(skipCycle){
            skipCycle=false;
            return;
        }

        if(errorHalt){
            return;
        }


       if(nowOp == ADD)
             add_Instruction();
       else if(nowOp == ADDI)
             addi_Instruction();
       else if(nowOp == AND)
            and_Instruction();
       else if(nowOp == ANDI)
            andi_Instruction();
       else if(nowOp == BEQ)
            beq_Instruction();
       else if(nowOp == BNE)
            bne_Instruction();
       else if(nowOp == J)
            j_Instruction();
       else if(nowOp == JAL)
            jal_Instruction();
       else if(nowOp == JR)
            jr_Instruction();
       else if(nowOp == LB)
            lb_Instruction();
       else if(nowOp == LBU)
            lbu_Instruction();
       else if(nowOp == LH)
            lh_Instruction();
       else if(nowOp == LHU)
            lhu_Instruction();
       else if(nowOp ==LUI)
            lui_Instruction();
       else if(nowOp == LW)
            lw_Instruction();
       else if(nowOp == NAND)
            nand_Instruction();
       else if(nowOp == NOR)
            nor_Instruction();
       else if(nowOp == NORI)
            nori_Instruction();
       else if(nowOp == OR)
            or_Instruction();
       else if(nowOp == ORI)
            ori_Instruction();
       else if(nowOp == SB)
            sb_Instruction();
       else if(nowOp == SH)
            sh_Instruction();
       else if(nowOp == SLL)
            sll_Instruction();
       else if(nowOp == SLT)
            slt_Instruction();
       else if(nowOp ==SLTI)
            slti_Instruction();
       else if(nowOp == SRA)
            sra_Instruction();
       else if(nowOp == SRL)
            srl_Instruction();
       else if(nowOp == SUB)
            sub_Instruction();
       else if(nowOp == SW)
            sw_Instruction();
       else if(nowOp == XOR)
            xor_Instruction();


}

void simulator::add_Instruction(){

    reg[RRD]=reg[RRS]+reg[RRT];

}

void  simulator::addi_Instruction(){

    reg[IRT] = reg[IRS] + IIM;
}

void  simulator::and_Instruction(){
    reg[RRD] = reg[RRS] & reg[RRT];
}

void simulator::andi_Instruction(){
    unsigned short temp = (unsigned short) IIM;
    unsigned int mask_temp = (unsigned int)temp;
    reg[IRT] = reg[IRS] & mask_temp;
}

void simulator::beq_Instruction(){
    if(reg[IRS]==reg[IRT]){
        PC += (4+IIM*4);
        branch = 1;
    }
}

void simulator::bne_Instruction(){
    if(reg[IRS]!=reg[IRT]){
        PC += (4+IIM*4);
        branch = 1;
    }
}

void simulator::j_Instruction(){

    PC = (((PC+4)>>28)<<28) | ((JA)<<2);
    branch = 1;
}


void simulator::jal_Instruction(){
    reg[31]=PC+4;
    PC = (((PC+4)>>28)<<28) | ((JA)<<2);  // 這邊啦啦啦啦啦啦啦啦啦啦啦啦啊啦
                                            //ㄆㄆㄆㄆㄆㄆㄆㄆ
    branch = 1;
}


void simulator::jr_Instruction(){
    PC =(unsigned int) reg[31];
    branch = 1;
}

void simulator::lb_Instruction(){
    char temp;
    temp =  ((char)D_mem_byte[reg[IRS] + IIM]);
    reg[IRT] = (int)temp;
}

void simulator::lbu_Instruction(){ // 恩亨？
    int temp = 0;
    unsigned char unchar = (unsigned char) D_mem_byte[reg[IRS] + IIM];
    temp = (int)unchar;
    reg[IRT] = temp;
}


void simulator::lh_Instruction(){

    char unchar = (char) D_mem_byte[reg[IRS] + IIM];
    char unchar1 = (char) D_mem_byte[reg[IRS] + IIM+1];
    int temp = ((int)unchar<<8)|((int)unchar1);
    reg[IRT] = temp;
   // printf("TEMP 0x%08X\n",temp);
    //reg[IRT] = temp
}

void simulator::lhu_Instruction(){
    unsigned char unchar = (unsigned char) D_mem_byte[reg[IRS] + IIM];
    unsigned char unchar1 = (unsigned char) D_mem_byte[reg[IRS] + IIM+1];
    int temp = ((int)unchar<<8)|((int)unchar1);
   // printf("TEMP 0x%08X\n",temp);
    reg[IRT] = temp;
    /*(((int)D_mem_byte[reg[IRS] + IIM])<<8)|((int) (D_mem_byte[reg[IRS] + IIM +1]))<0?
                     temp
                    :(((int)D_mem_byte[reg[IRS] + IIM])<<8)|((int) (D_mem_byte[reg[IRS] + IIM +1]))*/;
}


void simulator::lui_Instruction(){
    unsigned short temp ;
    temp = (unsigned short)IIM;
    unsigned int temp2 =((unsigned int) temp)<<16;
    reg[IRT] = temp2;
}

void simulator::lw_Instruction(){
    int temp;
    temp = reg[IRS]+IIM ;
    reg[IRT] = (((unsigned int)D_mem_byte[temp])<<24 )| (((unsigned int)D_mem_byte[temp+1])<<16) | (((unsigned int)D_mem_byte[temp+2])<<8) | (((unsigned int)D_mem_byte[temp+3]));
}

void simulator::nand_Instruction(){
    reg[RRD] = ~(reg[RRS]&reg[RRT]);
}

void simulator::nor_Instruction(){
    reg[RRD] = ~(reg[RRS]|reg[RRT]);
}

void simulator::nori_Instruction(){

    unsigned short temp = (unsigned short) IIM;
    unsigned int mask_temp = (unsigned int)temp;
    reg[IRT] = ~(reg[IRS] | mask_temp);
}

void simulator::or_Instruction(){
    reg[RRD] = reg[RRS] | reg[RRT];
}

void simulator::ori_Instruction(){
    //unsigned short unshort = (unsigned short);
    unsigned short temp = (unsigned short) IIM;
    unsigned int mask_temp = (unsigned int)temp;
    reg[IRT] = (reg[IRS] | mask_temp);
}

void simulator::sb_Instruction(){
    D_mem_byte[reg[IRS]+IIM] =(unsigned char) (reg[IRT] & 0x000000FF);
}

void simulator::sh_Instruction(){
    D_mem_byte[reg[IRS]+IIM] =  (unsigned char)((reg[IRT]&0x0000FF00)>>8);
    D_mem_byte[reg[IRS]+IIM+1] = (unsigned char)reg[IRT]&0x000000FF;

}

void simulator::sll_Instruction(){
    reg[RRD] =  reg[RRT] << r_instruction.shamt;

}


void simulator::slt_Instruction(){
    reg[RRD] = reg[RRS]<reg[RRT];
}

void simulator::slti_Instruction(){
    reg[IRT] = reg[IRS] <IIM;
}

void simulator::sra_Instruction(){
   /* int sign=((0x80000000&reg[RRT])>>31);

    if(sign){
        int temp = (int)(reg[RRT] >>r_instruction.shamt);
        temp *= -1;
        reg[RRD]= (unsigned int)temp;
    }
    else{
        reg[RRD] = reg[RRT] >>r_instruction.shamt;
    }*/
    reg[RRD] = reg[RRT] >>r_instruction.shamt;

}

void simulator::srl_Instruction(){
    unsigned int temp = (unsigned int) reg[RRT];
    reg[RRD] =temp;
}

void simulator::sub_Instruction(){
    reg[RRD] = reg[RRS] - reg[RRT];
}

void simulator::sw_Instruction(){
   D_mem_byte[reg[IRS]+IIM] = ((unsigned int)(reg[IRT]&0xFF000000))>>24;
   D_mem_byte[reg[IRS]+IIM+1] = (reg[IRT]&0x00FF0000)>>16;
   D_mem_byte[reg[IRS]+IIM+2] = (reg[IRT]&0x0000FF00)>>8;
   D_mem_byte[reg[IRS]+IIM+3] = reg[IRT]&0x000000FF;
}

void simulator::xor_Instruction(){
    reg[RRD] = reg[RRS]^reg[RRT];
}
