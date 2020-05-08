#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
static gboolean delete_event(GtkWidget * widget, GdkEvent * event,
		gpointer data) {
	gtk_main_quit();
	return FALSE;
}
int main(int argc, char *argv[]) {
	GtkWidget *window;
	GtkWidget *label;
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), "RobberPhex");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	label = gtk_label_new("Hello GTK!");
	gtk_container_add(GTK_CONTAINER(window), label);
	gtk_widget_show(label);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}