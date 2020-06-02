#include <stdlib.h>

typedef enum{
    T_LED, T_BUTTON
}TagCmd;

typedef enum{
    NBPINS_PAR, NBBUTTON_PAR, NBLED_PAR, SCREEN_PAR
}TagParam;

typedef struct _prog * Prog;
typedef struct _cmds * Cmds;
typedef struct _cmd * Cmd;
typedef struct _pins* Pins;
typedef struct _param * Param;
typedef struct _params * Params;

struct _pins{
    int pin;
    Pins next;
};

struct _param{
    int param1;
    int param2;
    TagParam tag;
};

struct _params
{
    Param head;
    Params next;
};


struct _cmd{
    TagCmd tag;
    union{
        struct{
            int id;
            Pins pins;
        }_led;
        struct{
            int id;
            char *label;
            Pins pins;
        }_button;
    }content;
};

struct _cmds{
    Cmd head;
    Cmds next;
};

struct _prog{
    char * name;
    Params params;
    Cmds content;
};

//param
Param newParam(int param1, int param2, TagParam tag);
Params appendParams(Param param, Params params);
//Pins
Pins appendPins(int pin_id, Pins pins);
// Cmds
Cmd newLed(int id, Pins pins);
Cmd newButton(int id, char *label, Pins pins);
Cmds appendCmds(Cmd cmd, Cmds cmds);
// Prog
Prog newProg(char *name, Params params, Cmds cmds);

#define mallocParam (Param)malloc(sizeof(struct _param))
#define mallocParams (Params)malloc(sizeof(struct _params))
#define mallocPins (Pins)malloc(sizeof(struct _pins))
#define mallocCmd (Cmd)malloc(sizeof(struct _cmd))
#define mallocCmds (Cmds)malloc(sizeof(struct _cmds))
#define mallocProg (Prog)malloc(sizeof(struct _prog))



