#ifndef CLIENT_H
#define CLIENT_H

#include "gui.h"
#include <pthread.h> //pthread_t
#include <unistd.h> //read, write
#include <sys/stat.h> //mkfifo
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <signal.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "../simul/shared.h"

#define SCREEN_SIZE 5
#define NB_PIN 16

#define BIT(i) (1 << (i))
#define SET_BIT(reg, n) reg |= BIT(n)
#define CLR_BIT(reg, n) reg &= ~BIT(n)
#define READ_BIT(reg, n) ((reg>>n) & 0b1)


extern GtkWidget* screen[SCREEN_SIZE][SCREEN_SIZE];
extern GtkWidget** leds, **pin_row, **pin_col;
extern int bval[2];
extern int64_t pins_mode;
extern int64_t pins_niveau; 
extern int *pins_vals;
extern int leds_state;

extern pid_t server_pid, mypid;
extern void *vshm1, *vshm2, *venv;
extern struct shared_use_st *shm1, *shm2;
extern int shm2id, shm1id, envid;
extern Env shm_env;


void shm_init();
void print_Env(Env shm_env);
void print_pins_niveau();
void modify_screen(int x, int y, int v);
void clear_screen();
void flush_screen();
void send_msg(int code);
void* gui_lisener(void * arg);

#endif