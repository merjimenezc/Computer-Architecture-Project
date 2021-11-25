//  Decode.c
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

//Test cases:
//00000001000010011000100000100000 - Result: Instruction Type: R, Operation: add, Rs: t0 (R8), Rt: t1 (R9), Rd: s1 (R17), Shamt: 0, Funct: 32

//00000001001010100100000000100010 - Result: Instruction Type: R, Operation: sub, Rs: t1 (R9), Rt: t2 (R10), Rd: t0 (R8), Shamt: 0, Funct: 34

//00110100001001000000000000000000 - Result: Instruction Type: I, Operation: ori, Rs: at (R1), Rt: a0 (R4), Immediate: 0

//00100010011010100000000000000100 - Result: Instruction Type: I, Operation: addi, Rs: s3 (R19), Rt: t2 (R10), Immediate: 4

//10001101000010000000000000001100 - Result: Instruction Type: I, Operation: lw, Rs: t0 (R8), Rt: t0 (R8), Immediate: 12

//00001000000000000000000000000100 - Result: Instruction Type: J, Operation: j, Address: 4

//00001000000000000000000000000000 - Result: Instruction Type: J, Operation: j, Address: 0

//Function to handle R type instruction
static void RFormat(char instruction[], int Opcode);
//Function to handle I type instruction
static void IFormat(char instruction[], int Opcode);
//Function to handle J type instruction
static void JFormat(char instruction[], int Opcode);
unsigned int Fetch (int address);
unsigned int endianSwap(unsigned int);


//Function to decode register - convert int value into a string for R, J, and I type instructions
char *registerDecode(int regNum);
//Function to decode function - convert int value for operations into a string
char *FunctionDecode(int functNum);
//Function to decode operation - convert int value for operations into a string
char *opDecode(int opNum);
//convert decimal to hexadecimal value
char DecimaltoHex(int decimal);
//convert a binary number into an int value
int BinarytoDecimal(signed int binary);
//int Execute(instruction*, regNum*);
static void Mem(int address, int opNum, int rs, int offset);
static void ControlUnit(int OpNum);
int jump, RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, branch = 0;
int InstType[2] = {0, 0};
//Declaring global variable + initializing to 0
int pc = 0;
int next_pc = 0;
int branch_target;
//Initializing jump_target
int jump_target = 0;

//Declaring function
int main()
{

    //unsigned int Fetch (int){
    char buffer[1000], Opcode[7], imm[17];
    int opcodeNum;
    //Take in 32-bit user input in binary
    //printf("Enter an instruction in machine code:\n");

    //unsigned int Fetch (int addr)
    //char *line = NULL;
    //size_t len = 0;
    int i = 0;
    int text;
    FILE *file;
    /*FILE *fptr;
       if ((fptr = fopen("sample_binary.txt", "r")) {
           while (fscanf (fptr, "%d", &buffer[i]) !=EOF)
           {
               ++i;
           }
           fclose(fptr);*/
          //printf("Error! opening file");
           // Program exits if file pointer returns NULL.
    if((file = fopen("sample_binary.txt", "r")) == NULL)
    {
        printf("Error opening file");
        //Program exists if file pointer returns NULL
        exit(1);
    }
    fscanf (file, "%[^\n]", buffer);
    //Tester to see if file was succesfully fetched
    printf("Data from the file: \n%s", buffer);
    printf("\n");
    fclose(file);

    //printf("this is a test");
    //Read machine code instruction into a char array buffer
    for (int i = 0; i < 35; i++)
    {
        //buffer[i] = getchar();
        //Increasing PC value by 4
        pc =+4;
        //Storing pc + 4 value into next pc value
        next_pc = pc;
        if(i < 6)
            //Store 6 first digits for decoding Opcode
            Opcode[i] = buffer[i];
    }
    //for(int i = 0; i < 6; i++){
    //printf("%s", Opcode);
    //}
    //converts the string argument str to an int (Atoi)
    //convert binary opcode into decimal value
    opcodeNum = BinarytoDecimal(atoi(Opcode));

    //For R type instructions only - Only opcode equal to zero
    if(opcodeNum == 0)
        RFormat(buffer,opcodeNum);

    //For I type instructions only - Any number greater than 3
    if(opcodeNum > 3)
        IFormat(buffer,opcodeNum);

    //For J type instructions only - Numbers 2 and 3
    if(opcodeNum == 2 || opcodeNum == 3)
        JFormat(buffer, opcodeNum);
}
//Jordan and Mercedes collaborated
//Declaring J Format Function
void JFormat(char instruction[], int Opcode)
{
     //Displaying Function info for R type instruction
    printf("Instruction Type: J\n");

    //Creating arrays to store char for each instruction
    char temp[26];
    for (int i = 6; i < 32; i++)
        temp[i - 6] = instruction[i];

    //Displaying Function info for J type instruction
    char *operation = opDecode(Opcode);
    printf("Operation: %s\n",operation);

    int address = BinarytoDecimal(atoi(temp));
    printf("Address: %d\n", address);
}
//Declaring I Format Function - MJ
void IFormat(char instruction[], int Opcode)
{
    //Displaying Function info for R type instruction
    printf("Instruction Type: I\n");

    //Creating arrays to store char for each instruction
    char temp[7], temp1[6], temp2[17];
    for (int i = 6; i < 11; i++)
        temp[i - 6] = instruction[i];
    //Bits 11 to 16
    for (int i = 11; i < 16; i++)
        temp1[i - 11] = instruction[i];
    //Bits 16 to 32
    for (int i = 16; i < 32; i++)
        temp2[i - 16] = instruction[i];

    //Displaying Function info for I type instruction
    char *operation = opDecode(Opcode);
    printf("Operation: %s\n",operation);

    int rsNum = BinarytoDecimal(atoi(temp));
    char *rs = registerDecode(rsNum);
    printf("Rs: %s\n", rs);

    int rtNum = BinarytoDecimal(atoi(temp1));
    char *rt = registerDecode(rtNum);
    printf("Rt: %s\n", rt);

    int imm = BinarytoDecimal(atoi(temp2));
    printf("Immediate: %d\n", imm);
}
//Jordan worked on this
//Declaring R Format Function
void RFormat(char instruction[], int Opcode)
{
    //Creating arrays to store char for each instruction
    char temp[6], temp1[6], temp2[6], temp3[6], temp4[7];
    for (int i = 6; i < 11; i++)
        temp[i - 6] = instruction[i];

    for (int i = 11; i < 16; i++)
        temp1[i - 11] = instruction[i];

    for (int i = 16; i < 21; i++)
        temp2[i - 16] = instruction[i];

    for (int i = 21; i < 26; i++)
        temp3[i - 21] = instruction[i];

    for (int i = 26; i < 32; i++)
        temp4[i - 26] = instruction[i];

    //Displaying Function info for R type instruction
    printf("Instruction Type: R\n");

    int funct = BinarytoDecimal(atoi(temp4));

    char *operation = FunctionDecode(funct);
    printf("Operation: %s\n",operation);

    int rsNum = BinarytoDecimal(atoi(temp));
    char *rs = registerDecode(rsNum);
    printf("Rs: %s\n", rs);

    int rtNum = BinarytoDecimal(atoi(temp1));
    char *rt = registerDecode(rtNum);
    printf("Rt: %s\n", rt);

    int rdNum = BinarytoDecimal(atoi(temp2));
    char *rd = registerDecode(rdNum);
    printf("Rd: %s\n", rd);

    int shamt = BinarytoDecimal(atoi(temp3));
    printf("Shamt: %d\n", shamt);

    funct = BinarytoDecimal(atoi(temp4));
    printf("Funct: %d\n", funct);

}
//Conversion from Binary to Decimal values
int BinarytoDecimal(signed int binary)
{
    //Initializing decimal
    int decimal = 0;

    for(int i = 0; binary; i++, binary /= 10)
        if(binary % 10)
            decimal += pow(2, i);
    return decimal;
}
//Conversion from Decimal to Hex - Jordan
char DecimaltoHex(int decimal) {
    int i = 0;
    int *remainder;
    while(decimal >= 0) {
        remainder[i] = decimal % 16;
        decimal /= 16;
        i++;
    }
    char hex[i];
    for(i; i < 0; i--) {
        if(remainder[i] == 10){
            hex[i] = 'A';
    }   else if(remainder[i] == 11) {
            hex[i] = 'B';
    }   else if(remainder[i] == 12) {
            hex[i] = 'C';
    }   else if(remainder[i] == 13) {
            hex[i] = 'D';
    }   else if(remainder[i] == 14) {
            hex[i] = 'E';
    }   else if(remainder[i] == 15) {
            hex[i] = 'F';
    }   else {
            hex[i] = remainder[i];
    }
    return hex[i];
}
}
//Function for decoding register value - Given on refernce sheet
char *registerDecode(int regNum)
{
    //Creating array of char
    static char *reg;
    //use int value to return corresponding register name
    if(regNum == 0)
        reg = "zero (R0)";
    else if(regNum == 1)
        reg = "at (R1)";
    else if(regNum == 2)
        reg = "v0 (R2)";
    else if(regNum == 3)
        reg = "v1 (R3)";
    else if(regNum == 4)
        reg = "a0 (R4)";
    else if(regNum == 5)
        reg = "a1 (R5)";
    else if(regNum == 6)
        reg = "a2 (R6)";
    else if(regNum == 7)
        reg = "a3 (R7)";
    else if(regNum == 8)
        reg = "t0 (R8)";
    else if(regNum == 9)
        reg = "t1 (R9)";
    else if(regNum == 10)
        reg = "t2 (R10)";
    else if(regNum == 11)
        reg = "t3 (R11)";
    else if(regNum == 12)
        reg = "t4 (R12)";
    else if(regNum == 13)
        reg = "t5 (R13)";
    else if(regNum == 14)
        reg = "t6 (R14)";
    else if(regNum == 15)
        reg = "t7 (R15)";
    else if(regNum == 16)
        reg = "s0 (R16)";
    else if(regNum == 17)
        reg = "s1 (R17)";
    else if(regNum == 18)
        reg = "s2 (R18)";
    else if(regNum == 19)
        reg = "s3 (R19)";
    else if(regNum == 20)
        reg = "s4 (R20)";
    else if(regNum == 21)
        reg = "s5 (R21)";
    else if(regNum == 22)
        reg = "s6 (R22)";
    else if(regNum == 23)
        reg = "s7 (R23)";
    else if(regNum == 24)
        reg = "t8 (R24)";
    else if(regNum == 25)
        reg = "t9 (R25)";
    else if(regNum == 26)
        reg = "k0 (R26)";
    else if(regNum == 27)
        reg = "k1 (R27)";
    else if(regNum == 28)
        reg = "gp (R28)";
    else if(regNum == 29)
        reg = "sp (R29)";
    else if(regNum == 30)
        reg = "fp (R30)";
    else if(regNum == 31)
        reg = "ra (R31)";
    else
        reg = "unknown";
    return reg;
}
//Function to decode the functions being used
char *FunctionDecode(int functNum)
{
    //use int value to return corresponding function name
    static char *funct;
    if(functNum == 0)
        funct = "sll";
    else if(functNum == 2)
        funct = "srl";
    else if(functNum == 8)
        funct = "jr";
    else if(functNum == 32)
        funct = "add";
    else if(functNum == 33)
        funct = "addu";
    else if(functNum == 34)
        funct = "sub";
    else if(functNum == 35)
        funct = "subu";
    else if(functNum == 36)
        funct = "and";
    else if(functNum == 37)
        funct = "or";
    else if(functNum == 38)
        funct = "xor";
    else if(functNum == 39)
        funct = "nor";
    else if(functNum == 42)
        funct = "slt";
    else if(functNum == 43)
        funct = "sltu";
    else
        funct = "unknown";
    return funct;
}
//Function for decoding operation value - Given on refence sheet
char *opDecode(int opNum)
{
    //use int value to return corresponding operation name
    static char *op;
    if(opNum == 2)
        op = "j";
    else if(opNum == 3)
        op = "jal";
    else if(opNum == 4)
        op = "beq";
    else if(opNum == 5)
        op = "bne";
    else if(opNum == 8)
        op = "addi";
    else if(opNum == 9)
        op = "addiu";
    else if(opNum == 10)
        op = "slti";
    else if(opNum == 11)
        op = "sltiu";
    else if(opNum == 12)
        op = "andi";
    else if(opNum == 13)
        op = "ori";
    else if(opNum == 15)
        op = "lui";
    else if(opNum == 35)
        op = "lw";
    else if(opNum == 36)
        op = "lbu";
    else if(opNum == 37)
        op = "lhu";
    else if(opNum == 40)
        op = "sb";
    else if(opNum == 41)
        op = "sh";
    else if(opNum == 43)
        op = "sw";
    else if(opNum == 48)
        op = "ll";
    else if(opNum == 56)
        op = "sc";
    else
        op = "unknown";
    return op;
}
//opNum = opcode to specify lw or sw, rs = the source register for sw & destination register for lw
//offset = the number to add to address to get destination address for sw
void Mem (int address, int opNum, int rs, int offset){
    int dmem[32] = { };
    int tracker = 0;
    address = BinarytoDecimal(atoi(address));
    opNum = BinarytoDecimal(atoi(opNum));
    rs = BinarytoDecimal(atoi(rs));
    offset = BinarytoDecimal(atoi(offset));
    //int dest[31] = { };
    if(opNum == 35) {//lw
        for(int i = 0; i < 31; i++){
            if(dmem[i] == 0){
                dmem[i] = address + (offset*4);
                tracker = 0;
            } else{
                tracker = 1;
            }
        }  if(tracker == 1) {
                //end();//??
            exit(1);
            }

        //WriteBack(dmem[0]);
    } else if(opNum == 43){ //sw
        dmem[0] = address + (offset*4);
        dmem[0] = rs;
    }

}
void ControlUnit(int OpNum){
    if(OpNum == 0){//r-type instruction
        jump = 0;
        RegDst = 1;
        ALUSrc = 0;
        MemtoReg = 0;
        RegWrite = 1;
        MemRead = 0;
        MemWrite = 0;
        branch = 0;
       int InstType[] = { 1, 0 };
    } else if(OpNum == 35) {//lw
        jump = 0;
        RegDst = 0;
        ALUSrc = 1;
        MemtoReg = 1;
        RegWrite = 1;
        MemRead = 1;
        MemWrite = 0;
        branch = 0;
        int InstType[] = { 0, 0 };
    } else if(OpNum == 43) { //sw
        jump = 0;
        RegDst = 0;
        ALUSrc = 1;
        MemtoReg = 0;
        RegWrite = 0;
        MemRead = 0;
        MemWrite = 1;
        branch = 0;
        int InstType[2] = {0, 0};
    } else if(OpNum == 4) { //beq
        jump = 0;
        RegDst = 0;
        ALUSrc = 0;
        MemtoReg = 0;
        RegWrite = 0;
        MemRead = 0;
        MemWrite = 0;
        branch = 1;
        int InstType[2] = {0, 1};
    } else if(OpNum == 2) { //jump
        jump = 1;
        RegDst = 0;
        ALUSrc = 0;
        MemtoReg = 0;
        RegWrite = 0;
        MemRead = 0;
        MemWrite = 0;
        branch = 0;
        int InstType[2] = {0, 0};
    }
   // void ALUCtrl;
}
