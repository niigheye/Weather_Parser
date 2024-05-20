#include "appView.h"

using json=nlohmann::json;

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
    GtkWidget *entry_period;
    GtkWidget *city;
    GtkWidget *time;
    GtkWidget *header_label;  
    GtkWidget *forecast_box;  
    GtkEntryCompletion *completion_period; 
    GtkEntryCompletion *completion_city;
    GtkWidget *tree_view_city;
    GtkWidget *tree_view_period;
    GtkTreeModel *completion_model_city;
    GtkTreeModel *completion_model_period;
    GtkTreeSelection *tree_selection_period;
    GtkTreeSelection *tree_selection_city;
    GtkTreeIter iter_selected_city;
    GtkTreeIter iter_selected_period;


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

    
    completion_model_city = WeatherForecastModel::create_completion_model();
    completion_model_period = WeatherForecastModel::create_completion_model();
    tree_view_city = gtk_tree_view_new_with_model(completion_model_city);
    tree_view_period = gtk_tree_view_new_with_model(completion_model_period);
    tree_selection_city = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_city));
    tree_selection_period = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view_period));
    gtk_tree_selection_set_mode(tree_selection_city, GTK_SELECTION_SINGLE);
    gtk_tree_selection_set_mode(tree_selection_period, GTK_SELECTION_SINGLE);
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

    /* Create our entry for time */
    time = gtk_label_new("Time:");
    gtk_widget_set_name(time, "time-label");
    gtk_widget_set_margin_end(time,5);
    gtk_widget_set_margin_bottom(time, 15);
    context = gtk_widget_get_style_context(time);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_grid_attach(GTK_GRID(grid), time, 0, 0, 1, 1);

    entry_period = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_period, 1, 0, 3, 1);
    gtk_widget_set_margin_bottom(entry_period, 10);
    gtk_box_append(GTK_BOX(vbox), entry_period);
    gtk_widget_set_size_request(entry_period, 20, 10);

    gtk_accessible_update_property(GTK_ACCESSIBLE(entry_period),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);

    /* Create the completion object */
    completion_period = gtk_entry_completion_new();

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry_period), completion_period);
    g_object_unref(completion_period);

    /* Create a tree model and use it as the completion model */

    gtk_entry_completion_set_model(completion_period, completion_model_period);
    g_object_unref(completion_model_period);

    /* Use model column 0 as the text column */
    gtk_entry_completion_set_text_column(completion_period, 0);

    gtk_entry_completion_set_inline_completion(completion_period, TRUE);
    gtk_entry_completion_set_inline_selection(completion_period, TRUE);
    gtk_entry_completion_get_popup_set_width(completion_period);


    /* Create our entry for city */

    city = gtk_label_new("City:");
    gtk_widget_set_name(city, "city-label");
    gtk_grid_attach(GTK_GRID(grid), city, 0, 1, 1, 1);
    gtk_widget_set_margin_end(city, -10);
    gtk_widget_set_margin_bottom(city, 15);
    context = gtk_widget_get_style_context(city);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    entry_city = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_city, 1, 1, 3, 1);
    //gtk_widget_set_margin_top(entry, 10);
    gtk_widget_set_margin_bottom(entry_city, 10);
    gtk_box_append(GTK_BOX(vbox), entry_city);
    gtk_widget_set_size_request(button_quit, 20, 10);

    gtk_accessible_update_property(GTK_ACCESSIBLE(entry_city),
                                   GTK_ACCESSIBLE_PROPERTY_AUTOCOMPLETE, GTK_ACCESSIBLE_AUTOCOMPLETE_LIST,
                                   -1);

    /* Create the completion object */
    completion_city = gtk_entry_completion_new();

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry_city), completion_city);
    g_object_unref(completion_city);

    /* Create a tree model and use it as the completion model */

    gtk_entry_completion_set_model(completion_city, completion_model_city);
    g_object_unref(completion_model_city);

    /* Use model column 0 as the text column */
    gtk_entry_completion_set_text_column(completion_city, 0);

    gtk_entry_completion_set_inline_completion(completion_city, TRUE);
    gtk_entry_completion_set_inline_selection(completion_city, TRUE);
    gtk_entry_completion_get_popup_set_width(completion_city);
    //----------------------------------------------------------------------
   
 

    g_signal_connect(button_request, "clicked", G_CALLBACK(on_find_clicked), entry_city);
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

    // Simulate loading forecast data from a JSON file

    GtkWidget *forecast_grid = gtk_grid_new();

    std::ifstream ifs("../src/request.json");
    json forecast_data = json::parse(ifs);
    display_weather_forecast(forecast_box, forecast_data,forecast_grid);
    gtk_grid_attach(GTK_GRID(grid),forecast_grid,0,0,1,1 );
}

void AppView::display_weather_forecast(GtkWidget *container, const json &forecast_data,GtkWidget *forecast_grid)
{
    
    gtk_grid_set_row_spacing(GTK_GRID(forecast_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(forecast_grid), 10);

    int row = 0;
    for (size_t i = 0; i < 40; i += 8)
    {
        const auto &day = forecast_data["list"][i];

        GtkWidget *date_label = gtk_label_new(day["dt"].get<std::string>().c_str());
        GtkWidget *icon_image = gtk_image_new_from_file((day["weather"][0]["icon"].get<std::string>() + ".png").c_str());
        std::string temp_text = std::to_string(day["main"]["temp_min"].get<float>()) + " / " + std::to_string(day["main"]["temp_max"].get<float>());
        GtkWidget *temp_label = gtk_label_new(temp_text.c_str());

       gtk_grid_attach(GTK_GRID(forecast_grid), date_label, 0, row, 1, 1);
       gtk_grid_attach(GTK_GRID(forecast_grid), icon_image, 1, row, 1, 1);
       gtk_grid_attach(GTK_GRID(forecast_grid), temp_label, 2, row, 1, 1);

        row++;
    }

    gtk_box_append(GTK_BOX(container), forecast_grid);

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