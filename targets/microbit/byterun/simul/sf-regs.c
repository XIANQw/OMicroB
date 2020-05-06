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

void *vshw = NULL, *vshr=NULL;
struct shared_use_st *shr = NULL, *shw=NULL;


void* fun_lisener(void * arg){
  while(1){
    pthread_mutex_lock(&shr->mute);
    printf("server lisener lock\n");  
    if(shr->written==0){ // server lisener bloc, until client writer notify
      printf("server lisener wait\n");
      pthread_cond_wait(&shr->cond_r, &shr->mute);
    }
    
    if(strlen(shr->text) > 0){
      printf("recieve %s\n", shr->text);
      pthread_mutex_lock(&mute);
      switch (shr->text[0]){
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
    } else if (strcmp("EOF",shr->text) == 0){
      printf("client quit, stop read\n"); break;   
    } else{
      printf("server: msg is empty\n");
    }
    shr->written=0;
    pthread_cond_signal(&shr->cond_w); //notift client writer
    printf("server lisener notify client writer\n");
    pthread_mutex_unlock(&shr->mute);
  }//while
  printf("---------------------notify：terminate read processus\n");  
  return NULL;
}


void simul_init(){
  if(flag_simul[0]) return;
  flag_simul[0]=1;
  

  int shrid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
  if(shrid < 0){
    perror("shrid shrget fail"); exit(1);
  }
  void * vshr = shmat(shrid, 0, 0);
  if(vshr < 0){
    perror("vshr shmat failed"); exit(1);
  }
  shr=(struct shared_use_st*)vshr;
  shr->shmid = shrid;
  shr->written = 0;
  pthread_mutex_init(&shr->mute, NULL);
  pthread_cond_init(&shr->cond_r, NULL);
  pthread_cond_init(&shr->cond_w, NULL);
  printf("shrid=%d, memory attached at %X\n",shrid, shr);

  int shwid = shmget((key_t)1230, sizeof(struct shared_use_st), 0666|IPC_CREAT);
  if(shwid < 0){
    perror("shwget fail"); exit(1);
  }
  void * vshw = shmat(shwid, 0, 0);
  if(vshw < 0){
    perror("shwid shmat failed"); exit(1);
  }
  shw=(struct shared_use_st*)vshw;
  shw->shmid = shwid;
  shw->written = 0;
  pthread_mutex_init(&shw->mute, NULL);
  pthread_cond_init(&shw->cond_r, NULL);
  pthread_cond_init(&shw->cond_w, NULL);
  printf("shwid=%d, memory attached at %X\n",shwid, shw);


  pid_t child = vfork();
  if(child < 0) exit(0);
  if(child == 0){
    printf("child\n");
    char pidstr[10], shwidstr[10], shridstr[10];
    snprintf(pidstr, 10, "%d", getppid());
    snprintf(shwidstr, 10, "%d", shwid);
    snprintf(shridstr, 10, "%d", shrid);
    printf("father=%d\n", getppid());
    char *const argv[] = {"gui", pidstr, shwidstr, shridstr, NULL};
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
  pthread_mutex_lock(&shw->mute);
  printf("server sender lock\n");
  if(shw->written==1) { // server writer bloc, util client lisener notify
    printf("server sender wait\n");
    pthread_cond_wait(&shw->cond_w, &shw->mute);
  }
  strcpy(shw->text, str);
  printf("send: len=%ld \n%s\n", strlen(str), str);
  shw->written = 1;
  pthread_cond_signal(&shw->cond_r); // notify client lisener
  printf("server sender notify client lisener\n");
  pthread_mutex_unlock(&shw->mute);
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
