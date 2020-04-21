/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
#include <signal.h>

#define SIG_A 42
#define SIG_NO_A 43
#define SIG_B 44
#define SIG_NO_B 45

void handler(int sig){
  switch (sig) {
  case SIG_A:
    button[0] = 1; break;
  case SIG_NO_A:
    button[0] = 0; break;
  case SIG_B:
    button[1] = 1; break;
  case SIG_NO_B:
    button[1] = 0; break;
  default:
    break;
  }
}

void simul_init(){
  if(flag_simul[0]) return;
  flag_simul[0]=1;
  int fd_w;
  char msg_w[BUF_SIZE];
  fd_w = open(SERVER_W,O_RDWR);
  snprintf(msg_w, BUF_SIZE, "pid%d", getpid());
  if(fd_w < 0){
      perror("open pipe_read fail");
      exit(0);
  }
  if (write(fd_w, msg_w, strlen(msg_w)+1) == -1){
    perror("server send msg fail");
  }else{
    printf("**************** server send :%s\n",msg_w);
  }
  signal(SIG_A, handler);
  signal(SIG_B, handler);
  signal(SIG_NO_A, handler);
  signal(SIG_NO_B, handler);
  sleep(1);
}

void send_msg(char * str){
    //pipe_write exist or not
    if(access(SERVER_W, 0) < 0){
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
  simul_init();
  send_msg(str);
}

void microbit_print_int(int i) {
  simul_init();
  snprintf(buf, 50, "%d", i);
  send_msg(buf);
}

void microbit_write_pixel(int x, int y, int l) {
  simul_init();
  if((l==0 && image[6*y+x]==' ') || (l!=0 && image[6*y+x]=='.')) return;
  if(l==0) image[6*y+x] = ' ';
  else image[6*y+x] = '.';
  send_msg(image);
}

void microbit_print_image(char *str) {
  simul_init();
  char tmp[30];
  for(int y = 0; y < 5; y++) {
    for(int x = 0; x < 5; x++) {
      if (str[y*5+x]==0) tmp[6*y+x] = ' ';
      else tmp[6*y+x] = '.';
    }
    tmp[6*(y+1)-1] = '\n';
  }
  tmp[29] = '\0';
  strcpy(image, tmp);
  send_msg(tmp);
}

void microbit_clear_screen() {}

int microbit_button_is_pressed(int b) {
  // printf("Button is %d\n", b);
  simul_init();
  if(b > 2 || b < 0){
    printf("button %d dosen't exist", b);
    return 0;
  }
  // printf("button%d is %d\n",b, button[b]);
  return button[b];
}

void microbit_pin_mode(int p, int m) {
  simul_init();
  if(m == 0) snprintf(buf, 50, "Setting PIN%d to INPUT", p);
  else snprintf(buf, 50, "Setting PIN%d to OUTPUT", p);
  send_msg(buf);
}

void microbit_digital_write(int p, int l) {
  simul_init();
  snprintf(buf, 50, "Writing value %d to pin %d", l, p);
  send_msg(buf);
}

void microbit_analog_write(int p, int l) {
  simul_init();
  printf("Writing value %d to pin %d", l, p);
}

int microbit_analog_read(int p) {
  simul_init();
  return 0;
}

int microbit_digital_read(int p) {
  simul_init();
  return 0;
}

void microbit_delay(int ms) {
  simul_init();
  usleep((useconds_t) ms * 1000);
}

int microbit_millis() {
  printf("millis()\n");
  return 0;
}

/******************************************************************************/

void microbit_serial_write(char c) {
  simul_init();
  printf("serial write %c\n", c);
}

char microbit_serial_read() {
  simul_init();
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
