/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
#include <signal.h>
#include <pthread.h>
#include <sys/shm.h>
#include "../client/protocol.h"
#include "shared.h"
#include "sf-regs.h"
#include "chs_tab.h"

#define BUF_SIZE 50
#define NB_PIN 16

pthread_mutex_t mute;
pthread_t lisener;

int *pins_val;
int *pins_niveau;
int *pins_mode;

void *vshm1 = NULL, *vshm2 = NULL;
struct shared_use_st *shm2 = NULL, *shm1 = NULL;
Env shm_env;

void *fun_lisener(void *arg)
{
  while (1)
  {
    pthread_mutex_lock(&shm2->mute);
    // printf("server lisener lock\n");
    if (shm2->written == 0)
    { // server lisener bloc, until client writer notify
      pthread_cond_wait(&shm2->cond_r, &shm2->mute);
    }
    pthread_mutex_lock(&mute);
    switch (shm2->code)
    {
    case 1:
      button[0] = 1;
      break;
    case 2:
      button[0] = 0;
      break;
    case 3:
      button[1] = 1;
      break;
    case 4:
      button[1] = 0;
      break;
    default:
      break;
    }
    pthread_mutex_unlock(&mute);

    shm2->written = 0;
    pthread_cond_signal(&shm2->cond_w); //notift client writer
    pthread_mutex_unlock(&shm2->mute);
  }
  return NULL;
}

void init_env()
{
  int nb_pin = shm_env->nb_pins_col + shm_env->nb_pins_row;
  pins_mode = (int *)malloc(sizeof(int) * nb_pin);
  pins_niveau = (int *)malloc(sizeof(int) * nb_pin);
  pins_val = (int *)malloc(sizeof(int) * nb_pin);
  memset(pins_mode, 0, nb_pin);
  int i;
  for (i = 0; i < shm_env->nb_pins_row; i++)
    pins_niveau[i] = 0;
  for (; i < shm_env->nb_pins_col; i++)
    pins_niveau[i] = 1;
  memset(pins_val, 0, nb_pin);
}

void simul_init()
{
  if (flag_simul[0])
    return;
  flag_simul[0] = 1;

  shm1 = create_shm(1234);
  int shm1id = shm1->shmid;
  shm2 = create_shm(1230);
  int shm2id = shm2->shmid;
  shm_env = create_env(1238);
  int envid = shm_env->shmid;
  char env_id_str[10];
  snprintf(env_id_str, 10, "%d", envid);

  pid_t child = vfork();
  if (child < 0)
    exit(0);
  if (child == 0)
  {
    char *const argv[] = {"montage", env_id_str, NULL};
    printf("child1, envid=%d\n", envid);
    int res = execvp("/tmp/montage", argv);
    printf("montage=%d\n", res);
  }
  sleep(1);

  init_env();
  child = vfork();
  if (child < 0)
    exit(0);
  if (child == 0)
  {
    printf("child2\n");
    char pidstr[10], shm1idstr[10], shm2idstr[10];
    snprintf(pidstr, 10, "%d", getppid());
    snprintf(shm1idstr, 10, "%d", shm1id);
    snprintf(shm2idstr, 10, "%d", shm2id);
    printf("father=%d\n", getppid());
    char *const argv[] = {"gui", pidstr, shm1idstr, shm2idstr, env_id_str, NULL};
    int res = execvp("/tmp/client", argv);
    printf("client=%d\n", res);
  }
  else
  {
    sleep(1);
    if (pthread_create(&lisener, NULL, fun_lisener, NULL) == -1)
    {
      perror("create lisener fail");
      exit(1);
    }
    pthread_detach(lisener);
    pthread_mutex_init(&mute, NULL);
  }
}

void send_msg(int code)
{
  //pipe_write exist or not
  pthread_mutex_lock(&shm1->mute);
  if (shm1->written == 1)
  { // server writer bloc, util client lisener notify
    printf("s stop\n");
    pthread_cond_wait(&shm1->cond_w, &shm1->mute);
  }
  shm1->code = code;
  shm1->written = 1;
  pthread_cond_signal(&shm1->cond_r); // notify client lisener
  pthread_mutex_unlock(&shm1->mute);
}

void print_char(char AscC)
{
  getCharTab(AscC, chs_tab);
  for (int y = 0; y < 5; y++)
  {
    for (int x = 0; x < 5; x++)
    {
      if ((ch_tab[y] & (1 << x)) == (1 << x))
        microbit_write_pixel(x, y, 1);
      else
        microbit_write_pixel(x, y, 0);
    }
  }
}

void microbit_print_string(char *str)
{
  simul_init();
  while (*str != '\0')
  {
    print_char(*str++);
    delay(500);
  }
}

void microbit_print_int(int i)
{
  simul_init();
  snprintf(msg_w, BUF_SIZE, "%d", i);
  microbit_print_string(msg_w);
}

//   inst      x       y    v
// 31----25 24---13 12---1  0
void microbit_write_pixel(int x, int y, int l)
{
  simul_init();
  int ledid = 5 * y + x, v;
  int row_pin = shm_env->leds[ledid][0], col_pin = shm_env->leds[ledid][1] + shm_env->nb_pins_row;
  // printf("x=%d, y=%d, v=%d\n", x, y, l);
  if (l > 0)
  {
    microbit_digital_write(row_pin, 1);
    microbit_digital_write(col_pin, 0);
  }
  else
  {
    microbit_digital_write(row_pin, 0);
    microbit_digital_write(col_pin, 1);
  }
  if (pixels[5 * y + x] == l)
    return;
  if (l == 0)
  {
    pixels[5 * y + x] = 0;
    v = 0;
  }
  else
  {
    pixels[5 * y + x] = l;
    v = 1;
  }
  int code = SET_PIXEL(x, y, v);
  send_msg(code);
}

void microbit_print_image(char *str)
{
  simul_init();
  for (int y = 0; y < 5; y++)
  {
    for (int x = 0; x < 5; x++)
    {
      if (str[y * 5 + x] == 0)
      {
        microbit_write_pixel(x, y, 0);
      }
      else
      {
        microbit_write_pixel(x, y, 1);
      }
    }
  }
}

void microbit_clear_screen()
{
  simul_init();
  int code = CLEAR_SCREEN;
  send_msg(code);
}

void microbit_show()
{
  printf("show\n");
  send_msg(SHOW);
}

int microbit_button_is_pressed(int b)
{
  // printf("Button is %d\n", b);
  simul_init();
  if (b > 2 || b < 0)
  {
    printf("button %d dosen't exist", b);
    return 0;
  }
  pthread_mutex_lock(&mute);
  int res = button[b];
  pthread_mutex_unlock(&mute);
  return res;
}

void microbit_pin_mode(int p, int m)
{
  simul_init();
  pins_mode[p] = m;
  int v = m == 0 ? 0 : 1;
  int code = PIN_MODE(p, m);
  send_msg(code);
}

//   inst    PIN     v
// 31----25 24---17 16--0
void microbit_digital_write(int p, int l)
{
  simul_init();
  pins_niveau[p] = l;
  int v = l == 0 ? 0 : 1;
  int code = DIGITAL_WRITE(p, v);
  send_msg(code);
  printf("(s->c)p=%d, v=%d\n", p, v);
}

int microbit_digital_read(int p)
{
  simul_init();
  return pins_val[p];
}

//   inst    PIN     v
// 31----25 24---17 16--0
void microbit_analog_write(int p, int l)
{
  simul_init();
  int code = ANALOG_WRITE(p, l);
  pins_val[p] = l;
  send_msg(code);
}

int microbit_analog_read(int p)
{
  simul_init();
  return pins_val[p];
}

void microbit_delay(int ms)
{
  simul_init();
  usleep((useconds_t)ms * 1000);
}

int microbit_millis()
{
  printf("millis()\n");
  return 0;
}

/******************************************************************************/

void getCharTab(char c, char *chs_tab)
{
  memset(ch_tab, 0, 5 * sizeof(char));
  int offset;
  offset = (int)c * 5; /*compute the offset by ascii code*/
  //printf("%d",offset);
  for (int i = 0; i < 5; i++)
  {
    ch_tab[i] = chs_tab[offset + i];
  }
}

/*******************************************************************************/

char buffer[BUF_SIZE];
int buf_ptr, read_ptr;

void microbit_serial_write(char c)
{
  simul_init();
  if (buf_ptr > BUF_SIZE)
  {
    perror("buffer pointer > buffer size");
    exit(0);
  }
  buffer[buf_ptr++] = c;
}

char microbit_serial_read()
{
  simul_init();
  if (read_ptr == buf_ptr)
  {
    perror("head pointer > buffer ptr");
    exit(0);
  }
  char res = buffer[read_ptr++];
  if (read_ptr == buf_ptr)
  {
    buf_ptr = 0;
    read_ptr = 0;
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

void microbit_radio_send(char *s)
{
  printf("Sending %s by radio\n", s);
}

const char *microbit_radio_recv()
{
  return "a";
}

/*****************************************************************************/

void microbit_i2c_init() {}

void microbit_i2c_write(int a, const char *buf, int l) {}

int microbit_i2c_read(int a, char *buf)
{
  return 0;
}

/*****************************************************************************/

void microbit_spi_init() {}
char microbit_spi_transmit(char c)
{
  printf("Emited %d\n", c);
  return c;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/