#include <stdlib.h>
typedef enum{
    T_LED, T_BUTTON
}TagCmd;

typedef struct _prog * Prog;
typedef struct _cmds * Cmds;
typedef struct _cmd * Cmd;
typedef struct _pins* Pins;
typedef struct _param * Param;

struct _pins{
    int pin;
    Pins next;
};

struct _param{
    int nb_pins_row;
    int nb_pins_col;
    int nb_leds;
    int nb_buttons;
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
    Param param;
    Cmds content;
};

//init
Param newParam(int nbpins_row, int nbpins_col, int nbled, int nbbutton);
//Pins
Pins appendPins(int pin_id, Pins pins);
// Cmds
Cmd newLed(int id, Pins pins);
Cmd newButton(int id, char *label, Pins pins);
Cmds appendCmds(Cmd cmd, Cmds cmds);
// Prog
Prog newProg(Param param, Cmds cmds);

#define mallocParam (Param)malloc(sizeof(struct _param))
#define mallocPins (Pins)malloc(sizeof(struct _pins))
#define mallocCmd (Cmd)malloc(sizeof(struct _cmd))
#define mallocCmds (Cmds)malloc(sizeof(struct _cmds))
#define mallocProg (Prog)malloc(sizeof(struct _prog))



