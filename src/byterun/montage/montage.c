#include "../simul/shared.h"
#include "montage.h"

#include <stdio.h>
#include <string.h>

Env env;

void init_env(int shmid){
    void *vshm=shmat(shmid, 0, 0);
    if(vshm < 0){
        perror("vshm shmat failed"); exit(0);
    }
    env=(Env)vshm;
    printf("envid=%d, memory attached at %p\n",shmid, env);
}

void add_led(int led, int pin1, int pin2){
    if(led>LED_MAX){
        printf("led %d > max %d", led, LED_MAX); exit(0);
    }
    env->leds[led][0]=pin1;
    env->leds[led][1]=pin2;
}

void add_button(int id, char *label, int pin){
    if(id > BUTTON_MAX){
        printf("button %d > max %d", id, BUTTON_MAX); exit(0);
    }
    strcpy(env->buttons[id].label, label);
    env->buttons[id].pins=pin;
}


void printCmd(Cmd cmd) {
    int pin1=-1, pin2=-1;
    switch(cmd->tag) {
        case T_LED: 
            if(cmd->content._led.pins) {
                pin1=cmd->content._led.pins->pin;
                if(cmd->content._led.pins->next) pin2=cmd->content._led.pins->next->pin;
            }
            add_led(cmd->content._led.id, pin1, pin2);
            break;
        case T_BUTTON: 
            add_button(cmd->content._button.id, cmd->content._button.label, cmd->content._button.pins->pin);
        break;
    }
}

void printPins(Pins pins){
    while(pins->next){
        pins=pins->next;
    }
}

void printCmds(Cmds cmds){
    if(!cmds) return ;
    while(cmds->next){
        printCmd(cmds->head);
        cmds = cmds->next;
    }
    printCmd(cmds->head);
}

void printParam(Param param){
    switch (param->tag)
    {
    case NBPINS_PAR:
        env->nb_pins_row=param->param1;
        env->nb_pins_col=param->param2;
        break;
    case NBLED_PAR:
        env->nb_leds=param->param1;
        break;
    case NBBUTTON_PAR:
        env->nb_buttons=param->param1;
        break;
    case SCREEN_PAR:
        env->screen_row=param->param1;
        env->screen_col=param->param2;
        break;
    default:
        break;
    }
}

void printParams(Params params){
    if(!params) return ;
    while(params->next){
        printParam(params->head);
        params = params->next;
    }
    printParam(params->head);
}

void printEnv(Env env){
    printf("%s\n", env->name);
    printf("nbpins=(%d,%d) nbleds=%d\n", env->nb_pins_row, env->nb_pins_col, env->nb_leds);
    printf("leds=[\n");
    for(int i=0; i<env->screen_row; i++){
        for(int j=0; j<env->screen_col; j++){
            int index=i*env->screen_col+j;
            printf("(p%d, p%d) ", env->leds[index][0], env->leds[index][1]);
        }
        printf("\n");
    }
    printf("]\n");
    printf("nb_buttons=%d\n", env->nb_buttons);
    for(int i=0; i<env->nb_buttons; i++){
        printf("button[%d]->(%s, pin%d)\n", i, env->buttons[i].label, env->buttons[i].pins);
    }
}

void printProg(Prog prog, int shmid){
    init_env(shmid);
    strcpy(env->name, prog->name);
    printParams(prog->params);
    printCmds(prog->content);
}
