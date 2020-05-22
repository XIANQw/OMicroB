#include "ast.h"
#define BUTTON_MAX 64
#define LED_MAX 128
#define PIN_MAX 256
#define LABEL_MAX 16


struct button{
    char label[LABEL_MAX];
    int pins;
};

struct _env{
    int nb_pins_row;
    int nb_pins_col;
    int nb_buttons;
    int nb_leds;
    int leds[LED_MAX][2];
    struct button buttons[BUTTON_MAX];
}Env;

void add_led(int led, int pin1, int pin2);
void add_button(int id, char *label, int pin);


void printNum(int n);
void printId(char * id);
void printPins(Pins pins);
void printCmd(Cmd cmd);
void printCmds(Cmds cmds);
void printParam(Param param);
void printProg(Prog prog);

