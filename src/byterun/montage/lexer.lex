%{
    #include <stdlib.h>
    #include "ast.h"
    #include "y.tab.h"
%}


nls "\n"|"\r"|"\r\n"
nums "-"?[0-9]+
idents [a-zA-Z][a-zA-Z0-9]*

%%
[ \t]  { /* On ignore */ }
{nls}  {  }

"{" return(LACO);
"}" return(RACO);
"(" return(LPAR);
")" return(RPAR);
"[" return(LCO);
"]" return(RCO);
":" return(COLON);
";" return(PV);
"," return(VRG);
"*" return(STAR);
"->" return(FLECH);

"led" return(LED);
"pin" return(PIN);
"button" return(BUTTON);
"nb_buttons" return (NBBUTTON);
"nb_leds" return(NBLED);
"nb_pins" return(NBPINS);
"screen" return(SCREEN);

{nums}    {
    yylval.num=atoi(yytext);
    return(NUM);
}

{idents}    {
    yylval.str=strdup(yytext);
    return(IDENT);
}

