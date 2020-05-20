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

void send_msg(char* str){
    pthread_mutex_lock(&shm2->mute);
    // printf("client sender_a lock\n");
    if(shm2->written==1){ // client writer wait, util server lisener notify
        // printf("client sender_a wait\n");
        pthread_cond_wait(&shm2->cond_w, &shm2->mute);
    }
    printf("c->s:%s\n", str);
    strcpy(shm2->text, str);
    shm2->written=1;
    pthread_cond_signal(&shm2->cond_r); //notify server lisener
    // printf("client sender_a notify server lisener\n");
    pthread_mutex_unlock(&shm2->mute);
}

void press_a(GtkWidget* widget, gpointer data){
    bval[0]= 1-bval[0];
    if(bval[0]) snprintf(msg_w, BUFSIZ, "%d", 1);
    else snprintf(msg_w, BUFSIZ, "%d", 2);
    send_msg(msg_w);
}

void press_b(GtkWidget* widget, gpointer data){
    bval[1]= 1-bval[1];
    if(bval[1]) snprintf(msg_w, BUFSIZ, "%d", 3);
    else snprintf(msg_w, BUFSIZ, "%d", 4);
    send_msg(msg_w);
}

void gui_destroy(GtkWidget* widget, gpointer data){
    gtk_main_quit();
    kill(server_pid, SIGKILL);
    kill(mypid, SIGKILL);
}


void* gui_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    int x=0, y=0;
    while(1){           
        pthread_mutex_lock(&shm1->mute);
        // printf("client lisener lock\n");
        if(shm1->written==0){ // client lisener bloc, util server writer notify. 
            // printf("client lisener wait\n");
            pthread_cond_wait(&shm1->cond_r, &shm1->mute);
        }
        size_t len = strlen(shm1->text);
        if(len==0){
            printf("c:empty\n"); continue;
        }
        printf("c:%s\n", shm1->text);
          
        char op = shm1->text[0];
        switch (op) {
        case '0':
            gdk_threads_enter();
            for(int i=0; i<SCREEN_SIZE; i++){
                for(int j=0; j<SCREEN_SIZE; j++){
                    int index = SCREEN_SIZE*i+j+1;
                    if(index>=len) {
                        gdk_threads_leave();
                        break;
                    }
                    modify_screen(j, i, shm1->text[index]-'0');
                }
            }
            gdk_threads_leave();
            break;
        case '1':
            x=shm1->text[1]-'0';
            y=shm1->text[2]-'0';
            // printf("x=%d, y=%d\n", x, y);
            gdk_threads_enter();
            modify_screen(x,y,shm1->text[3]-'0');
            gdk_threads_leave();
            break;
        case '2':
            gdk_threads_enter();
            clear_screen();
            gdk_threads_leave();
            break;
        default:
            printf("--\n");
            break;
        }
        shm1->written = 0;
        pthread_cond_signal(&shm1->cond_w); // finish treatement, notify server writer;
        // printf("client lisener notify server writer\n");
        pthread_mutex_unlock(&shm1->mute);
    }//while
}
static gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data) {
	gtk_main_quit();
	return FALSE;
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

    
    printf("pid=%d\n", server_pid);
   

    //---------gtk start

    if (!g_thread_supported()) g_thread_init(NULL);
    gdk_threads_init();

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
    
    g_thread_create((GThreadFunc)gui_lisener,NULL,FALSE,NULL);
 
    gdk_threads_enter();
    gtk_main();
    gdk_threads_leave();
    return 0;
}