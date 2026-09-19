/*
    Name 1: Anmol Dubey 
    UTEID 1: ad56328
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *files[], int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(files[i]);
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(&argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

/* Returns the 16-bit word stored at (even) byte address addr */
int read_word(int addr)
{
  int row = addr >> 1;
  int low = MEMORY[row][0];
  int high = MEMORY[row][1];
  return (high << 8) | low;
}

/* Stores a 16-bit word at (even) byte address addr */
void write_word(int addr, int value)
{
  int row = addr >> 1;
  MEMORY[row][0] = value & 0xFF; /* low byte:  bits [7:0] of value */
  MEMORY[row][1] = (value >> 8) & 0xFF; /* high byte: bits [15:8] of value */
}

/* Sign extends to 16 bits */
int sext(int value, int bits)
{
  int sign_bit = (value >> (bits - 1)) & 1;
  if(sign_bit == 1)
  {
    int field_mask = (1 << bits) - 1;
    value = value | (~field_mask);
  }

  return Low16bits(value);
}

/* Sets N/Z/P in NEXT_LATCHES based on a 16-bit result. */
void setcc(int value)
{
  if (((value >> 15) & 1) == 1)
  { 
    NEXT_LATCHES.N = 1;
    NEXT_LATCHES.Z = 0;
    NEXT_LATCHES.P = 0;
  }
  else if (value == 0)
  {
    NEXT_LATCHES.N = 0;
    NEXT_LATCHES.Z = 1;
    NEXT_LATCHES.P = 0;
  }
  else
  {
    NEXT_LATCHES.N = 0;
    NEXT_LATCHES.Z = 0;
    NEXT_LATCHES.P = 1;
  }
}

void process_instruction()
{
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */

  /* Fetch Phase */
  int instr = read_word(CURRENT_LATCHES.PC);
  int pc_inc = Low16bits(CURRENT_LATCHES.PC + 2);
  NEXT_LATCHES.PC = pc_inc;

  /* ---------- DECODE ---------- */
  int opcode = (instr >> 12) & 0xF;
  int dr = (instr >> 9) & 0x7;  /* bits [11:9] */
  int sr1 = (instr >> 6) & 0x7; /* bits [8:6]  */

  /* EXECUTE */
  switch (opcode)
  {

  case 1: /* ADD */
  case 5: /* AND */
  case 9:
  {                                   /* XOR, NOT */
    int mode = (instr >> 5) & 0x1; /* bit [5] */
    int op1 = CURRENT_LATCHES.REGS[sr1];
    int op2;

    if (mode == 0)
    {
      int sr2 = instr & 0x7; /* bits [2:0] */
      op2 = CURRENT_LATCHES.REGS[sr2];
    }
    else
    {
      op2 = sext(instr & 0x1f, 5); /* imm5 */
    }

    int result;
    if (opcode == 1)
      result = op1 + op2; /* ADD */
    else if (opcode == 5)
      result = op1 & op2; /* AND */
    else
      result = op1 ^ op2; /* XOR */

    result = Low16bits(result);
    NEXT_LATCHES.REGS[dr] = result;
    setcc(result);
    break;
  }

  case 13:
  {                                   /* SHF */
    int amount4 = instr & 0xF;       /* bits [3:0], unsigned */
    int dir = (instr >> 4) & 0x1;   /* bit [4] */
    int arith = (instr >> 5) & 0x1; /* bit [5] */
    int src = CURRENT_LATCHES.REGS[sr1];
    int result;

    if (dir == 0)
    { /* left shift */
      result = src << amount4;
    }
    else
    {
      result = src >> amount4; /* logical right shift */
      if (arith == 1 && ((src >> 15) & 1) == 1)
      {
        /* fill the vacated top amount4 bits with 1s */
        result = result | (((1 << amount4) - 1) << (16 - amount4));
      }
    }

    result = Low16bits(result);
    NEXT_LATCHES.REGS[dr] = result;
    setcc(result);
    break;
  }

  case 6:
  { /* LDW */
    int offset6 = sext(instr & 0x3F, 6);
    int addr = Low16bits(CURRENT_LATCHES.REGS[sr1] + (offset6 << 1));
    int value = read_word(addr);

    NEXT_LATCHES.REGS[dr] = Low16bits(value);
    setcc(value);
    break;
  }

  case 7:
  { /* STW */
    int offset6 = sext(instr & 0x3F, 6);
    int addr = Low16bits(CURRENT_LATCHES.REGS[sr1] + (offset6 << 1));

    write_word(addr, CURRENT_LATCHES.REGS[dr]);
    break;
  }

  case 2:
  { /* LDB */
    int boffset6 = sext(instr & 0x3F, 6);
    int addr = Low16bits(CURRENT_LATCHES.REGS[sr1] + boffset6); /* note: no shift */
    int byte = MEMORY[addr >> 1][addr & 1];                   /* row, then column */
    int value = sext(byte, 8);

    NEXT_LATCHES.REGS[dr] = Low16bits(value);
    setcc(value);
    break;
  }

  case 3:
  { /* STB */
    int boffset6 = sext(instr & 0x3F, 6);
    int addr = Low16bits(CURRENT_LATCHES.REGS[sr1] + boffset6);

    MEMORY[addr >> 1][addr & 1] = CURRENT_LATCHES.REGS[dr] & 0xFF;
    break;
  }

  case 14:
  { /* LEA */
    int pcoffset9 = sext(instr & 0x01FF, 9);
    int addr = Low16bits(pc_inc + (pcoffset9 << 1));

    NEXT_LATCHES.REGS[dr] = addr;
    /* no setcc here */
    break;
  }

  case 0:
  { /* BR */
    int n = (instr >> 11) & 0x1;
    int z = (instr >> 10) & 0x1;
    int p = (instr >> 9) & 0x1;

    if ((n && CURRENT_LATCHES.N) || (z && CURRENT_LATCHES.Z) || (p && CURRENT_LATCHES.P))
    {
      int pcoffset9 = sext(instr & 0x01FF, 9);
      NEXT_LATCHES.PC = Low16bits(pc_inc + (pcoffset9 << 1));
    }
    break;
  }

  default:
    break;
  }
}