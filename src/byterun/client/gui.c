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
#include "shmdata.h"

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
    while(shw->written==1){}
    strcpy(shw->text, msg_w);
    shw->written=1;
}
void press_b(GtkWidget* widget, gpointer data){
    bval[1]= 1-bval[1];
    if(bval[1]) snprintf(msg_w, BUFSIZ, "%d", 3);
    else snprintf(msg_w, BUFSIZ, "%d", 4);
    printf("send instr %s\n", msg_w);
    while(shw->written==1) {}
    strcpy(shw->text, msg_w);
    shw->written=1;
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
        if(shr->written == 0) continue;
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
    }//while
}


int main(int argc, char ** argv){
    printf("Program start\n");
    if(argc < 2){ 
        printf("argc=%d", argc);
        exit(0);
    }
    server_pid = atoi(argv[1]);
    printf("pid=%d\n", server_pid);
    pthread_t pgui;
    pthread_create(&pgui, NULL, (void *)&gui, NULL);
    mypid = getpid();
    // shm of server_write 1234
    shr = create_shm(1234);
    // shm of server_read 1230
    shw = create_shm(1230);
    printf("client start communication\n");
    pthread_t plisener;
    pthread_create(&plisener, NULL, (void *)&gui_lisener, NULL);
    pthread_join(pgui, NULL);
    pthread_join(plisener, NULL);
    return 0;
}