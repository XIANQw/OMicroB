#include "client.h"

GtkWidget* screen[SCREEN_SIZE][SCREEN_SIZE];
GtkWidget** leds;
int bval[2];
int64_t pins_mode;
int64_t pins_niveau; 
int *pins_vals;
struct shared_use_st *shm1, *shm2;
int shm2id, shm1id, envid;
Env shm_env;


inline void shm_init(){
    void * vshm2 = shmat(shm2id, 0, 0);
    if(vshm2 < 0){
        perror("shm2id shmat failed"); exit(1);
    }
    shm2=(struct shared_use_st*)vshm2;
    shm2->shmid = shm2id;
    shm2->written = 0;
    printf("shm2id=%d, memory attached at %p\n",shm2id, shm2);
    
    void * vshm1 = shmat(shm1id, 0, 0);
    if(vshm1 < 0){
        perror("vshm1 shmat failed"); exit(1);
    }
    shm1=(struct shared_use_st*)vshm1;
    shm1->shmid = shm1id;
    shm1->written = 0;
    printf("shm1id=%d, memory attached at %p\n",shm1id, shm1);

    void * venv = shmat(envid, 0, 0);
    if(vshm1 < 0){
        perror("venv shmat failed"); exit(1);
    }
    shm_env = (Env)venv;
    shm_env->shmid = envid;
    printf("envid=%d, memory attached at %p\n", envid, shm_env);
}

void print_Env(Env shm_env){
    printf("nbpins=(%d,%d) nbleds=%d\n", shm_env->nb_pins_row, shm_env->nb_pins_col, shm_env->nb_leds);
    for(int i=0; i<shm_env->nb_leds; i++){
        printf("led[%d]->(pin%d, pin%d)\n", i, shm_env->leds[i][0], shm_env->leds[i][1]);
    }
    printf("nb_buttons=%d\n", shm_env->nb_buttons);
    for(int i=0; i<shm_env->nb_buttons; i++){
        printf("button[%d]->(%s, pin%d)\n", i, shm_env->buttons[i].label, shm_env->buttons[i].pins);
    }
}

void print_pins_niveau(){
    printf("pins_niveau=[");
    for(int i=0; i<shm_env->nb_pins_row+shm_env->nb_pins_col; i++){
        printf("%d, ", READ_BIT(pins_niveau, i));
    }   
    printf("]\n");
}


void modify_screen(int x, int y, int v){
    if(x>=SCREEN_SIZE || x<0 || y >= SCREEN_SIZE || y < 0) return;
    if(v>0) gtk_button_set_label(GTK_BUTTON(screen[x][y]), "o");
    else gtk_button_set_label(GTK_BUTTON(screen[x][y]), "");
}

void clear_screen(){
    for(int i=0; i<SCREEN_SIZE; i++){
        for(int j=0; j<SCREEN_SIZE; j++){
            modify_screen(i, j, 0);
        }
    }
}

void flush_screen(){
    printf("leds_state=[");
    for(int i=0; i<shm_env->nb_leds; i++){
        int row=shm_env->leds[i][0], col=shm_env->leds[i][1] + shm_env->nb_pins_row;
        int x=i%5, y=i/5, v=READ_BIT(pins_niveau, row)-READ_BIT(pins_niveau, col);
        if(v>0) SET_BIT(leds_state, shm_env->nb_leds-i-1);
        else CLR_BIT(leds_state, shm_env->nb_leds-i-1);
        printf("%d ", READ_BIT(leds_state, shm_env->nb_leds-i-1));
        // if(v>0) printf("x=%d, y=%d\n", x, y);
        // modify_screen(x, y, v);
    }
    printf("]\n");
}

void send_msg(int code){
    pthread_mutex_lock(&shm2->mute);
    if(shm2->written==1){ // client writer wait, util server lisener notify
        pthread_cond_wait(&shm2->cond_w, &shm2->mute);
    }
    shm2->written=1;
    shm2->code=code;
    pthread_cond_signal(&shm2->cond_r); //notify server lisener
    pthread_mutex_unlock(&shm2->mute);
}

void init_env(){
    int nb_pin = shm_env->nb_pins_col+shm_env->nb_pins_row;
    pins_vals = (int *)malloc(sizeof(int)*nb_pin);
    memset(pins_vals, 0, nb_pin);
    int i;
    for(i=shm_env->nb_pins_row; i<nb_pin; i++) SET_BIT(pins_niveau, i);
    print_pins_niveau();
    leds=(GtkWidget **)malloc(sizeof(GtkWidget *) * (shm_env->nb_pins_row * shm_env->nb_pins_col));
    for(int i=0; i < shm_env->nb_leds; i++){
        int row = shm_env->leds[i][0], col = shm_env->leds[i][1];
        int index=row*shm_env->nb_pins_col + col;
        int x = i%SCREEN_SIZE, y=i/SCREEN_SIZE;
        leds[index] = screen[y][x];
    }
}

void* gui_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    int x=0, y=0, v=0, pin=0;
    while(1){           
        pthread_mutex_lock(&shm1->mute);
        // printf("client lisener lock\n");
        if(shm1->written==0){ // client lisener bloc, util server writer notify. 
            printf("cl:wait\n");    pthread_cond_wait(&shm1->cond_r, &shm1->mute);
        }
        
        // printf("c:%d\n", shm1->code);
        int code = shm1->code;
        char op = code >> 25;
        switch (op) {
        case 1:
            v = code & 0b1; 
            y = (code >> 1) & 0b111111111111;
            x = (code >> 13) & 0b111111111111;
            gdk_threads_enter();
            modify_screen(x,y,v);
            gdk_threads_leave();
            break;
        case 2:
            gdk_threads_enter();
            clear_screen();
            gdk_threads_leave();
            break;
        case 3:
            v = code & 0b1;
            pin = (code >> 17) & 0b11111111;
            printf("c:p=%d, v=%d\n", pin, v);
            if(v) SET_BIT(pins_niveau, pin);
            else CLR_BIT(pins_niveau, pin);
            flush_screen();
            print_pins_niveau();
            break;
        default:
            break;
        }
        shm1->written = 0;
        printf("cl notify sw\n");
        pthread_cond_signal(&shm1->cond_w); // finish treatement, notify server writer;
        pthread_mutex_unlock(&shm1->mute);
    }//while
}


int main(int argc, char ** argv){
    printf("Program start\n");
    if(argc < 5){ 
        printf("argc=%d", argc);
        exit(0);
    }
    mypid = getpid();
    server_pid = atoi(argv[1]);
    shm1id = atoi(argv[2]);
    shm2id = atoi(argv[3]);
    envid = atoi(argv[4]);
    shm_init();
    init_env();
    //---------gtk start

    if (!g_thread_supported()) g_thread_init(NULL);
    gdk_threads_init();
    gtk_init(&argc, &argv);

    GtkWidget *window = create_UI();
    gtk_widget_show_all(window);

    g_thread_create((GThreadFunc)gui_lisener,NULL,FALSE,NULL);

    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
    return 0;
}

