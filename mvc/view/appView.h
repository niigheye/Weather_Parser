#pragma once
#include <weatherForecastModel.h>
#include <view.h>
#include <observer.h>
#include <iostream>
#include <gtk/gtk.h>

class AppView : public View, public Observer
{
    WeatherForecastModel *model;

public:
    static int argc;
    static char **argv;

    AppView(WeatherForecastModel *model);
    virtual void Update() override;
    virtual void ShowInfo() override;
    static void print_hello(GtkWidget *widget, gpointer data);
    static void activate(GtkApplication *app, gpointer user_data);
};