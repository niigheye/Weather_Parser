#pragma once

#include <model.h>
#include <observer.h>

#include <string>
#include <iostream>
#include <fstream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class WeatherForecastModel : public Model, public Observable
{
private:
    std::string _city = "moscow";
    std::string _token = "";
    std::string _units = "metric";
    std::string _local = "ru";
    std::string _request = "";
    json _answer;

public:
    std::string m_GetCity();
    std::string m_GetToken();
    std::string m_GetUnits();
    std::string m_GetLocal();
    std::string m_GetRequest();
    json m_GetAnswer();

    void m_SetCity(std::string city);
    void m_SetToken(std::string token);
    void m_SetUnits(std::string units);
    void m_SetLocal(std::string local);
    void m_SetRequest(std::string request);
    void m_SetAnswer(json answer);

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *data);

    void m_GetForecast();
    void Logic();

    void m_CreateRequest();
    void m_PutDataToFile(std::string buffer);
    void m_Parse();

};