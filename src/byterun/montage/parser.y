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
%token LPAR RPAR LCO RCO LACO RACO
// symbol
%token NL COLON PV VRG STAR FLECH
%token PIN LED BUTTON
%token NBBUTTON NBLED NBPINS SCREEN

%union {
    int num;
    Param param;
    Params params;
    char* str;
    Pins pins;
    Cmds cmds;
    Cmd cmd;
    Prog prog;
}
%type<param> param
%type<params> params
%type<cmd> cmd
%type<cmds> cmds
%type<prog> prog

%start prog
%%

param:
NBPINS NUM {$$= newParam($2, 0, NBPINS_PAR);}
|NBPINS LPAR NUM VRG NUM RPAR {$$= newParam($3, $5, NBPINS_PAR);}
|NBBUTTON NUM {$$= newParam($2, 0, NBBUTTON_PAR);}
|NBLED NUM {$$= newParam($2, 0, NBLED_PAR); }
|SCREEN LPAR NUM VRG NUM RPAR {$$= newParam($3, $5, SCREEN_PAR);}
;
params:
param {$$= appendParams($1, NULL);}
|param VRG params {$$= appendParams($1, $3);}
;

cmd:
LED NUM COLON LPAR NUM VRG NUM RPAR VRG PIN NUM VRG PIN NUM {$$= newLed($2, $5, $7, $11, $14);}
|BUTTON NUM COLON IDENT PIN NUM {$$= newButton($2, $4, $6); }
;
cmds:
cmd PV{$$= appendCmds($1, NULL);}
|cmd PV cmds {$$ = appendCmds($1, $3);}
;
prog:
IDENT LACO params LCO cmds RCO RACO {theProg = newProg($1, $3, $5);}
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
    return 0;
}
