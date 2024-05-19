#include "appView.h"

void on_find_clicked(GtkWidget *widget, GtkWidget *entry)
{
    std::string res = gtk_editable_get_text(GTK_EDITABLE(entry));
    int pos = res.find_first_of(',');
    std::string city = res.substr(0, pos);
    std::string state = res.substr(pos+2, res.length() - pos); 
    if (!(city == WeatherForecastModel::m_GetCity() && state == WeatherForecastModel::m_GetState()))
    {
        WeatherForecastModel::m_SetCity(city);
        WeatherForecastModel::m_SetState(state);
        WeatherForecastModel::m_GetForecast();
    }
}

AppView::AppView(WeatherForecastModel *model)
{
    this->model = model;
    this->model->AddObserver(this);
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
    static GtkWidget *grid;
    GtkWidget *button_quit;
    GtkWidget *button_request;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *entry_city;
    GtkEntryCompletion *completion_city;
    GtkWidget *tree_view_city;
    GtkTreeModel *completion_model_city;
    GtkTreeSelection *tree_selection_city;
    GtkTreeIter iter_selected_city;

    GtkWidget *entry_period;
    GtkEntryCompletion *completion_period;
    GtkWidget *tree_view_period;
    GtkTreeModel *completion_model_period;
    GtkTreeSelection *tree_selection_period;
    GtkTreeIter iter_selected_period;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    init_window(window);
    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(window), grid);

    button_request = gtk_button_new_with_label("find");
    button_quit = gtk_button_new_with_label("Quit");
    completion_model_city = WeatherForecastModel::create_completion_model();
    completion_model_period = WeatherForecastModel::create_completion_model();
    tree_view_city = gtk_tree_view_new_with_model(completion_model_city);
    tree_view_period = gtk_tree_view_new_with_model(completion_model_period);
    tree_selection_city = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_city));
    tree_selection_period = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_period));
    gtk_tree_selection_set_mode(tree_selection_city, GTK_SELECTION_SINGLE);
    gtk_tree_selection_set_mode(tree_selection_period, GTK_SELECTION_SINGLE);
    //------------------------------- CSS LOGIC

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "../src/main.css");

    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    //----------------------------------

    gtk_grid_attach(GTK_GRID(grid), button_request, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_quit, 0, 1, 2, 1);
    // gtk_grid_attach(GTK_GRID(grid), background_texture, 0, 1, 2, 1);
    // gtk_grid_attach(GTK_GRID(grid), combobox, 5, 5, 5, 5);

    // g_signal_connect(completion_model, "changed", G_CALLBACK(on_changed), NULL);
    // g_signal_connect(G_OBJECT(entry), "changed", G_CALLBACK(on_changed), NULL);

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
    gtk_grid_attach(GTK_GRID(grid), vbox, 28, 2, 2, 1);

    /* Create our entry */
    entry_city = gtk_entry_new();
    entry_period = gtk_entry_new();
    gtk_box_append(GTK_BOX(vbox), entry_city);
    gtk_box_append(GTK_BOX(vbox), entry_period);

    gtk_accessible_update_property(GTK_ACCESSIBLE(entry_city),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);
    gtk_accessible_update_property(GTK_ACCESSIBLE(entry_period),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);

    /* Create the completion object */
    completion_city = gtk_entry_completion_new();
    completion_period = gtk_entry_completion_new();

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry_city), completion_city);
    gtk_entry_set_completion(GTK_ENTRY(entry_period), completion_period);
    g_object_unref(completion_city);
    g_object_unref(completion_period);

    /* Create a tree model and use it as the completion model */

    gtk_entry_completion_set_model(completion_city, completion_model_city);
    gtk_entry_completion_set_model(completion_period, completion_model_period);
    g_object_unref(completion_model_city);
    g_object_unref(completion_model_period);

    /* Use model column 0 as the text column */
    gtk_entry_completion_set_text_column(completion_city, 0);
    gtk_entry_completion_set_text_column(completion_period, 0);

    gtk_entry_completion_set_inline_completion(completion_city, TRUE);
    gtk_entry_completion_set_inline_selection(completion_city, TRUE);
    gtk_entry_completion_get_popup_set_width(completion_city);

    
    gtk_entry_completion_set_inline_completion(completion_period, TRUE);
    gtk_entry_completion_set_inline_selection(completion_period, TRUE);
    gtk_entry_completion_get_popup_set_width(completion_period);
    //----------------------------------------------------------------------

    g_signal_connect(button_request, "clicked", G_CALLBACK(on_find_clicked), entry_city);
    g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

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
