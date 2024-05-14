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

#include <fstream>
#include <string>

int main(int argc, char **argv)
{
    system("chcp 1251>nul");

    std::ifstream in("../src/cities.txt");
    std::ofstream out("../src/res.txt");
    std::string s;
    // int i=0;
    while (std::getline(in, s))
    {
        std::cout << s << std::endl;
        iterator begin = find_if(s.begin(), s.end(), ::isalpha);
        string::iterator end = find_if(begin + 1, s.end(), is_not_alpha);
        s.erase(begin, end);
        // out << tmp << std::endl;
        // std::cout << i++;
    }
    std::cout << "i finished";

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
