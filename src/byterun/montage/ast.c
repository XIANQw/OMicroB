#include <stdio.h>
#include "ast.h"

Pins appendPins(int pin_id, Pins pins){
    Pins res = mallocPins;
    res->pin=pin_id;
    res->next=pins;
    return res;
}

Param newParam(int nbpins_row, int nb_pins_col, int nbleds, int nbbuttons){
    Param res=mallocParam;
    res->nb_pins_row=nbpins_row;
    res->nb_pins_col=nb_pins_col;
    res->nb_leds=nbleds;
    res->nb_buttons=nbbuttons;
    return res;
}

// Cmds
Cmd newLed(int id, Pins pins){
    Cmd res=mallocCmd;
    res->tag=T_LED;
    res->content._led.id=id;
    res->content._led.pins=pins;
    return res;
}

Cmd newButton(int id, char *label, Pins pins){
    Cmd res=mallocCmd;
    res->tag=T_BUTTON;
    res->content._button.id=id;
    res->content._button.label=label;
    res->content._button.pins=pins;
    return res;
}
// Commande
Cmds appendCmds(Cmd cmd, Cmds cmds) {
    Cmds res = mallocCmds;
    res->head = cmd;
    res->next = cmds;
    return res;
}

// Prog
Prog newProg(Param param, Cmds cmds){
    Prog res = mallocProg;
    res->param= param;
    res->content =cmds;
    return res;
}













