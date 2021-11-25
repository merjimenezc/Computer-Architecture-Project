# Computer-Architecture-Project


1.	Single-cycle MIPS CPU 
1.1	Overview
Using our HW 3 Decode.c program, our project file can decode a 32-bit input machine code. Our program reads in machine code instructions into an array of characters and stores the first 6 digits for decoding the opcode from the instruction. The remainder digits are sorted into different parts of the type format that they belong to. These are converted into their decimal equivalents and used to be matched to their corresponding register value, function, or operation. 

1.2	Code Structure

Our program reads in user inputted machine code instructions into an array of characters and stores the first 6 digits for decoding the opcode from each instruction. We store these 6 digits into a buffer variable and then convert the binary opcode into a decimal value. In our binary to decimal function, we initialize our decimal value to zero and divide each binary value by its decimal base 10. The digits of the binary instruction are extracted one by one from right to left and are multiplied by the power of 2 and are stored in the decimal variable. Once we obtain the decimal equivalent of the machine code instruction, there is a set of statements that will determine the type of instruction we are working with. If the opcode number is equal to 0 then it is R type, if it is greater than 3 then it is I type, and if it is equal to 2 or 3 then it is a J type instruction. Based on the type of instruction, we created temporary arrays to store the characters for each part of the instruction. In the R type instruction, we store the characters into 5 different temporary arrays with different sizes that represent the following formats: Rs, Rt, Rd, Shamt, and Funct values. In the I type instruction, we store the characters into 3 different temporary arrays with different sizes that represent the formats: Rs, Rt, and Immediate values. In the J type instruction, we store the characters into 1 temporary array with a size of 26 bits for the address format value. After storing these instructions into their corresponding formats and values, these are decoded through the function decode and register decode functions by using their previously obtained decimal value. In the register decode function, each instruction stored in the rs, rt, or rd variables will pass through a set of statements to determine the value of its corresponding register name based on its decimal value. In the function decode and op decode functions, each instruction stored in the operation and function variable will pass through a set of statements that will determine the function name by the decimal value of the function and opcode operations.

1.2.1	Functions
We used a multitude of different functions for our Single-cycled MIPS CPU. The main function was used to read in the file and store it in a buffer variable. From there, we concatenated the first 6 elements and store them in the opcode variable. Using “if-statements" we determined what the format the instruction was and called the corresponding function. We used functions called: void JFormat, void IFormat, and void RFormat. These were used to determine the exact instruction. We used the function int BinarytoDecimal so that we could convert each field of the instruction to decimal and determine what the value was for the instruction. These functions were used to decode the different field values: char *registerDecode, *char functionDecode, and *char opDecode. The void Mem function is used to either store the value from a SW instruction into data memory, or pull a value from data memory and store it in a register. The void ControlUnit function was used to determine the control signals depending on the instruction.
1.2.2	Variables
We made sure to define all 8 control signals as global variables: jump, RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, and branch as integer values and set them equal to 0. Also defined InstType as an integer array or 2 elements, which is initialized to 0. Next, we have global variables: pc, next_pc, branch_target, and jump_target as integers and initialized them to 0. The variable int dmem is an integer array used to store the data memory.
1.3	Execution Results

  
1.4	Challenges and Limitations
 We encountered some challenges during the implementation of our single-cycle MIPS CPU portion of this assignment. The fetch functionality of the project was not completely implemented, and it is not working properly. This might be due to an issue with the storage of the variable that is being used to store the character instructions. There are no errors when opening the file as it opens successfully. Our program has some limitations due to the missing implementation of the execute functionality and the control unit of the single-cycle CPU. Without many of these functionalities, we were unable to get our program to work correctly. However, we succeeded at implementing a successful decode function for this portion of the project.
2.	Pipelined MIPS CPU 
2.1	Overview
// Explain the overall structure of your pipelined CPU program
2.2	Baseline Code Structure
// Explain the detailed code structure that shows the functions and variables with how you implemented and how those are interacting with the other functions or variables.
// Especially in pipelined CPU, explain how you handled NOPs and Flushes (e.g., implementing dependency checking logic and so on)
2.2.1	Functions
2.2.2	Variables
2.3	 Optimizations
// If you implemented data forwarding, explain how you implemented here. If otherwise, don’t need to add this section. 
2.4	Execution Results
// Show how to run your program and a sample output screenshot
// If you implemented data forwarding, please add one more sample output with data forwarding that may have fewer cycles. 
2.5	Challenges and Limitations
We encountered challenges during the implementation of the Pipelined MIPS CPU program. We were unable to implement any of the functionalities and logic required for the program and therefore some parts do not work properly. 

![image](https://user-images.githubusercontent.com/44124264/143346691-6f8e4c58-504b-48a8-a336-9f6c799b3940.png)
