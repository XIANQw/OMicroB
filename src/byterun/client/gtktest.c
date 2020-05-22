#include <gtk/gtk.h>

static void activate(GtkApplication * app, gpointer user_date);
static void print_hello(GtkWidget *widget, gpointer data);

int main(int argc, char * argv[])
{
    GtkApplication * app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static void activate(GtkApplication * app, gpointer user_date)
{
    GtkWidget * window;
    GtkWidget * grid;
    GtkWidget * button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_container_set_border_width(GTK_CONTAINER(window), 150);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    button = gtk_button_new_with_label("Button 1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label("Button 2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    button = gtk_button_new_with_label("Quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

    GtkWidget *view;
    GtkTextBuffer *buffer;

    view = gtk_text_view_new ();

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

    gtk_text_buffer_set_text (buffer, "Hello, this is some text", -1);

    gtk_grid_attach(GTK_GRID(grid), view, 0, 2, 2, 1);
    /* Now you might put the view in a container and display it on the
    * screen; when the user edits the text, signals on the buffer
    * will be emitted, such as "changed", "insert_text", and so on.
    */

    gtk_widget_show_all(window);
}

static void print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World\n");
}