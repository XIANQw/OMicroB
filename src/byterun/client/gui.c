#include "gui.h"
#include "client.h"

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
    free_shm(shm1);
    free_shm(shm2);
    shmdt(shm_env);
    shmctl(envid, IPC_RMID, 0);
    gtk_main_quit();
    kill(server_pid, SIGKILL);
    kill(mypid, SIGKILL);
}

static gboolean delete_event(GtkWidget * widget, GdkEvent * event, gpointer data) {
	gtk_main_quit();
	return FALSE;
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


GtkWidget* create_pin_row(int height, int width){
    GtkWidget* grid, *button;
    grid = gtk_grid_new();
    button = gtk_button_new_with_label("Row pins");
    gtk_widget_set_sensitive(button, FALSE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, width, 1);
    pin_row=(GtkWidget **)malloc(sizeof(GtkWidget *) * shm_env->nb_pins_row);
    for(int i=0; i<shm_env->nb_pins_row; i++){
        button = gtk_button_new_with_label("0");
        pin_row[i] = button;
        gtk_widget_set_sensitive(button, FALSE);
        gtk_grid_attach(GTK_GRID(grid), button, i, 1, 1, 1);
    }
    return grid;
}

GtkWidget* create_pin_col(int height, int width){
    GtkWidget* grid, *button;
    grid = gtk_grid_new();
    button = gtk_button_new_with_label("Col pins");
    gtk_widget_set_sensitive(button, FALSE);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, width, 1);
    pin_col=(GtkWidget **)malloc(sizeof(GtkWidget *) * shm_env->nb_pins_col);
    for(int i=0; i<shm_env->nb_pins_col; i++){
        button = gtk_button_new_with_label("1");
        pin_col[i] = button;
        gtk_widget_set_sensitive(button, FALSE);
        gtk_grid_attach(GTK_GRID(grid), button, i, 1, 1, 1);
    }
    return grid;
}



GtkWidget* create_UI(){
    GtkWidget *window, *button_box, *button;

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "simulator");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gui_destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 50);
    
    GtkWidget* box=gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget* grid=create_pin_row(1, shm_env->nb_pins_row);
    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 3);
    
    grid=create_pin_col(1, shm_env->nb_pins_col);
    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 3);
    
    grid=create_grid(SCREEN_SIZE, SCREEN_SIZE);
    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 3);
    
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_set_spacing(GTK_BOX(button_box), 5);
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 3);

    button = gtk_button_new_with_label("A");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(press_a), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("B");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(press_b), NULL);
    gtk_box_pack_start(GTK_BOX(button_box), button, FALSE, FALSE, 0);

    return window;
}
