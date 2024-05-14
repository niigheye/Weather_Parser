#include <iostream>
#include <fstream>
#include <string>
#include <stdbool.h>
#include <glib/gstdio.h>
#include <stdio.h>
#include <errno.h>
#include <pixman.h>

#include "mvc/model/weatherForecastModel.h"
#include "mvc/view/appView.h"
#include "mvc/controller/appController.h"

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
