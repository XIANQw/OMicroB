/* C-interface between:                             */
/*   -> prims/bindings.c                            */
/*   -> simul/sf-regs.c | microbit/microbitlib.c    */

#include <stdint.h>
#include <stdbool.h>

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifndef PRIMS_H_MICROBIT
#define PRIMS_H_MICROBIT

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN8 8
#define PIN12 12
#define PIN16 16

#define BUF_SIZE 50

char msg_r[BUF_SIZE];
char msg_w[BUF_SIZE];
int buf_ptr, ptr_head;
bool button[2];
bool flag_simul[1];
char image[30];
bool pins_val[20];
bool pins_mode[20];


void simul_init();
void send_msg(char * str);
void microbit_pin_mode(int p, int m);
void microbit_digital_write(int p, int l);
int microbit_digital_read(int p);
void microbit_analog_write(int p, int l);
int microbit_analog_read(int p);

/******************************************************************************/

void microbit_write_pixel(int x, int y, int l);
void microbit_print_string(char *str);
void microbit_print_image(char *str);
void microbit_clear_screen();

/******************************************************************************/

int microbit_button_is_pressed(int b);

/******************************************************************************/

void microbit_serial_write(char c);
char microbit_serial_read();

/******************************************************************************/

int microbit_accelerometer_x();
int microbit_accelerometer_y();
int microbit_accelerometer_z();
int microbit_accelerometer_pitch();
int microbit_accelerometer_roll();

/******************************************************************************/

void microbit_compass_calibrate();
int microbit_compass_heading();

/******************************************************************************/
void microbit_radio_init();
void microbit_radio_send(char *s);
const char *microbit_radio_recv();

/******************************************************************************/

void microbit_i2c_init();
void microbit_i2c_write(int a, const char *buf, int n);
int microbit_i2c_read(int a, char *buf);

/******************************************************************************/

void microbit_spi_init();
char microbit_spi_transmit(char c);

#endif

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
