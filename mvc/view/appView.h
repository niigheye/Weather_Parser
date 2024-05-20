#pragma once
#include "weatherForecastModel.h"
#include "view.h"
#include "observer.h"
#include <iostream>
#include <gtk/gtk.h>

class AppView : public View, public Observer
{
private:
    WeatherForecastModel *model;

public:
    inline static int argc = 0;
    inline static char **argv = nullptr;

    AppView(WeatherForecastModel *model);
    virtual void Update() override;
    virtual void ShowInfo() override;
    static void init_window(GtkWidget *window);

    static void StyleBox(GtkWidget *vbox);
    static void StyleGrid(GtkWidget *grid);
    static void StyleHeaderBox(GtkWidget *header_box);
    static void StyleBtnQuit(GtkWidget *button_quit);
    static void StyleBtnRequest(GtkWidget *button_request);
    static void StyleScrollWindow(GtkWidget *scrolled_window);
    static void StyleForecastGrid(GtkWidget *forecast_grid);
    static void StyleEntryCompletion(GtkEntryCompletion *completion_city);
    static void activate(GtkApplication *app, gpointer user_data);
    static void display_weather_forecast(GtkWidget *forecast_grid);
};