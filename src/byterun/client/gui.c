#include <gtk/gtk.h>
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


GtkWidget* window; // main window
GtkWidget* button_box; // button_box
GtkWidget* box;
GtkWidget* table;
GtkWidget* button; // buttons

GtkWidget* screen[SCREEN_SIZE][SCREEN_SIZE];
int bval[2];
char msg_w[BUFSIZ],msg_r[BUFSIZ];
pid_t server_pid=0, mypid=0;
void *vshr = NULL, *vshw=NULL;
struct shared_use_st *shr, *shw;
int shwid, shrid;

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

void press_a(GtkWidget* widget, gpointer data){
    bval[0]= 1-bval[0];
    if(bval[0]) snprintf(msg_w, BUFSIZ, "%d", 1);
    else snprintf(msg_w, BUFSIZ, "%d", 2);
    printf("send instr %s\n", msg_w);
    
    pthread_mutex_lock(&shw->mute);
    printf("client sender_a lock\n");
    if(shw->written==1){ // client writer wait, util server lisener notify
        printf("client sender_a wait\n");
        pthread_cond_wait(&shw->cond_w, &shw->mute);
    }
    strcpy(shw->text, msg_w);
    shw->written=1;
    pthread_cond_signal(&shw->cond_r); //notify server lisener
    printf("client sender_a notify server lisener\n");
    pthread_mutex_unlock(&shw->mute);
}
void press_b(GtkWidget* widget, gpointer data){
    bval[1]= 1-bval[1];
    if(bval[1]) snprintf(msg_w, BUFSIZ, "%d", 3);
    else snprintf(msg_w, BUFSIZ, "%d", 4);
    printf("send instr %s\n", msg_w);
    
    pthread_mutex_lock(&shw->mute);
    printf("client sender_b lock\n");
    if(shw->written==1){
        printf("client sender_b wait\n");
        pthread_cond_wait(&shw->cond_w, &shw->mute);
    }
    strcpy(shw->text, msg_w);
    shw->written=1;
    pthread_cond_signal(&shw->cond_r);
    printf("client sender_a notify server lisener\n");
    pthread_mutex_unlock(&shw->mute);
}

void gui_destroy(GtkWidget* widget, gpointer data){
    gtk_main_quit();
    kill(server_pid, SIGKILL);
    kill(mypid, SIGKILL);
}

int gui(int argc, char **argv){
    // window > box > table + buttonbox > buttons

    gtk_init(&argc, &argv);
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "simulator");
    
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gui_destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 150);
    
    box = gtk_vbox_new(FALSE, 3);
    gtk_container_add(GTK_CONTAINER(window), box);

    table = gtk_table_new(5, 5, TRUE);
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            button = gtk_button_new_with_label("");
            screen[i][j] = button;
            gtk_widget_set_sensitive(button, FALSE);
            gtk_table_attach_defaults(GTK_TABLE(table), button, i, i+1, j, j+1);
            gtk_widget_show(button);
        }
    }
    gtk_widget_show(table);

    button_box = gtk_hbutton_box_new();
    gtk_box_set_spacing(GTK_BOX(button_box), 5);
    gtk_box_pack_start(GTK_BOX(box), table, FALSE, FALSE, 3);
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 3);

    button = gtk_button_new_with_label("A");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(press_a), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);
    gtk_widget_show(button);
    
    button = gtk_button_new_with_label("B");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(press_b), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    gtk_widget_show(button_box);
    gtk_widget_show(box);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}

void* gui_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    int x=0, y=0;
    while(1){           
        pthread_mutex_lock(&shr->mute);
        printf("client lisener lock\n");
        if(shr->written==0){ // client lisener bloc, util server writer notify. 
            printf("client lisener wait\n");
            pthread_cond_wait(&shr->cond_r, &shr->mute);
        }
        size_t len = strlen(shr->text);
        if(len==0){
            printf("msg is empty\n"); continue;
        }
        printf("msg=%s\n", shr->text);
        char op = shr->text[0];
        switch (op) {
        case '0':
            for(int i=0; i<SCREEN_SIZE; i++){
                for(int j=0; j<SCREEN_SIZE; j++){
                    int index = SCREEN_SIZE*i+j+1;
                    if(index>=len) break;
                    modify_screen(j, i, shr->text[index]-'0');
                }
            }
        case '1':
            x=shr->text[1]-'0';
            y=shr->text[2]-'0';
            // printf("x=%d, y=%d\n", x, y);
            modify_screen(x,y,shr->text[3]-'0');
            break;
        case '2':
            clear_screen(); break;
        default:
            printf("--\n");
        }
        shr->written = 0;
        pthread_cond_signal(&shr->cond_w); // finish treatement, notify server writer;
        printf("client lisener notify server writer\n");
        pthread_mutex_unlock(&shr->mute);
    }//while
}


int main(int argc, char ** argv){
    printf("Program start\n");
    if(argc < 2){ 
        printf("argc=%d", argc);
        exit(0);
    }
    mypid = getpid();
    server_pid = atoi(argv[1]);
    shrid = atoi(argv[2]);
    shwid = atoi(argv[3]);
    void * vshw = shmat(shwid, 0, 0);
    if(vshw < 0){
        perror("shwid shmat failed"); exit(1);
    }
    shw=(struct shared_use_st*)vshw;
    shw->shmid = shwid;
    shw->written = 0;
    printf("shwid=%d, memory attached at %X\n",shwid, shw);
    
    void * vshr = shmat(shrid, 0, 0);
    if(vshr < 0){
        perror("vshr shmat failed"); exit(1);
    }
    shr=(struct shared_use_st*)vshr;
    shr->shmid = shrid;
    shr->written = 0;
    printf("shrid=%d, memory attached at %X\n",shrid, shr);


    printf("pid=%d\n", server_pid);
    pthread_t pgui;
    pthread_create(&pgui, NULL, (void *)&gui, NULL);

    printf("client start communication\n");
    pthread_t plisener;
    pthread_create(&plisener, NULL, (void *)&gui_lisener, NULL);
    pthread_join(pgui, NULL);
    pthread_join(plisener, NULL);
    return 0;
}