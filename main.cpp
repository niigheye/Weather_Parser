#include "main.h"
#include <stdio.h>

#include "curl/curl.h"
#pragma comment(lib, "curllib-bcb.lib") // Для C++Builder
// #pragma comment(lib,"curllib.lib")    // для VC++
int main(int argc, char **argv)
{

    system("chcp 1251>nul");
    ifstream pass("../token.txt");
    string token;
    string id = "524901";
    pass >> token;
    string request = "http://api.openweathermap.org/data/2.5/forecast?id=" + id + "&appid=" + token;
    cout << "My request is " << request << endl;
    cout << token;

    pass.close();

    //---------------------------------------------------------------------------
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << readBuffer << std::endl;
    }
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
