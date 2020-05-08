#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "shared.h"
#include "simul.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#define NB_REG 255
#define NB_PORT 11
#define LOWER_PORT 0x0
#define HIGHER_PORT (LOWER_PORT + NB_PORT - 1)
#define LOWER_DDR (HIGHER_PORT + 1)
#define HIGHER_DDR (LOWER_DDR + NB_PORT - 1)
#define LOWER_PIN (HIGHER_DDR + 1)
#define HIGHER_PIN (LOWER_PIN + NB_PORT - 1)


#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4
#define PORTF 5
#define PORTG 6
#define PORTH 7
#define PORTI 8
#define PORTJ 9
#define PORTK 10
#define PORTL 11

#define DDRA 12
#define DDRB 13
#define DDRC 14
#define DDRD 15
#define DDRE 16
#define DDRF 17
#define DDRG 18
#define DDRH 19
#define DDRI 20
#define DDRJ 21
#define DDRK 22
#define DDRL 23

#define PINA 24
#define PINB 25
#define PINC 26
#define PIND 27
#define PINE 28
#define PINF 29
#define PING 30
#define PINH 31
#define PINI 32
#define PINJ 33
#define PINK 34
#define PINL 35


#define SPSR 36
#define SPDR 37

static unsigned char *regs;
static unsigned int *analogs;
static int *sync_counter;
static int sem_regs;
static int sem_sync;
static int sem_done;
static int proc_nb;
static int is_slow;

#define INPUT 0x0
#define OUTPUT 0x1

/* On AVR : PORT = output / PIN = intput / DDR = data direction */
/* and output = 1 / input = 0 */

void init_regs(int n, int slow){
  int i;
  regs = (unsigned char *) alloc_shm(NB_REG * sizeof(unsigned char));
  analogs = (unsigned int *) alloc_shm(16 * sizeof(unsigned int));
  sync_counter = (int *) alloc_shm(sizeof(int));
  *sync_counter = 0;
  is_slow = slow;
  sem_regs = create_sem(1);
  sem_sync = create_sem(1);
  sem_done = create_sem(0);
  proc_nb = n;
  for(i = 0 ; i < NB_REG ; i ++) regs[i] = 0x00;
}

void destroy_regs(void){
  destroy_sem(sem_regs);
  destroy_sem(sem_sync);
  destroy_sem(sem_done);
}

void dump_regs(void){
  int i, j;
  P(sem_regs);
  for(i = LOWER_PORT ; i <= HIGHER_PORT ; i ++){
    printf("%c: 0b", 'A' + i);
    for(j = 7 ; j >= 0 ; j --) printf("%d", (regs[i] & (1 << j)) != 0);
    printf("  = %3d  = 0x%02x\n", regs[i], regs[i]);
  }
  V(sem_regs);
}

static int int_of_hexchar(char c){
  if(c >= '0' && c <= '9') return c - '0';
  if(c >= 'A' && c <= 'F') return c - 'A' + 10;
  if(c >= 'a' && c <= 'f') return c - 'a' + 10;
  return -1;
}

static char hexchar_of_int(int n){
  n &= 0x0F;
  if(n < 10) return '0' + n;
  return 'A' + n - 10;
}

/******************************/

static void micro_sleep(unsigned long usec){
  struct timespec t;
  t.tv_sec  = usec / 1000000;
  t.tv_nsec = (1000 * usec) % 1000000000;
  nanosleep(&t, NULL);
}

static unsigned long elapsed_time(void){
  return (unsigned long) clock() * 1000000 / CLOCKS_PER_SEC;
}

static void may_sleep(){
  static unsigned long mem_elpt = 0;
  if(is_slow){
    unsigned long elpt = elapsed_time();
    if(elpt - mem_elpt > 100){
      unsigned long usec = (elpt - mem_elpt) * 4;
      micro_sleep(usec);
      mem_elpt = elapsed_time();
    }
  }
}

/******************************/

static void send_write(char cmnd, int port, unsigned char val){
  char buf[5];
  buf[0] = cmnd;
  buf[1] = port + 'A';
  buf[2] = hexchar_of_int((val >> 4) & 0x0F);
  buf[3] = hexchar_of_int(val & 0x0F);
  buf[4] = '\n';
  send_all_proc(buf, 5);
}

static void send_write_port(int port, unsigned char val){
  /* printf("sending write %d = %d \n", port, val); */
  send_write('W', port, val);
}

static void send_write_ddr(int ddr, unsigned char val){
  send_write('T', ddr - LOWER_DDR, val);
}


/* static void send_config_analogs(void){ */
  /* static unsigned int old_analog_nb = 0xFFFF; */
  /* unsigned int analog_nb = 0; */
  /* if((regs[ADCON0] & 1) != 0){ */
  /*   analog_nb = 0xF - (regs[ADCON1] & 0xF); */
  /*   if (analog_nb > 13) analog_nb = 13; */
  /* } */
  /* if (old_analog_nb != analog_nb) { */
  /*   char buf[3]; */
  /*   old_analog_nb = analog_nb; */
  /*   buf[0] = 'C'; */
  /*   buf[1] = hexchar_of_int(analog_nb); */
  /*   buf[2] = '\n'; */
  /*   send_all_proc(buf, 3); */
  /* } */
/* } */

static void send_set_analog(unsigned int chan, unsigned int val){
  fprintf(stderr,"SET ANALOG %d to %d",chan,val);
  char buf[6];
  buf[0] = 'Z';
  buf[1] = hexchar_of_int(chan);
  buf[2] = hexchar_of_int((val >> 8) & 0x0F);
  buf[3] = hexchar_of_int((val >> 4) & 0x0F);
  buf[4] = hexchar_of_int(val & 0x0F);
  buf[5] = '\n';
  send_all_proc(buf, 6);
}

/**************/

static void synchronize(){
  P(sem_sync);
  *sync_counter = proc_nb;
  V(sem_sync);
  send_all_proc("SYNC\n", 5);
  if(proc_nb != 0) P(sem_done);
  may_sleep();
}

static int is_reg_need_synchro(uint8_t reg){
  return
    (reg >= LOWER_PORT && reg <= HIGHER_PORT);
}

static void write_register_gen(int reg, uint8_t new_val){
  uint8_t old_val = regs[reg];
  if(reg >= LOWER_PORT && reg <= HIGHER_PORT){
    if(old_val != new_val){
      int ddr = reg - LOWER_PORT + LOWER_DDR;
      uint8_t ddr_val = regs[ddr];
      if(ddr_val == 0x00){
        char port_c = 'A' + reg - LOWER_PORT;
        fprintf(stderr, "Warning: the avr writes PORT%c when DDR%c=0xFF\n",
                port_c, port_c);
      }else{
	regs[reg] = new_val;
	send_write_port(reg, new_val);
      }
    }
  }
  else if(reg >= LOWER_DDR && reg <= HIGHER_DDR){
    if(old_val != new_val){
      send_write_ddr(reg, new_val);
      regs[reg] = new_val;
    }
  }
  else if(reg == SPDR){
    regs[reg] = new_val;
    send_write_port('G'-'A',new_val);
  }
  else{
    regs[reg] = new_val;
  }
  may_sleep();
}

void write_register(uint8_t reg, uint8_t new_val){
  /* printf("avr_write_register(%d, %d)\n", (int) reg, (int) new_val); */
  init_simulator();
  P(sem_regs);
  write_register_gen(reg, new_val);
  V(sem_regs);
}

uint8_t read_register(uint8_t reg){
  /* printf("avr_read_register(%d)\n", (int) reg); */
  uint8_t val;
  init_simulator();
  if (is_reg_need_synchro(reg)) synchronize();
  P(sem_regs);
  val = regs[reg];
  V(sem_regs);
  may_sleep();
  return val;
}

bool read_bit(uint8_t reg, uint8_t bit){
    /* printf("avr_read_bit(%d, %d)\n", (int) reg, (int) bit); */
  /* Dirty hack  */
  if (reg == SPSR){
      return 1;
  }
  uint8_t mask = 1 << bit;
  uint8_t val;
  init_simulator();
  if (is_reg_need_synchro(reg)) synchronize();
  P(sem_regs);
  /* the simulator doesnt change PINs, only PORTs ...  */
  val = (regs[reg-LOWER_PIN] & mask) != 0;
  V(sem_regs);
  may_sleep();
  return val;
}

void clear_bit(uint8_t reg, uint8_t bit){
  /* printf("avr_clear_bit(%d, %d)\n", (int) reg, (int) bit);  */
  init_simulator();
  P(sem_regs);
  {
    uint8_t old_val = regs[reg];
    uint8_t mask = 1 << bit;
    uint8_t new_val = old_val & ~mask;
    if(reg >= LOWER_PORT && reg <= HIGHER_PORT){
      if(old_val != new_val){
        int ddr = reg - LOWER_PORT + LOWER_DDR;
        uint8_t ddr_val = regs[ddr];
        if(!(ddr_val & mask)){
          char port_c = 'A' + reg - LOWER_PORT;
          fprintf(stderr,
                  "Warning: the avr clears PORT%c.R%c%d when DDR%c=0x%02X\n",
                  port_c, port_c, bit, port_c, ddr_val);
        }else{
          regs[reg] = new_val;
          send_write_port(reg, new_val);
        }
      }
    } else if(reg >= LOWER_DDR && reg <= HIGHER_DDR){
      if(old_val != new_val){
        regs[reg] = new_val;
        send_write_ddr(reg, new_val);
      }
    }else{
      regs[reg] = new_val;
    }
  }
  V(sem_regs);
  may_sleep();
}

void set_bit(uint8_t reg, uint8_t bit){
  /* printf("avr_set_bit(%d, %d)\n", (int) reg, (int) bit);  */
  init_simulator();
  P(sem_regs);
  uint8_t old_val = regs[reg];
  uint8_t mask = 1 << bit;
  uint8_t new_val = old_val | mask;
  if(reg >= LOWER_PORT && reg <= HIGHER_PORT){
    if (old_val != new_val){
      /* int ddr = reg - LOWER_PORT + LOWER_DDR; */
      /* uint8_t ddr_val = regs[ddr]; */
      /* Forced to remove this as with INPUT_PULLUP, the PORT is also set at the beginning : */
      /* if(!(ddr_val & mask)){ */
	/* char port_c = 'B' + reg - LOWER_PORT; */
	/* fprintf(stderr, "Warning: the avr sets PORT%c.R%c%d when DDR%c=0x%02X\n", */
		/* port_c, port_c, bit, port_c, ddr_val); */
      /* } */
      /* else { */
	regs[reg] = new_val;
	send_write_port(reg,new_val);
      /* } */
    }
  }
  else if (reg >= LOWER_DDR && reg <= HIGHER_DDR){
    if(old_val != new_val){
      regs[reg] = new_val;
      send_write_ddr(reg,new_val);
    }
  }
  else if (reg >= LOWER_PIN && reg <= HIGHER_PIN){
    char port_c = 'A' + reg - LOWER_PIN;
    fprintf(stderr, "Warning : PIN%c is only a read register, it shouldn't be written\n",
		port_c);
  }
  else{
  regs[reg] = new_val;
  /* send_write_port(reg,new_val); */ // don't send internal modifications
  }
  /* printf("set bit %d on reg %d \n", bit, reg); */
  V(sem_regs);
  may_sleep();
}

/***************************************************/

void delay(int ms) {
  printf("delay(%d)\n", ms);
  usleep((useconds_t) ms * 1000);
}

int millis() {
  printf("millis\n");
  return 0;
}

/******************************/

int pic_tris_of_port(int port_or_bit){
  /* unsigned int reg = Long_val(port_or_bit) & 0x7F; */
  /* unsigned int mask = Long_val(port_or_bit) >> 7; */
  /* may_sleep(); */
  /* if(reg >= LOWER_PORT && reg <= HIGHER_PORT){ */
  /*   return Val_long((mask << 7) | (reg + LOWER_TRIS)); */
  /* }else if(reg >= LOWER_LAT && reg <= HIGHER_LAT){ */
  /*   return Val_long((mask << 7) | (reg - LOWER_LAT + LOWER_TRIS)); */
  /* }else{ */
  /*   fprintf(stderr, "Error: tris_of_port(0x%0x): not a port\n", reg); */
  /*   return Val_long(LOWER_TRIS); */
  /* } */
  return 1;
}

/******************************/

static void out_write_port(int port, unsigned char new_val){
  P(sem_regs);
  {
    int ddr = port - LOWER_PORT + LOWER_DDR;
    int ddr_val = regs[ddr];
    int old_val = regs[port];
    if((new_val & ~ddr_val) != 0xFF){
      char port_c = 'A' + port - LOWER_PORT;
      fprintf(stderr,
              "Warning: an outside component writes PORT%c=0x%02X when TRIS%c=0x%02X\n",
              port_c, new_val, port_c, ddr_val);
    }
    if(new_val != old_val){
      regs[port] = new_val;
      send_write_port(port, new_val);
    }
  }
  V(sem_regs);
  may_sleep();
}

static void out_clear_port_bit(int port, int bit){
  P(sem_regs);
  {
    int mask = 1 << bit;
    int ddr = port - LOWER_PORT + LOWER_DDR;
    int ddr_val = regs[ddr];
    int old_val = regs[port];
    int new_val = old_val & ~mask;
    if(ddr_val & mask){
      char port_c = 'A' + port - LOWER_PORT;
      fprintf(stderr,
        "Warning: an outside component clears PORT%c.R%c%d when TRIS%c=0x%02X\n",
              port_c, port_c, bit, port_c, ddr_val);
    }
    if(old_val != new_val){
      regs[port] = new_val;
      send_write_port(port, new_val);
    }
  }
  V(sem_regs);
  may_sleep();
}

static void out_set_port_bit(int port, int bit){
  P(sem_regs);
  {
    int mask = 1 << bit;
    int ddr = port - LOWER_PORT + LOWER_DDR;
    int ddr_val = regs[ddr];
    int old_val = regs[port];
    int new_val = old_val | mask;
    if(ddr_val & mask){
      char port_c = 'A' + port - LOWER_PORT;
      fprintf(stderr,
        "Warning: an outside component sets PORT%c.R%c%d when DDR%c=0x%02X\n",
              port_c, port_c, bit, port_c, ddr_val);
    }
    if(old_val != new_val){
      regs[port] = new_val;
      send_write_port(port, new_val);
    }
  }
  V(sem_regs);
  may_sleep();
}

static void out_set_analog(unsigned int chan, unsigned int val){
  P(sem_regs);
  {
    if(val != analogs[chan]){
      analogs[chan] = val;
      fprintf(stderr,"I put %d in analog %d", val, chan);
      send_set_analog(chan, val);
    }
  }
  V(sem_regs);
  may_sleep();
}

/**************/

static void invalid_instr(char *instr){
  fprintf(stderr, "Invalid instruction: '%s'\n", instr);
}

void exec_instr(char *instr, int size){
  if(size < 3){
    invalid_instr(instr);
  }else if((size == 4) &&
           (instr[0] == 'D' || instr[0] == 'd') &&
           (instr[1] == 'O' || instr[1] == 'o') &&
           (instr[2] == 'N' || instr[2] == 'n') &&
           (instr[3] == 'E' || instr[3] == 'e')){
    P(sem_sync);
    *sync_counter = *sync_counter - 1;
    if(*sync_counter == 0) V(sem_done);
    V(sem_sync);
  }else if(size == 5 && instr[0] == 'Z'){
    int chan, h2, h1, h0, val;
    chan = int_of_hexchar(instr[1]);
    if(chan == -1 || chan > 14){
      invalid_instr(instr);
      return;
    }
    h2 = int_of_hexchar(instr[2]);
    h1 = int_of_hexchar(instr[3]);
    h0 = int_of_hexchar(instr[4]);
    val = 16 * 16 * h2 + 16 * h1 + h0;
    if(h2 == -1 || h1 == -1 || h0 == -1 || val >= (1 << 10)){
      invalid_instr(instr);
      return;
    }
    out_set_analog(chan, val);
  }else{
    int port;
    if(instr[1] >= 'A' && instr[1] <= ('A' + HIGHER_PORT - LOWER_PORT)){
      port = instr[1] - 'A' + LOWER_PORT;
    }else if(instr[1] >= 'a' && instr[1] <= ('a' + HIGHER_PORT - LOWER_PORT)){
      port = instr[1] - 'a' + LOWER_PORT;
    }else{
      invalid_instr(instr);
      return;
    }
    switch(instr[0]){
    case 'W':
    case 'w':
      if(size != 4) invalid_instr(instr);
      else{
        int h1 = int_of_hexchar(instr[2]);
        int h0 = int_of_hexchar(instr[3]);
        if (h1 == -1 || h0 == -1) {
          invalid_instr(instr);
          return;
        }
        out_write_port(port, 16 * h1 + h0);
      }
    break;
    case 'C':
    case 'c':
      if(size != 3 || instr[2] < '0' || instr[2] > '7') invalid_instr(instr);
      else {
        out_clear_port_bit(port, instr[2] - '0');
      }
    break;
    case 'S':
    case 's':
      if(size != 3 || instr[2] < '0' || instr[2] > '7') invalid_instr(instr);
      else {
        out_set_port_bit(port, instr[2] - '0');
      }
    break;
    default:
      invalid_instr(instr);
    }
  }
}
int r = 0;
int avr_random(int max){
  r = (r*109+89)%max;
  return r;
}

void avr_adc_init(){
  printf("adc init\n");
  init_simulator();
}

#include <unistd.h>

uint16_t avr_analog_read(uint8_t ch){
  /* printf("analog read (%d)\n", ch); */
  /* out_set_analog(ch,0b11111111); */
  /* usleep(50000); */
  /* out_set_analog(ch,analogs[ch]); */
  return analogs[ch];
}

/******************************************************************************/

void avr_serial_init(){
  init_simulator();
  printf("serial init\n");
  set_bit(DDRD,3);
}

char avr_serial_read(){
  printf("serial read\n");
  set_bit(PORTD,2);
  usleep(10000);
  clear_bit(PORTD,2);
  return '0';
}

void avr_serial_write(char c){
  printf("serial write(%c)\n",c);
  set_bit(PORTD,3);
  usleep(10000);
  clear_bit(PORTD,3);
}
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
