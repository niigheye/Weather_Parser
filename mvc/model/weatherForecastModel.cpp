#include "weatherForecastModel.h"
// ????????? ??????? ??????? Parse ? ?? ???????????

void WeatherForecastModel::m_GetForecast()
{
    if (m_GetCity() != "") // need to validate the city
    {
        std::string buffer = "";
        m_CreateRequest();
        m_DoRequest(buffer);
        m_PutDataToFile(buffer);
        m_SetAnswer(json::parse(buffer));
    }
    else
    {
        std::cout << "the entry is empty";
    }
}

std::string WeatherForecastModel::m_GetCity()
{
    return _city;
}

std::string WeatherForecastModel::m_GetState()
{
    return _state;
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

int WeatherForecastModel::m_GetDays()
{
    return _days;
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

void WeatherForecastModel::m_SetDays(int days)
{
    _days = days;
}

void WeatherForecastModel::m_SetState(std::string state)
{
    _state = state;
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
    if (answer.at("cod") != "200")
    {
        std::cout << answer.at("message");
    }
    _answer = answer;
}

void WeatherForecastModel::Logic()
{
    m_GetForecast();
}

void WeatherForecastModel::m_CreateRequest()
{
    m_ParseToken();
    // std::cout << "\ni set token to " << m_GetToken() << std::endl;
    _request = std::string("api.openweathermap.org/data/2.5/forecast?"

                           "q=" +
                           m_GetCity() +
                           "," + m_GetState() +
                           "&units=" + m_GetUnits() +
                           "&cnt=" + std::to_string(m_GetDays() * 8) + // надо умножить спаршенное число на 8n + 1
                           "&lang=" + m_GetLocal() +
                           "&appid=" + m_GetToken());
}

std::string WeatherForecastModel::m_DoRequest(std::string &buffer)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, true);
        curl_easy_setopt(curl, CURLOPT_URL, m_GetRequest().c_str()); // ��� � ��������� ������ �� �����
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cout << "Error #" << res << " " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        std::cout << "i do request with\n";
        std::cout << "city: " << m_GetCity() << std::endl;
        std::cout << "local: " << m_GetLocal() << std::endl;
        std::cout << "token: " << m_GetToken() << std::endl;
        std::cout << "units: " << m_GetUnits() << std::endl;
        std::cout << "request: " << m_GetRequest() << std::endl;
    }
    else
    {
        std::cout << "Error: adding curl handler";
    }
    return buffer;
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

void WeatherForecastModel::m_ParseToken()
{
    std::ifstream pass(settings::token_path);
    if (!pass)
    {
        std::cout << "Error: can not open the " << settings::token_path << std::endl;
        return;
    }
    std::string tmp;
    pass >> tmp;
    m_SetToken(tmp);
    pass.close();
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

void WeatherForecastModel::ParseFileToVector(std::vector<std::string> &myvec, std::string path)
{
    std::ifstream in(path);
    std::string tmp;
    if (!in.is_open())
    {
        std::cout << "i can't open the cities.txt file((\n";
    }

    while (std::getline(in, tmp))
    {
        myvec.push_back(tmp);
    }
    in.close();
}

GtkTreeModel *WeatherForecastModel::CreateCompletionModelCity()
{
    std::vector<std::string> strings;
    ParseFileToVector(strings, "../src/cities.txt");

    GtkListStore *store;
    store = gtk_list_store_new(1, G_TYPE_STRING);
    FillGtkTreeCity(store, strings);
    return GTK_TREE_MODEL(store);
}

GtkListStore *WeatherForecastModel::CreateListStorePeriod()
{
    GtkListStore *store;
    store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    FillGtkTreePeriod(store);
    return store; //
}

void WeatherForecastModel::FillGtkTreeCity(GtkListStore *store, std::vector<std::string> myvector)
{
    GtkTreeIter iter;
    for (const auto &str : myvector)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, str.c_str(), -1);
    }
}

void WeatherForecastModel::FillGtkTreePeriod(GtkListStore *store)
{
    gtk_list_store_insert_with_values(store, NULL, -1, 1, "1 day", -1);
    gtk_list_store_insert_with_values(store, NULL, -1, 1, "3 days", -1);
    gtk_list_store_insert_with_values(store, NULL, -1, 1, "5 days", -1);
}
