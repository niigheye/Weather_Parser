#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include <weatherForecastModel.h>
#include <appView.h>
#include <appController.h>

int main(int argc, char **argv)
{
    system("chcp 1251>nul");

    WeatherForecastModel model;
    AppView view(&model);
    AppController controller(&model);

    AppView::argc = argc;
    AppView::argv = argv;

    controller.GetInfo();

    model.m_CreateRequest();
    model.m_Parse();
    view.ShowInfo();
    return 0;
}
