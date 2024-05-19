#include "appView.h"

void on_find_clicked(GtkWidget *widget, GtkWidget *entry)
{
    std::string res = gtk_editable_get_text(GTK_EDITABLE(entry));
    res = res.substr(0, res.find_first_of(','));
    std::cout << res;
}

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
    static GtkWidget *grid;
    GtkWidget *button_quit;
    GtkWidget *button_request;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *entry1;
    GtkWidget *city;
    GtkWidget *time;
    GtkWidget *header_label;  
    GtkEntryCompletion *completion; 
    GtkEntryCompletion *completion1;
    GtkWidget *tree_view;
    GtkTreeModel *completion_model;
    GtkTreeModel *completion_model1;
    GtkTreeSelection *tree_selection;
    GtkTreeIter iter_selected;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    init_window(window);


    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), overlay);

    /* Create a box to center the grid */
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), box);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    /* Here we construct the container that is going to pack our buttons */
    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);


    /* Pack the grid in the box */
    gtk_box_append(GTK_BOX(box), grid);

    /* Pack the container in the window */
    //gtk_window_set_child(GTK_WINDOW(window), grid);

    button_request = gtk_button_new_with_label("Find");
    gtk_widget_set_name(button_request, "find-button");
    gtk_grid_attach(GTK_GRID(grid), button_request, 0, 3, 5, 1);
    gtk_widget_set_size_request(button_request, 250, 50);
    gtk_widget_set_halign(button_request, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_request, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(button_quit, 20);

    
    /* Create the "Quit" button */
    button_quit = gtk_button_new_with_label("Quit");
    gtk_widget_set_name(button_quit, "quit-button");
    gtk_widget_set_halign(button_quit, GTK_ALIGN_START);  // Align to the start (left)
    gtk_widget_set_valign(button_quit, GTK_ALIGN_START);  // Align to the start (top)
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), button_quit);
    gtk_widget_set_margin_start(button_quit, 10);  // Add some margin for positioning
    gtk_widget_set_margin_top(button_quit, 10);
    gtk_widget_set_size_request(button_quit, 80, 40);

    
    completion_model = WeatherForecastModel::create_completion_model();
    tree_view = gtk_tree_view_new_with_model(completion_model);
    tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(tree_selection, GTK_SELECTION_SINGLE);
//-------------------------------
    //const char* background_file_path = "src/background.jpg";
    //GFile* background_file = g_file_new_for_path(background_file_path);
    //GdkTexture *background_texture = gdk_texture_new_from_file(background_file, NULL);

    //gtk_widget_set_style(window, "background-image: url('background.jpg'); background-repeat: no-repeat; background-size: cover;");
    
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "../src/main.css");

 GtkStyleContext *context;

    context = gtk_widget_get_style_context(button_request);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(button_quit);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    //--------------------------------------
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(vbox, 18);
    gtk_widget_set_margin_end(vbox, 18);
    gtk_widget_set_margin_top(vbox, 18);
    gtk_widget_set_margin_bottom(vbox, 18);
    gtk_grid_attach(GTK_GRID(grid), vbox, 2, 2, 2, 1);

    /* Create the second entry with label */
    time = gtk_label_new("Time:");
    gtk_widget_set_name(time, "time-label");
    gtk_widget_set_margin_end(time,5);
    gtk_widget_set_margin_bottom(time, 15);
    context = gtk_widget_get_style_context(time);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    
    gtk_grid_attach(GTK_GRID(grid), time, 0, 0, 1, 1);

    entry1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 3, 1);

    gtk_widget_set_margin_bottom(entry1, 10);
    gtk_box_append(GTK_BOX(vbox), entry1);
    gtk_widget_set_size_request(entry1, 20, 10);

    gtk_accessible_update_property(GTK_ACCESSIBLE(entry1),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);

    /* Create the completion object */
    completion = gtk_entry_completion_new();

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry1), completion1);
    g_object_unref(completion1);

    /* Create a tree model and use it as the completion model */

    gtk_entry_completion_set_model(completion1, completion_model1);
    g_object_unref(completion_model1);

    /* Use model column 0 as the text column */
    gtk_entry_completion_set_text_column(completion1, 0);

    gtk_entry_completion_set_inline_completion(completion1, TRUE);
    gtk_entry_completion_set_inline_selection(completion1, TRUE);
    gtk_entry_completion_get_popup_set_width(completion1);


    /* Create our entry */

    city = gtk_label_new("City:");
    gtk_widget_set_name(city, "city-label");
    gtk_grid_attach(GTK_GRID(grid), city, 0, 1, 1, 1);
    gtk_widget_set_margin_end(city, -10);
    gtk_widget_set_margin_bottom(city, 15);
    context = gtk_widget_get_style_context(city);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 3, 1);
    //gtk_widget_set_margin_top(entry, 10);
    gtk_widget_set_margin_bottom(entry, 10);
    gtk_box_append(GTK_BOX(vbox), entry);
    gtk_widget_set_size_request(button_quit, 20, 10);

    gtk_accessible_update_property(GTK_ACCESSIBLE(entry),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);

    /* Create the completion object */
    completion = gtk_entry_completion_new();

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry), completion);
    g_object_unref(completion);

    /* Create a tree model and use it as the completion model */

    gtk_entry_completion_set_model(completion, completion_model);
    g_object_unref(completion_model);

    /* Use model column 0 as the text column */
    gtk_entry_completion_set_text_column(completion, 0);

    gtk_entry_completion_set_inline_completion(completion, TRUE);
    gtk_entry_completion_set_inline_selection(completion, TRUE);
    gtk_entry_completion_get_popup_set_width(completion);
    //----------------------------------------------------------------------
   
 
    g_signal_connect(button_request, "clicked", G_CALLBACK(on_find_clicked), entry1);
    g_signal_connect(button_request, "clicked", G_CALLBACK(on_find_clicked), entry);
    g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

    header_label = gtk_label_new("Let's check your weather!");
    gtk_widget_set_name(header_label, "header-label");
    gtk_grid_attach(GTK_GRID(grid), header_label, -4.5, 0, 4, 2);
    gtk_widget_set_halign(header_label, GTK_ALIGN_CENTER);
    context = gtk_widget_get_style_context(header_label);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

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