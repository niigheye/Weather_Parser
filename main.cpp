#include "main.h"
int main(int argc, char **argv)
{
    system("chcp 1251>nul");
    string request;
    createRequest(request);
    //---------------------------------------------------------------------------
    FILE *request_file = fopen(settings::request_path.c_str(), "w");
    if (!request_file)
    {
        cout << "Error: can not open the " << settings::request_path << endl;
        return 0;
    }
    //---------------------------------------------------------------------------
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/forecast?id=547560&appid=" + request::token); // баг с выгрузкой токена из файла
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, request_file);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cout << "Error #" << res << " " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
    }
    else
    {
        cout << "Error: adding curl handler";
        return 0;
    }

    //---------------------------------------------------------------------------
    json data = json::parse(request_file);

    //---------------------------------------------------------------------------

    return 0;

    // CURL *curl;
    // CURLcode res;

    // curl = curl_easy_init();
    // if (curl)
    // {
    //     /* First set the URL that is about to receive our POST. This URL can
    //        just as well be a https:// URL if that is what should receive the
    //        data. */
    //     curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");

    //     /* Perform the request, res will get the return code */
    //     res = curl_easy_perform(curl);

    //     /* always cleanup */
    //     curl_easy_cleanup(curl);
    // }
    // return 0;
    // GtkApplication *app;
    // int status;
    // app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    // g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    // status = g_application_run(G_APPLICATION(app), argc, argv);
    // g_object_unref(app);

    // return status;
}
