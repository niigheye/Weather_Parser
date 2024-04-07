#include "main.h"

int main(int argc, char **argv)
{
    system("chcp 1251>nul");

    string request;
    createRequest(request);
    //---------------------------------------------------------------------------
    CURL *curl;
    CURLcode res;
    string buffer;
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, true);
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str()); // баг с выгрузкой токена из файла
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cout << "Error #" << res << " " << curl_easy_strerror(res) << std::endl;
            return -1;
        }
        curl_easy_cleanup(curl);
        putDataToFile(buffer);
    }
    else
    {
        cout << "Error: adding curl handler";
        return -1;
    }

    //---------------------------------------------------------------------------
    json data = json::parse(buffer);
    //---------------------------------------------------------------------------


    GtkApplication *app;
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
