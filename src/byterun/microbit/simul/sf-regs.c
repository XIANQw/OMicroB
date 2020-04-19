/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
#include <stdbool.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
 
#define SERVER_W "serverWrite"
#define SERVER_R "serverRead"

#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11
#define PIN12 12
#define PIN13 13
#define PIN14 14
#define PIN15 15
#define PIN16 16
#define PIN19 19
#define PIN20 20



#define NB_REG 255
#define REG_SIZE 8

bool regs[NB_REG][REG_SIZE];
bool mode[19];


void send_msg(char * str){
    //pipe_write exist or not
    if(access(SERVER_W,0) < 0){
        printf("pipe_write%s doesn't exist\n",SERVER_W);
        return ;
    }
    //open pipe_write
    int fd_w = open(SERVER_W,O_RDWR);
    if(fd_w < 0){
        perror("open pipe_write fail");
    }
    printf("open pipe_write\n");
    if ( write(fd_w, str, strlen(str)+1) == -1){
      perror("server send msg fail");
    }
}


void microbit_print_string(char *str) {
  printf("%s\n", str);
}

void microbit_print_int(int i) {
  printf("%d\n", i);
}

void microbit_write_pixel(int x, int y, int l) {
  if(l == 0) printf("Turning off pixel %d %d\n", x, y);
  else printf("Turning on pixel %d %d at level %d\n", x, y, l);
}

void microbit_print_image(char *str) {
  char image[35];
  for(int y = 0; y < 5; y++) {
    for(int x = 0; x < 5; x++) {
      image[6*y+x] = str[y*5+x]; 
      // printf("%d ", str[y*5+x]);
    }
    image[6*(y+1)-1] = '\n';
    // printf("\n");
  }
  image[29] = '\0';
  send_msg(image);
}

void microbit_clear_screen() {}

int microbit_button_is_pressed(int b) {
  return 0;
}

void microbit_pin_mode(int p, int m) {
  if(m == 0) printf("Setting PIN%d to INPUT\n", p);
  else printf("Setting PIN%d to OUTPUT\n", p);
}

void microbit_digital_write(int p, int l) {
  printf("Writing value %d to pin %d\n", l, p);
}

void microbit_analog_write(int p, int l) {
  printf("Writing value %d to pin %d\n", l, p);
}

int microbit_analog_read(int p) {
  return 0;
}

int microbit_digital_read(int p) {
  return 0;
}

void microbit_delay(int ms) {
  usleep((useconds_t) ms * 1000);
}

int microbit_millis() {
  printf("millis()\n");
  return 0;
}

/******************************************************************************/

void microbit_serial_write(char c) {
  printf("%c", c);
}

char microbit_serial_read() {
  return 0;
}

/*****************************************************************************/

int microbit_accelerometer_x() { return 0; }
int microbit_accelerometer_y() { return 0; }
int microbit_accelerometer_z() { return 0; }
int microbit_accelerometer_pitch() { return 0; }
int microbit_accelerometer_roll() { return 0; }

/*****************************************************************************/

void microbit_compass_calibrate() {}
int microbit_compass_heading() { return 0; }

/*****************************************************************************/

void microbit_radio_init() {}

void microbit_radio_send(char *s) {
  printf("Sending %s by radio\n", s);
}

const char *microbit_radio_recv() {
  return "a";
}

/*****************************************************************************/

void microbit_i2c_init() {}

void microbit_i2c_write(int a, const char *buf, int l) {}

int microbit_i2c_read(int a, char *buf) {
  return 0;
}

/*****************************************************************************/

void microbit_spi_init() {}
char microbit_spi_transmit(char c) {
  printf("Emited %d\n", c);
  return c;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
