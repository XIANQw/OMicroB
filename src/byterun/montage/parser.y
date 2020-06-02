%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "montage.h"

    int yylex (void);
    int yyerror (char *);
    Prog theProg;
%}

%token<num>  NUM
%token<str>  IDENT

// bloc
%token  LPAR RPAR LCO RCO
// symbol
%token  NL COLON PV VRG STAR FLECH
%token PIN LED BUTTON
%token NBBUTTON NBLED NBPINS

%union {
    int num;
    Param param;
    char* str;
    Pins pins;
    Cmds cmds;
    Cmd cmd;
    Prog prog;
}
%type<num> pin
%type<param> param
%type<pins> pins
%type<cmd> cmd
%type<cmds> cmds
%type<prog> prog

%start prog
%%

param:
NBPINS NUM VRG NBLED NUM VRG NBBUTTON NUM {$$= newParam($2, -1, $5, $8);}
|NBPINS LPAR NUM VRG NUM RPAR VRG NBLED NUM VRG NBBUTTON NUM {$$= newParam($3, $5, $9, $12);}
;

pin:
PIN NUM {$$ = $2;}
;

pins:
pin {$$ = appendPins($1, NULL);}
|pin VRG pins {$$ = appendPins($1, $3);}
;
cmd:
LED NUM pins {$$= newLed($2, $3);}
|BUTTON NUM IDENT pins {$$= newButton($2, $3, $4); }
;
cmds:
cmd PV{$$= appendCmds($1, NULL);}
|cmd PV cmds {$$ = appendCmds($1, $3);}
;
prog:
param LCO cmds RCO {theProg = newProg($1,$3);}
;

%%

int yyerror(char *s) {
    printf("error: %s\n",s);
    return 1;
}

int main(int argc, char **argv) {
    if(argc < 2){
        printf("argc=%d", argc); exit(0);
    }
    int shmid = atoi(argv[1]);
    freopen("/tmp/circuit.txt", "r", stdin);
    yyparse();
    printProg(theProg, shmid);
    printf("\n");
    return 0;
}
