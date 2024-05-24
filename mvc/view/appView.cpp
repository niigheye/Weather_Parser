#include "appView.h"
#include "WeatherForecastModel.h"
#include <gtk/gtk.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;
void on_find_clicked(GtkWidget *widget, GtkWidget *entry)
{
    std::string res = gtk_editable_get_text(GTK_EDITABLE(entry));
    if (res == "")
        return;

    int pos = res.find_first_of(',');
    std::string city = res.substr(0, pos);
    std::string state = res.substr(pos + 2, res.length() - pos);
    if ((city == WeatherForecastModel::m_GetCity()) && (state == WeatherForecastModel::m_GetState()))
        return;
    WeatherForecastModel::m_SetCity(city);
    WeatherForecastModel::m_SetState(state);
    ;
    WeatherForecastModel::m_GetForecast();
}

void AppView::clear_forecast_grid(GtkWidget *forecast_grid)
{
    GtkWidget *child = gtk_widget_get_first_child(forecast_grid);
    while (child != NULL)
    {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_grid_remove(GTK_GRID(forecast_grid), child);
        child = next;
    }
}

void on_find_clicked_combo(GtkWidget *widget, GtkComboBox *combo_box)
{
    int val = gtk_combo_box_get_active(combo_box);
    int val_to_set = -1;
    if (val == 0)
    {
        val_to_set = 1;
    }
    else if (val == 1)
    {
        val_to_set = 3;
    }
    else
    {
        val_to_set = 5;
    }

    if (val_to_set != WeatherForecastModel::m_GetDays())
    {
        WeatherForecastModel::m_SetReady(1);
    }
    WeatherForecastModel::m_SetDays(val_to_set);
}

void drawHandler(GtkWidget *widget, GtkWidget *grid)
{
    AppView::display_weather_forecast(grid);
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

void AppView::StyleBox(GtkWidget *vbox)
{
    gtk_widget_set_margin_start(vbox, 18);
    gtk_widget_set_margin_end(vbox, 18);
    gtk_widget_set_margin_top(vbox, 18);
    gtk_widget_set_margin_bottom(vbox, 18);
}

void AppView::StyleGrid(GtkWidget *grid)
{
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
}

void AppView::StyleHeaderBox(GtkWidget *header_box)
{
    gtk_box_set_homogeneous(GTK_BOX(header_box), TRUE);
    gtk_widget_set_margin_top(header_box, 200);
    gtk_widget_set_margin_bottom(header_box, 100);
}

void AppView::StyleBtnQuit(GtkWidget *button_quit)
{
    gtk_widget_set_halign(button_quit, GTK_ALIGN_START);
    gtk_widget_set_valign(button_quit, GTK_ALIGN_START);
    gtk_widget_set_size_request(button_quit, 30, 30);
}

void AppView::StyleBtnRequest(GtkWidget *button_request)
{
    gtk_widget_set_size_request(button_request, 200, 60);
    gtk_widget_set_halign(button_request, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button_request, GTK_ALIGN_CENTER);
}

void AppView::StyleScrollWindow(GtkWidget *scrolled_window)
{
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
}

void AppView::StyleForecastGrid(GtkWidget *forecast_grid)
{
    gtk_grid_set_column_homogeneous(GTK_GRID(forecast_grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(forecast_grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(forecast_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(forecast_grid), 10);
}

void AppView::StyleEntryCompletion(GtkEntryCompletion *completion_city)
{
    gtk_entry_completion_set_text_column(completion_city, 0);
    gtk_entry_completion_set_inline_completion(completion_city, TRUE);
    gtk_entry_completion_set_inline_selection(completion_city, TRUE);
    gtk_entry_completion_get_popup_set_width(completion_city);
}

void AppView::StyleForecastScrolledWindow(GtkWidget *forecast_scrolled_window)
{
    gtk_widget_set_vexpand(forecast_scrolled_window, TRUE);
    gtk_widget_set_hexpand(forecast_scrolled_window, TRUE);
}

void AppView::activate(GtkApplication *app, gpointer user_data)
{
    static GtkWidget *window = NULL;
    GtkWidget *grid;
    GtkWidget *button_quit;
    GtkWidget *button_request;
    GtkWidget *label;
    GtkWidget *entry_city;
    GtkEntryCompletion *completion_city;
    GtkWidget *tree_view_city;
    GtkTreeModel *completion_model_city;
    GtkTreeIter iter_selected_city;

    GtkWidget *tree_view_period;
    GtkTreeModel *completion_model_period;

    GtkTreeIter iter;
    GtkListStore *liststore;
    GtkWidget *combo;
    GtkCellRenderer *column;

    GtkWidget *city;
    GtkWidget *time;
    GtkWidget *header_label;
    GtkWidget *forecast_box;
    GtkWidget *forecast_grid;
    GtkWidget *forecast_scrolled_window;

    GtkStyleContext *context;
    GtkCssProvider *provider;

    GtkWidget *scrolled_window;
    GtkWidget *main_vbox;
    GtkWidget *header_box;

    GtkWidget *overlay;
    //---------------------------------------------------------
    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    init_window(window);

    grid = gtk_grid_new();
    // gtk_window_set_child(GTK_WINDOW(window), grid);

    scrolled_window = gtk_scrolled_window_new();
    button_request = gtk_button_new_with_label("Find");
    button_quit = gtk_button_new_with_label("Quit");
    completion_model_city = WeatherForecastModel::CreateCompletionModelCity();
    liststore = WeatherForecastModel::CreateListStorePeriod();
    combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(liststore));
    column = gtk_cell_renderer_text_new();
    entry_city = gtk_entry_new();
    tree_view_city = gtk_tree_view_new_with_model(completion_model_city);
    completion_city = gtk_entry_completion_new();
    time = gtk_label_new("Time:");
    city = gtk_label_new("City:");
    entry_city = gtk_entry_new();
    header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    provider = gtk_css_provider_new();
    forecast_grid = gtk_grid_new();
    header_label = gtk_label_new("Let's check your weather!");
    forecast_scrolled_window = gtk_scrolled_window_new();
    overlay = gtk_overlay_new();

    //------------------------------------------------------------
    gtk_widget_set_name(combo, "combo-dropdown");
    gtk_widget_set_name(header_label, "header-label");
    gtk_widget_set_name(time, "time-label");
    gtk_widget_set_name(city, "city-label");
    gtk_widget_set_name(button_request, "find-button");
    gtk_widget_set_name(forecast_grid, "forecast-grid");
    gtk_widget_set_name(button_quit, "quit-button");
    //-------------------------------------------------------------
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

    // scrolled window
    StyleScrollWindow(scrolled_window);
    gtk_window_set_child(GTK_WINDOW(window), scrolled_window);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), main_vbox);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(forecast_scrolled_window), forecast_grid);
    // style forecast_scrolled_window
    StyleForecastScrolledWindow(forecast_scrolled_window);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), forecast_scrolled_window);
    gtk_box_append(GTK_BOX(main_vbox), overlay);
    // Верхняя часть интерфейса
    StyleHeaderBox(header_box);
    gtk_box_append(GTK_BOX(main_vbox), header_box);

    // settings of grid;
    StyleGrid(grid);
    gtk_box_append(GTK_BOX(header_box), grid);

    g_object_unref(liststore);
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), column, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), column, "cell-background", 0, "text", 1, NULL);

    //------------------------------- CSS LOGIC
    gtk_css_provider_load_from_path(provider, "../src/main.css");

    context = gtk_widget_get_style_context(header_label); //
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(time);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(city);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(button_request);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(button_quit);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // display_weather_forecast_5(forecast_grid, forecast_data);
    // display_weather_forecast_3(forecast_grid, forecast_data);
    //display_weather_forecast_1(forecast_grid, forecast_data);

    // Добавление контейнера для статичных элементов под forecast_grid
    GtkWidget *details_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(main_vbox), details_box);
    //display_static_weather_details_1(details_box, forecast_data);

    gtk_grid_attach(GTK_GRID(grid), button_request, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo, 1, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), time, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_city, 1, 1, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), button_quit, 2, 2, 2, 2);
    gtk_grid_attach(GTK_GRID(grid), city, 0, 1, 1, 1);

    /* Assign the completion to the entry */
    gtk_entry_set_completion(GTK_ENTRY(entry_city), completion_city);
    g_object_unref(completion_city);

    /* Create a tree model and use it as the completion model */

    gtk_entry_completion_set_model(completion_city, completion_model_city);
    g_object_unref(completion_model_city);

    /* Use model column 0 as the text column */
    StyleEntryCompletion(completion_city);

    //----------------------------------------------------------------------

    gtk_widget_set_halign(header_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(header_box), header_label);
    //--------------------------------------------
    gtk_widget_set_size_request(entry_city, 250, 70);
    //-----------------------------------
    StyleBtnRequest(button_request);
    StyleBtnQuit(button_quit);

    //-----------------------------------------------------------------------
    StyleForecastGrid(forecast_grid);
    gtk_box_append(GTK_BOX(main_vbox), forecast_grid);

    // Загрузка данных и отображение прогноза

    g_signal_connect(button_request, "clicked", G_CALLBACK(on_find_clicked_combo), GTK_COMBO_BOX(combo));
    g_signal_connect(button_request, "clicked", G_CALLBACK(on_find_clicked), entry_city);
    g_signal_connect_after(button_request, "clicked", G_CALLBACK(drawHandler), forecast_grid);
    g_signal_connect_swapped(button_quit, "clicked", G_CALLBACK(gtk_window_destroy), window);

    if (!gtk_widget_get_visible(window))
        gtk_widget_set_visible(window, TRUE);
    else
        gtk_window_destroy(GTK_WINDOW(window));
}

//------------------------------ приописываем вывод для 1 дня
void AppView::display_weather_forecast_1(GtkWidget *forecast_grid, const json &forecast_data)
{
    int column = 0;
    GtkWidget *times_row = gtk_grid_new();
    GtkWidget *icon_row = gtk_grid_new();
    GtkWidget *temp_row = gtk_grid_new();

    gtk_grid_set_row_spacing(GTK_GRID(times_row), 117);
    gtk_grid_set_column_spacing(GTK_GRID(times_row), 180);
    gtk_grid_set_row_spacing(GTK_GRID(icon_row), 5);
    gtk_grid_set_column_spacing(GTK_GRID(icon_row), 212);
    gtk_grid_set_row_spacing(GTK_GRID(temp_row), 5);
    gtk_grid_set_column_spacing(GTK_GRID(temp_row), 196);

    for (size_t i = 0; i < 9; i += 1)
    {
        const auto &day = forecast_data["list"][i];

        std::string dt_txt = day["dt_txt"].get<std::string>();
        std::string time = dt_txt.substr(11, 5);

        std::string temp_text = std::to_string(day["main"]["temp"].get<int>()) + "°C";

        std::string icon_path = "../src/icons/" + day["weather"][0]["icon"].get<std::string>() + ".png";
        GtkWidget *icon_image = gtk_image_new_from_file(icon_path.c_str());
        gtk_widget_set_size_request(icon_image, 100, 100);
        gtk_widget_set_margin_start(icon_image, 35);

        GtkWidget *times_label = gtk_label_new(time.c_str());
        gtk_widget_set_name(times_label, "times-label");

        GtkWidget *temp_label = gtk_label_new(temp_text.c_str());
        gtk_widget_set_name(temp_label, "temp-label");

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(provider, "../src/main.css");

        GtkStyleContext *context;
        context = gtk_widget_get_style_context(times_label);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
}

void AppView::display_weather_forecast(GtkWidget *forecast_grid)
{

    std::ifstream ifs("../src/request.json");
    json forecast_data = json::parse(ifs);
    ifs.close();
    int row = 0;
    // json forecast_data = WeatherForecastModel::m_GetAnswer();
    GtkCssProvider *provider1 = gtk_css_provider_new();
    GtkStyleContext *context1;
    GtkWidget *date_label;
    std::string month_date;
    std::string dt_txt;
    std::string icon_path;
    std::string temp_text;
    GtkWidget *temp_label;
    GtkWidget *icon_image;
    gtk_css_provider_load_from_path(provider1, "../src/main.css");

    if (gtk_grid_get_child_at(GTK_GRID(forecast_grid), 1, 1))
    {
        clear_forecast_grid(forecast_grid);
    }
    for (size_t i = 0; i < forecast_data["cnt"]; i += 8)
    {
        const auto &day = forecast_data["list"][i];

        dt_txt = day["dt_txt"].get<std::string>();
        month_date = dt_txt.substr(5, 5);
        date_label = gtk_label_new(month_date.c_str());
        gtk_widget_set_name(date_label, "date-label");

        icon_path = "../src/icons/" + day["weather"][0]["icon"].get<std::string>() + ".png";
        icon_image = gtk_image_new_from_file(icon_path.c_str());
        gtk_widget_set_name(icon_image, "icon-image");
        gtk_widget_set_size_request(icon_image, 80, 80);

        temp_text = std::to_string(day["main"]["temp_min"].get<int>()) + "°" + " / " + std::to_string(day["main"]["temp_max"].get<int>()) + "°";
        temp_label = gtk_label_new(temp_text.c_str());
        gtk_widget_set_name(temp_label, "temp-label");

        context1 = gtk_widget_get_style_context(date_label);
        gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

        context1 = gtk_widget_get_style_context(icon_image);
        gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

        context1 = gtk_widget_get_style_context(temp_label);
        gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

        column++;
    }

    gtk_grid_attach(GTK_GRID(forecast_grid), times_row, 0, 0, 2, 3);
    gtk_grid_attach(GTK_GRID(forecast_grid), icon_row, 0, 1, 2, 3);
    gtk_grid_attach(GTK_GRID(forecast_grid), temp_row, 0, 2, 2, 1);
}

void AppView::display_static_weather_details_1(GtkWidget *details_box, const json &forecast_data)
{
    const auto &current_day = forecast_data["list"][0];
    std::string pressure_text = "Pressure: " + std::to_string(current_day["main"]["pressure"].get<int>()) + " hPa";
    std::string humidity_text = "Humidity: " + std::to_string(current_day["main"]["humidity"].get<int>()) + "%";
    std::string feels_like_text = "Feels like: " + std::to_string(current_day["main"]["feels_like"].get<int>()) + "°C";
    std::string weather_desc_text = "Clouds: " + current_day["weather"][0]["description"].get<std::string>();
    std::string wind_speed_text = "Wind speed: " + std::to_string(current_day["wind"]["speed"].get<int>()) + " m/s";

    GtkWidget *pressure_label = gtk_label_new(pressure_text.c_str());
    gtk_widget_set_name(pressure_label, "pressure-label");

    GtkWidget *humidity_label = gtk_label_new(humidity_text.c_str());
    gtk_widget_set_name(humidity_label, "humidity-label");

    GtkWidget *feels_like_label = gtk_label_new(feels_like_text.c_str());
    gtk_widget_set_name(feels_like_label, "feels-like-label");

    GtkWidget *weather_desc_label = gtk_label_new(weather_desc_text.c_str());
    gtk_widget_set_name(weather_desc_label, "weather-desc-label");

    GtkWidget *wind_speed_label = gtk_label_new(wind_speed_text.c_str());
    gtk_widget_set_name(wind_speed_label, "wind-speed-label");

    GtkCssProvider *provider1 = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider1, "../src/main.css");

    GtkStyleContext *context1;
    context1 = gtk_widget_get_style_context(pressure_label);
    gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context1 = gtk_widget_get_style_context(humidity_label);
    gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context1 = gtk_widget_get_style_context(feels_like_label);
    gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context1 = gtk_widget_get_style_context(weather_desc_label);
    gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

    context1 = gtk_widget_get_style_context(wind_speed_label);
    gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_box_append(GTK_BOX(details_box), pressure_label);
    gtk_box_append(GTK_BOX(details_box), humidity_label);
    gtk_box_append(GTK_BOX(details_box), feels_like_label);
    gtk_box_append(GTK_BOX(details_box), weather_desc_label);
    gtk_box_append(GTK_BOX(details_box), wind_speed_label);
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
