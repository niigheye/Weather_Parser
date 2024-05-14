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

void AppView::init_window(GtkWidget *window)
{
    gtk_window_set_title(GTK_WINDOW(window), "BitBot_2.0");
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    g_object_add_weak_pointer(G_OBJECT(window), (gpointer *)&window);
}


void AppView::activate(GtkApplication *app, gpointer user_data)
{
    static GtkWidget *window = NULL;
    GtkWidget *grid;
    GtkWidget *button1;
    GtkWidget *button_quit;
    GtkWidget *button2;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *entry;
    GtkEntryCompletion *completion;
    GtkTreeModel *completion_model;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    init_window(window);
    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(window), grid);

    button1 = gtk_button_new_with_label("Button 1");
    button2 = gtk_button_new_with_label("Button 2");
    button_quit = gtk_button_new_with_label("Quit");


    /* Place the first button in the grid cell (0, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_quit, 0, 1, 2, 1);

    g_signal_connect(button1, "clicked", G_CALLBACK(print_hello), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(print_hello), NULL);
    g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

    /* Place the second button in the grid cell (1, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    

    /* Place the Quit button in the grid cell (0, 1), and make it
     * span 2 columns.
     */

    //--------------------------------------
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(vbox, 18);
    gtk_widget_set_margin_end(vbox, 18);
    gtk_widget_set_margin_top(vbox, 18);
    gtk_widget_set_margin_bottom(vbox, 18);
    gtk_grid_attach(GTK_GRID(grid), vbox, 2, 2, 2, 1);


    /* Create our entry */
    entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(vbox), entry);

    gtk_accessible_update_property(GTK_ACCESSIBLE(entry),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);

    /* Create the completion object */
    completion = gtk_entry_completion_new();

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry), completion);
    g_object_unref(completion);

    /* Create a tree model and use it as the completion model */
    completion_model = WeatherForecastModel::create_completion_model();
    gtk_entry_completion_set_model(completion, completion_model);
    g_object_unref(completion_model);

    /* Use model column 0 as the text column */
    gtk_entry_completion_set_text_column(completion, 0);

    gtk_entry_completion_set_inline_completion(completion, TRUE);
    gtk_entry_completion_set_inline_selection(completion, TRUE);

    if (!gtk_widget_get_visible(window))
        gtk_widget_set_visible(window, TRUE);
    else
        gtk_window_destroy(GTK_WINDOW(window));
    //-------------------------------

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
