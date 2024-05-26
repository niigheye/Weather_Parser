#pragma once
#include "model.h"
#include "observer.h"

#include <string>
#include <iostream>
#include <fstream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <gtk/gtk.h>
#include <vector>

using json = nlohmann::json;

class WeatherForecastModel : public Model, public Observable
{
private:
   inline static std::string _city = "";
   inline static std::string _state = "";
   inline static std::string _token = "";
   inline static std::string _units = "metric";
   inline static std::string _local = "ru";
   inline static std::string _request = "";
   inline static int _days = -1;
   inline static bool _ready_for_request = false;
   inline static json _answer;

public:
    static std::string m_GetCity();
    static std::string m_GetState();
    static std::string m_GetToken();
    static std::string m_GetUnits();
    static std::string m_GetLocal();
    static bool m_GetReady();
    static int m_GetDays();
    static std::string m_GetRequest();
    static json m_GetAnswer();

    static void m_SetCity(std::string city);
    static void m_SetDays(int days);
    static void m_SetReady(bool ready);
    static void m_SetState(std::string state);
    static void m_SetToken(std::string token);
    static void m_SetUnits(std::string units);
    static void m_SetLocal(std::string local);
    static void m_SetRequest(std::string request);
    static void m_SetAnswer(json answer);

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *data);

    void Logic();

    static void m_GetForecast();
    static void m_CreateRequest();
    static std::string m_DoRequest(std::string &buffer);
    static void m_PutDataToFile(std::string buffer);
    static void m_ParseToken();
    
    static GtkTreeModel *CreateCompletionModelCity();
    static GtkListStore *CreateListStorePeriod();
    static void FillGtkTreeCity(GtkListStore *store, std::vector<std::string> myvector);
    static void FillGtkTreePeriod(GtkListStore *store);
    static void ParseFileToVector(std::vector<std::string> &myvec, std::string path);
};