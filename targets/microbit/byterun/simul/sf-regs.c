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

#define SERVER_W "/home/xian/Projets/M1-S2/PSTL/OMicroB/src/client/serverWrite"
#define SERVER_R "/home/xian/Projets/M1-S2/PSTL/OMicroB/src/client/serverRead"

#define BUF_SIZE 50
char buf[BUF_SIZE];
char image[30];

void send_msg(char * str){
    //pipe_write exist or not
    if(access(SERVER_W,0) < 0){
        printf("pipe_write %s doesn't exist\n",SERVER_W);
        return ;
    }
    //open pipe_write
    int fd_w = open(SERVER_W, O_RDWR);

    if(fd_w < 0){
        printf("open pipe_write fail %d\n", fd_w);
        exit(0);
    }
    printf("send: len=%ld fd_w=%d\n%s\n", strlen(str), fd_w, str);
    if ( write(fd_w, str, strlen(str)+1) == -1){
      perror("server send msg fail");
    }
    close(fd_w);
}


void microbit_print_string(char *str) {
  send_msg(str);
}

void microbit_print_int(int i) {
  snprintf(buf, 50, "%d", i);
  send_msg(buf);
}

void microbit_write_pixel(int x, int y, int l) {
  if((l==0 && image[6*y+x]=='0') || (l!=0 && image[6*y+x]=='1')) return;
  if(l==0) image[6*y+x] = '0';
  else image[6*y+x] = '1';
  // if(l == 0) snprintf(buf, BUF_SIZE, "Turning off pixel %d %d", x, y);
  // else snprintf(buf, BUF_SIZE, "Turning on pixel %d %d at level %d", x, y, l);
  send_msg(image);
}

void microbit_print_image(char *str) {
  char tmp[30];
  for(int y = 0; y < 5; y++) {
    for(int x = 0; x < 5; x++) {
      tmp[6*y+x] = '0' + (str[y*5+x]*(-1)); 
      // printf("%d", str[y*5+x]);
    }
    tmp[6*(y+1)-1] = '\n';
    // printf("\n");
  }
  tmp[29] = '\0';
  strcpy(image, tmp);
  // printf("%s\n", image);
  send_msg(tmp);
}

void microbit_clear_screen() {}

int microbit_button_is_pressed(int b) {
  printf("Button is %d\n", b);
  return b;
}

void microbit_pin_mode(int p, int m) {
  if(m == 0) snprintf(buf, 50, "Setting PIN%d to INPUT", p);
  else snprintf(buf, 50, "Setting PIN%d to OUTPUT", p);
  send_msg(buf);
}

void microbit_digital_write(int p, int l) {
  snprintf(buf, 50, "Writing value %d to pin %d", l, p);
  send_msg(buf);
}

void microbit_analog_write(int p, int l) {
  printf("Writing value %d to pin %d", l, p);
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
  printf("serial write %c\n", c);
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
