#include <gtk-3.0/gtk/gtk.h>
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
void shm_init();

GtkWidget* screen[SCREEN_SIZE][SCREEN_SIZE];
int bval[2];
int pins_mode[NB_PIN];
int pins_niveau[NB_PIN]; 
int pins_vals[NB_PIN];

pid_t server_pid=0, mypid=0;
void *vshm1 = NULL, *vshm2=NULL;
struct shared_use_st *shm1, *shm2;
int shm2id, shm1id;

void modify_screen(int x, int y, int v){
    if(x>=SCREEN_SIZE || x<0 || y >= SCREEN_SIZE || y < 0) return;
    if(v) gtk_button_set_label(GTK_BUTTON(screen[x][y]), "o");
    else gtk_button_set_label(GTK_BUTTON(screen[x][y]), "");
}

void clear_screen(){
    for(int i=0; i<SCREEN_SIZE; i++){
        for(int j=0; j<SCREEN_SIZE; j++){
            modify_screen(i, j, 0);
        }
    }
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

void press_a(GtkWidget* widget, gpointer data){
    bval[0]= 1-bval[0]; int code=0;
    if(bval[0]) code=1;
    else code=2;
    send_msg(code);
}

void press_b(GtkWidget* widget, gpointer data){
    bval[1]= 1-bval[1]; int code=0;
    if(bval[1]) code=3;
    else code=4;
    send_msg(code);
}

void gui_destroy(GtkWidget* widget, gpointer data){
    shmdt(shm1);
	shmctl(shm1id, IPC_RMID, 0);
    shmdt(shm2);
	shmctl(shm2id, IPC_RMID, 0);
    gtk_main_quit();
    kill(server_pid, SIGKILL);
    kill(mypid, SIGKILL);
}


void* gui_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    int x=0, y=0, v=0, pin=0;
    while(1){           
        pthread_mutex_lock(&shm1->mute);
        // printf("client lisener lock\n");
        if(shm1->written==0){ // client lisener bloc, util server writer notify. 
            pthread_cond_wait(&shm1->cond_r, &shm1->mute);
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
            pin = code >> 17;
            break;
        default:
            break;
        }
        shm1->written = 0;
        pthread_cond_signal(&shm1->cond_w); // finish treatement, notify server writer;
        pthread_mutex_unlock(&shm1->mute);
    }//while
}
static gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data) {
	gtk_main_quit();
	return FALSE;
}

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
}


GtkWidget* create_grid(int col, int row){
    GtkWidget* grid, *button;
    grid = gtk_grid_new();
    button = gtk_button_new_with_label("Screen");
    gtk_widget_set_sensitive(button, FALSE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, col, 1);
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            button = gtk_button_new_with_label("");
            screen[i][j] = button;
            gtk_widget_set_sensitive(button, FALSE);
            gtk_grid_attach(GTK_GRID(grid), button, i, 1+j, 1, 1);
        }
    }
    return grid;
}

GtkWidget* create_UI(){
    GtkWidget *window, *button_box, *button;

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "simulator");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gui_destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 150);
    
    GtkWidget* box=gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget* grid=create_grid(SCREEN_SIZE, SCREEN_SIZE);
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 5);
    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 3);
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 3);

    button = gtk_button_new_with_label("A");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(press_a), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("B");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(press_b), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);

    return window;
}


int main(int argc, char ** argv){
    printf("Program start\n");
    if(argc < 2){ 
        printf("argc=%d", argc);
        exit(0);
    }
    mypid = getpid();
    server_pid = atoi(argv[1]);
    shm1id = atoi(argv[2]);
    shm2id = atoi(argv[3]);
    shm_init();
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