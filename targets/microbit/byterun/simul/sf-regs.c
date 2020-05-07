/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#include <signal.h>
#include <pthread.h>
#include <sys/shm.h>
#include "../client/protocol.h"
#include "shared.h"
#include "sf-regs.h"

#define BUF_SIZE 50

pthread_mutex_t mute;
pthread_t lisener;

void *vshm1 = NULL, *vshm2=NULL;
struct shared_use_st *shm2 = NULL, *shm1=NULL;


void* fun_lisener(void * arg){
  while(1){
    pthread_mutex_lock(&shm2->mute);
    printf("server lisener lock\n");  
    if(shm2->written==0){ // server lisener bloc, until client writer notify
      printf("server lisener wait\n");
      pthread_cond_wait(&shm2->cond_r, &shm2->mute);
    }
    
    if(strlen(shm2->text) > 0){
      printf("recieve %s\n", shm2->text);
      pthread_mutex_lock(&mute);
      switch (shm2->text[0]){
      case '1':
        button[0]=1; break;
      case '2':
        button[0]=0; break;
      case '3':
        button[1]=1; break;
      case '4':
        button[1]=0; break;
      default:
        break;
      }
      pthread_mutex_unlock(&mute);
    } else if (strcmp("EOF",shm2->text) == 0){
      printf("client quit, stop read\n"); break;   
    } else{
      printf("server: msg is empty\n");
    }
    shm2->written=0;
    pthread_cond_signal(&shm2->cond_w); //notift client writer
    printf("server lisener notify client writer\n");
    pthread_mutex_unlock(&shm2->mute);
  }//while
  printf("---------------------notify：terminate read processus\n");  
  return NULL;
}


void simul_init(){
  if(flag_simul[0]) return;
  flag_simul[0]=1;
  

  shm1 = create_shm(1234);
  int shm1id=shm1->shmid;
  shm2 = create_shm(1230);
  int shm2id=shm2->shmid;

  pid_t child = vfork();
  if(child < 0) exit(0);
  if(child == 0){
    printf("child\n");
    char pidstr[10], shm1idstr[10], shm2idstr[10];
    snprintf(pidstr, 10, "%d", getppid());
    snprintf(shm1idstr, 10, "%d", shm1id);
    snprintf(shm2idstr, 10, "%d", shm2id);
    printf("father=%d\n", getppid());
    char *const argv[] = {"gui", pidstr, shm1idstr, shm2idstr, NULL};
    int res = execvp("/tmp/gui",argv);
    printf("res=%d\n", res);
  }else{
    sleep(1);
    if (pthread_create(&lisener, NULL, fun_lisener, NULL)==-1){
      perror("create lisener fail"); exit(1);
    }
    pthread_detach(lisener);
    pthread_mutex_init(&mute, NULL);
  }
}

void send_msg(char * str){
  //pipe_write exist or not
  pthread_mutex_lock(&shm1->mute);
  printf("server sender lock\n");
  if(shm1->written==1) { // server writer bloc, util client lisener notify
    printf("server sender wait\n");
    pthread_cond_wait(&shm1->cond_w, &shm1->mute);
  }
  strcpy(shm1->text, str);
  printf("send: len=%ld \n%s\n", strlen(str), str);
  shm1->written = 1;
  pthread_cond_signal(&shm1->cond_r); // notify client lisener
  printf("server sender notify client lisener\n");
  pthread_mutex_unlock(&shm1->mute);
}


void microbit_print_string(char *str) {
  simul_init();
  send_msg(str);
}

void microbit_print_int(int i) {
  simul_init();
  snprintf(msg_w, BUF_SIZE, "%d", i);
  send_msg(msg_w);
}

void microbit_write_pixel(int x, int y, int l) {
  simul_init();
  if((l==0 && image[6*y+x]==' ') || (l!=0 && image[6*y+x]=='.')) return;
  if(l==0){
    image[6*y+x] = ' ';
    snprintf(msg_w, BUF_SIZE, "%d%d%d%d", SET_PIXEL, x, y, 0);
  } else{ 
    image[6*y+x] = '.';
    snprintf(msg_w, BUF_SIZE, "%d%d%d%d", SET_PIXEL, x, y, 1);
  }
  send_msg(msg_w);
}

void microbit_print_image(char *str) {
  simul_init();
  char tmp[30];
  for(int y = 0; y < 5; y++) {
    for(int x = 0; x < 5; x++) {
      if (str[y*5+x]==0) tmp[5*y+x] = '0';
      else tmp[5*y+x] = '1';
    }
  }
  tmp[25] = '\0';
  strcpy(image, tmp);
  snprintf(msg_w, BUF_SIZE, "%d%s", PRINT_IMAGE, tmp);
  send_msg(msg_w);
}

void microbit_clear_screen() {
  simul_init();
  snprintf(msg_w, BUF_SIZE, "%d", CLEAR_SCREEN);
}

int microbit_button_is_pressed(int b) {
  // printf("Button is %d\n", b);
  simul_init();
  if(b > 2 || b < 0){
    printf("button %d dosen't exist", b);
    return 0;
  }
  pthread_mutex_lock(&mute);
  int res = button[b];
  pthread_mutex_unlock(&mute);
  return res;
}

void microbit_pin_mode(int p, int m) {
  simul_init();
  pins_mode[p] = m;
  if(m == 0) snprintf(msg_w, BUF_SIZE, "Setting PIN%d to INPUT", p);
  else snprintf(msg_w, BUF_SIZE, "Setting PIN%d to OUTPUT", p);
  send_msg(msg_w);
}

void microbit_digital_write(int p, int l) {
  simul_init();
  pins_val[p] = l;
  snprintf(msg_w, BUF_SIZE, "Writing value %d to pin %d", l, p);
  send_msg(msg_w);
}

void microbit_analog_write(int p, int l) {
  simul_init();
  printf("Writing value %d to pin %d", l, p);
  pins_val[p] = l;
}

int microbit_analog_read(int p) {
  simul_init();
  return pins_val[p];
}

int microbit_digital_read(int p) {
  simul_init();
  return pins_val[p];
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
  if(buf_ptr>BUF_SIZE) {
    perror("buffer pointer > buffer size");
    exit(0);
  }
  // printf("serial write %c\n", c);
  msg_w[buf_ptr++] = c;
}

char microbit_serial_read() {
  simul_init();
  if(ptr_head == buf_ptr){
    perror("head pointer > buffer ptr");
    exit(0);
  }
  char res = msg_w[ptr_head++];
  if(ptr_head == buf_ptr){
    buf_ptr=0; ptr_head=0;
  }
  return res;
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
