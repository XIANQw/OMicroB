#include "prologTerm.h"
#include <stdio.h>
#include <string.h>

void add_led(int led, int pin1, int pin2){
    if(led>LED_MAX){
        printf("led %d > max %d", led, LED_MAX); exit(0);
    }
    Env.leds[led][0]=pin1;
    Env.leds[led][1]=pin2;
}

void add_button(int id, char *label, int pin){
    if(id > BUTTON_MAX){
        printf("button %d > max %d", id, BUTTON_MAX); exit(0);
    }
    strcpy(Env.buttons[id].label, label);
    Env.buttons[id].pins=pin;
}


void printNum(int n){
    printf("%d", n);
}
void printId(char * id){
    printf("%s", id);
}

void printCmd(Cmd cmd) {
    int pin1=-1, pin2=-1;
    switch(cmd->tag) {
        case T_LED: 
            printf("Led(");
            printf("%d,", cmd->content._led.id);
            printPins(cmd->content._led.pins);
            printf(")");
            if(cmd->content._led.pins) {
                pin1=cmd->content._led.pins->pin;
                if(cmd->content._led.pins->next) pin2=cmd->content._led.pins->next->pin;
            }
            add_led(cmd->content._led.id, pin1, pin2);
            break;
        case T_BUTTON: 
            printf("Button(");
            printf("%d,%s,", cmd->content._button.id, cmd->content._button.label);
            printPins(cmd->content._button.pins);
            printf(")");  
            add_button(cmd->content._button.id, cmd->content._button.label, cmd->content._button.pins->pin);
        break;
    }
}

void printPins(Pins pins){
    printf("Pins(");
    while(pins->next){
        printf("%d,", pins->pin);   
        pins=pins->next;
    }
    printf("%d", pins->pin);
    printf(")");
}

void printCmds(Cmds cmds){
    if(!cmds) return ;
    while(cmds->next){
        printCmd(cmds->head);
        printf(",");
        cmds = cmds->next;
    }
    printCmd(cmds->head);
}

void printParam(Param param){
    printf("Param(");
    printf("(%d, %d),%d,%d",param->nb_pins_row, param->nb_pins_col, param->nb_leds, param->nb_buttons);
    printf(")");
    Env.nb_pins_row=param->nb_pins_row;
    Env.nb_pins_col=param->nb_pins_col;
    Env.nb_buttons=param->nb_buttons;
    Env.nb_leds=param->nb_leds;
}

void printEnv(){
    printf("nbpins=(%d,%d) nbleds=%d\n", Env.nb_pins_row, Env.nb_pins_col, Env.nb_leds);
    for(int i=0; i<Env.nb_leds; i++){
        printf("led[%d]->(pin%d, pin%d)\n", i, Env.leds[i][0], Env.leds[i][1]);
    }
    printf("nb_buttons=%d\n", Env.nb_buttons);
    for(int i=0; i<Env.nb_buttons; i++){
        printf("button[%d]->(%s, pin%d)\n", i, Env.buttons[i].label, Env.buttons[i].pins);
    }

}

void printProg(Prog prog){
    printParam(prog->param);
    printf("\ncircuit([");
    printCmds(prog->content);
    printf("]).\n");
    printEnv();
}
