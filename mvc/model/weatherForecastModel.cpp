#include "weatherForecastModel.h"
// ????????? ??????? ??????? Parse ? ?? ???????????

void WeatherForecastModel::m_GetForecast()
{
}

std::string WeatherForecastModel::m_GetCity()
{
    return _city;
}

std::string WeatherForecastModel::m_GetToken()
{
    return _token;
}

std::string WeatherForecastModel::m_GetUnits()
{
    return _units;
}

std::string WeatherForecastModel::m_GetLocal()
{
    return _local;
}

std::string WeatherForecastModel::m_GetRequest()
{
    return _request;
}

json WeatherForecastModel::m_GetAnswer()
{
    return _answer;
}

void WeatherForecastModel::m_SetCity(std::string city)
{
    _city = city;
}

void WeatherForecastModel::m_SetToken(std::string token)
{
    _token = token;
}

void WeatherForecastModel::m_SetUnits(std::string units)
{
    _units = units;
}

void WeatherForecastModel::m_SetLocal(std::string local)
{
    _local = local;
}

void WeatherForecastModel::m_SetRequest(std::string request)
{
    _request = request;
}

void WeatherForecastModel::m_SetAnswer(json answer)
{
    _answer = answer;
}

void WeatherForecastModel::Logic()
{
    m_GetForecast();
}

void WeatherForecastModel::m_CreateRequest()
{
    std::ifstream pass(settings::token_path);
    if (!pass)
    {
        std::cout << "Error: can not open the " << settings::token_path << std::endl;
        return;
    }
    pass >> this->_token; // ????? ???? ???????? ? ???????????? ????
    _request = std::string("api.openweathermap.org/data/2.5/forecast?"
                           "q=" +
                           this->m_GetCity() +
                           "&units=" + this->m_GetUnits() +
                           "&lang=" + this->m_GetLocal() +
                           "&appid=" + this->m_GetToken());
    pass.close();
}

void WeatherForecastModel::m_DoRequest()
{
}

void WeatherForecastModel::m_PutDataToFile(std::string buffer)
{
    std::ofstream request_file(settings::request_path.c_str());
    if (!request_file)
    {
        std::cout << "Error: can not open the " << settings::request_path << std::endl;
        return;
    }
    request_file << buffer;
    request_file.close();
}

void WeatherForecastModel::m_Parse()
{
    CURL *curl;
    CURLcode res;
    std::string buffer;
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, true);
        curl_easy_setopt(curl, CURLOPT_URL, m_GetRequest().c_str()); // баг с выгрузкой токена из файла
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cout << "Error #" << res << " " << curl_easy_strerror(res) << std::endl;
            return;
        }
        curl_easy_cleanup(curl);
        m_PutDataToFile(buffer);
        m_SetAnswer(json::parse(buffer));
    }
    else
    {
        std::cout << "Error: adding curl handler";
        return;
    }
}

size_t WeatherForecastModel::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

size_t WeatherForecastModel::write_data(char *ptr, size_t size, size_t nmemb, FILE *data)
{
    return fwrite(ptr, size, nmemb, data);
}

GtkTreeModel *WeatherForecastModel::create_completion_model(void)
{
    std::ifstream in("src/cities.txt");
    std::string tmp;
    std::vector<std::string> strings;  

    while (std::getline(in, tmp))
    {
        strings.push_back(tmp); 
    }
    in.close();

    GtkListStore *store;
    GtkTreeIter iter;
    store = gtk_list_store_new(1, G_TYPE_STRING);
    
    for (const auto& str : strings) 
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, str.c_str(), -1);  
    }

    return GTK_TREE_MODEL(store);
}