#include <stdio.h>
#include "ast.h"

Pins appendPins(int pin_id, Pins pins){
    Pins res = mallocPins;
    res->pin=pin_id;
    res->next=pins;
    return res;
}

Param newParam(int param1, int param2, TagParam tag){
    Param res=mallocParam;
    res->tag=tag;
    res->param1 = param1;
    res->param2 = param2;
    return res;
}

Params appendParams(Param par, Params pars){
    Params res=mallocParams;
    res->head=par;
    res->next=pars;
    return res;
}

// Cmds
Cmd newLed(int id, int row, int col, int pin_row, int pin_col){
    Cmd res=mallocCmd;
    res->tag=T_LED;
    res->content._led.id=id;
    res->content._led.row=row;
    res->content._led.col=col;
    res->content._led.pin_row=pin_row;
    res->content._led.pin_col=pin_col;
    return res;
}

Cmd newButton(int id, char *label, int pin){
    Cmd res=mallocCmd;
    res->tag=T_BUTTON;
    res->content._button.id=id;
    res->content._button.label=label;
    res->content._button.pin=pin;
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
Prog newProg(char *name, Params params, Cmds cmds){
    Prog res = mallocProg;
    res->name=name;
    res->params= params;
    res->content =cmds;
    return res;
}













