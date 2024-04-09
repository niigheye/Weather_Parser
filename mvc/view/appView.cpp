#include "appView.h"


AppView::AppView(WeatherForecastModel *model)
{
    this->model = model;
    this->model->AddObserver(this);
}

void AppView::print_hello(GtkWidget *widget, gpointer data)
{
    g_print("Hello World\n");
}

void AppView::activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");

    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(window), grid);

    button = gtk_button_new_with_label("Button 1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    /* Place the first button in the grid cell (0, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label("Button 2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    /* Place the second button in the grid cell (1, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    button = gtk_button_new_with_label("Quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);

    /* Place the Quit button in the grid cell (0, 1), and make it
     * span 2 columns.
     */
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

    gtk_window_present(GTK_WINDOW(window));
}

void AppView::Update()
{
    std::cout << "the app ran!\n";
    GtkApplication *app;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), AppView::argc, AppView::argv);
    g_object_unref(app);
}

void AppView::ShowInfo()
{
    Update();
}

