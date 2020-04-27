#include <gtk/gtk.h>
#include <fcntl.h> //pid_t
#include <pthread.h> //pthread_t
#include <unistd.h> //read, write
#include<sys/stat.h> //mkfifo

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#define SCREEN_SIZE 5
#define SERVER_W "/tmp/serverWrite"
#define SERVER_R "/tmp/serverRead"


GtkWidget* window; // main window
GtkWidget* button_box; // button_box
GtkWidget* box;
GtkWidget* table;
GtkWidget* button; // buttons

GtkWidget* screen[SCREEN_SIZE][SCREEN_SIZE];
int bval[2];
int fd_w,fd_r;
int pid_w,pid_r;
char msg_w[BUFSIZ],msg_r[BUFSIZ];
pid_t server_pid=0;

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
    write(fd_w, msg_w, strlen(msg_w));
}
void press_b(GtkWidget* widget, gpointer data){
    bval[1]= 1-bval[1];
    if(bval[1]) snprintf(msg_w, BUFSIZ, "%d", 3);
    else snprintf(msg_w, BUFSIZ, "%d", 4);
    write(fd_w, msg_w, strlen(msg_w));
}

void destroy(GtkWidget* widget, gpointer data){
    gtk_main_quit();
}

int gui(int argc, char **argv){
    // window > box > table + buttonbox > buttons

    gtk_init(&argc, &argv);
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "simulator");
    
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
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

void* fun_lisener(void * arg){
    printf("read processus start\n");
    printf("\n---------------------notify: client can recieve msg\n");
    int x=0, y=0;
    while(1){           
        if(read(fd_r,msg_r,BUFSIZ) == -1){
            perror("client recieve msg fail"); continue;
        }
        size_t len = strlen(msg_r);
        if(len==0){
            printf("msg is empty\n"); continue;
        }
        printf("msg_r=%s\n", msg_r);
        char op = msg_r[0];
        switch (op) {
        case '0':
            for(int i=0; i<SCREEN_SIZE; i++){
                for(int j=0; j<SCREEN_SIZE; j++){
                    int index = SCREEN_SIZE*i+j+1;
                    if(index>=len) break;
                    modify_screen(j, i, msg_r[index]-'0');
                }
            }
        case '1':
            x=msg_r[1]-'0';
            y=msg_r[2]-'0';
            printf("x=%d, y=%d\n", x, y);
            modify_screen(x,y,msg_r[3]-'0');
            break;
        case '2':
            clear_screen(); break;
        default:
            break;
        }
    }//while
    printf("---------------------notify：client terminated read processus\n");
}


int main(){
    printf("Program start\n");
    pthread_t pgui;
    pthread_create(&pgui, NULL, (void *)&gui, NULL);
    // pipe_read
    if(access(SERVER_W,0) < 0){
        pid_r = mkfifo(SERVER_W,0700);
        if(pid_r < 0){
            perror("pipe_read create fail"); exit(0);
        }
        printf("pipe_read%s create\n",SERVER_W);
    }
    fd_r = open(SERVER_W,O_RDWR);
    if(fd_r < 0){
        perror("open pipe_read fail"); exit(0);
    }
    printf("open pipe_read\n");
 
    //pipe_write
    if(access(SERVER_R,0) < 0){
        pid_w = mkfifo(SERVER_R,0700);
        if(pid_w < 0){
            perror("client create pipe_write fail"); exit(0);
        }
        printf("pipe_write%s create\n",SERVER_R);
    }
    fd_w = open(SERVER_R,O_RDWR);
    if(fd_w < 0){
        perror("open pipe_write fail"); exit(0);
    }
    printf("open pipe_write\n");
    
    // get pid of server
    while(1){
        if( read(fd_r,msg_r,BUFSIZ) == -1 ){
            perror("client recieve msg fail"); exit(0);
        }
        if ((strlen(msg_r) > 3) && (msg_r[0]=='p') && (msg_r[1]='i') && (msg_r[2]=='d')){
            char pidch[10];
            strcpy(pidch, msg_r+3);
            server_pid = atoi(pidch);
            printf("server_pid=%d\n", server_pid);
            break;
        }    
    }
    printf("client start communication\n");
    pthread_t plisener;
    pthread_create(&plisener, NULL, (void *)&fun_lisener, NULL);
    pthread_join(pgui, NULL);
    pthread_join(plisener, NULL);
    return 0;
}