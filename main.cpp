#include "main.h"
int main(int argc, char **argv)
{
    ifstream pass("../token.txt");

    string token;
    string id = "524901";
    pass >> token;
    string request = "http://api.openweathermap.org/data/2.5/forecast?id=" + id + "&appid=" + token;
    cout << token;
    pass.close();

    CURL *curl;
    CURLcode res;

    GtkApplication *app;
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
