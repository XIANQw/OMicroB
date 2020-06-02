#include "ast.h"

void add_led(int led, int row, int col, int pin1, int pin2);
void add_button(int id, char *label, int pin);


void printNum(int n);
void printId(char * id);
void printPins(Pins pins);
void printCmd(Cmd cmd);
void printCmds(Cmds cmds);
void printParam(Param param);
void printProg(Prog prog, int shmid);

