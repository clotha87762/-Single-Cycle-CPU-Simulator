#ifndef simulator_h
#define simulator_h
#endif
#include<iostream>

enum instruction_name {ADD,SUB,AND,OR,XOR,NOR,NAND,SLT,SLL,SRL,SRA,JR,ADDI,LW,LH,LHU,LB,LBU
                        SW,SH,SB,LUI,ANDI,ORI,NORI,SLTI,BEQ,BNE,J,JAL,HALT};

struct RInstruction{
    int opcode;
    int rs;
    int rt;
    int rd;
    int schmt;
    int funct;
};

struct IInstruction{
    int opcode;
    int rs;
    int rt;
    int immediate;
};

struct JInstruction{
    int opcode;
    int address;

};
typedef struct RInstruction Rinstruction;
typedef struct IInstruction Iinstruction;
typedef struct JInstruction Jinstruction;
class simulator{
   public :
    unsigned int PC;
    unsigned int I_mem[256]={0};
    char D_mem[1025]={0};
    int reg[32]={0};
    int I_number;
    int D_number;

    Rinstruction r_instruction;
    Iinstruction i_instruction;
    Jinstruction j_instruction;

    void add_Instruction();
    void sub_Instruction();
    void and_Instruction();
    void or_Instruction();
    void xor_Instruction();
    void nor_Instruction();
    void nand_Instruction();
    void slt_Instruction();
    void sll_Instruction();
    void srl_Instruction();
    void sra_Instruction();
    void jr_Instruction();
    void addi_Instruction();
    void lw_Instruction();
    void lh_Instruction();
    void lhu_Instruction();
    void lb_Instruction();
    void lbu_Instruction();
    void sw_Instruction();
    void sh_Instruction();
    void sb_Instruction();
    void lui_Instruction();
    void andi_Instruction();
    void ori_Instruction();
    void nori_Instruction();
    void slti_Instruction();
    void beq_Instruction();
    void bne_Instruction();
    void j_Instruction();
    void jr_Instruction();
    void halt_Instruction();

    void PrintReg();

};

