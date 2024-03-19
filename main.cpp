#include "main.h"

int main(int argc,  char **argv)
{
    ifstream pass("..token.txt");
    string token;
    pass >> token;
    cout << token;
    pass.close();
    std::cout << "Hello, from aladin_practise_cur1!\n";

    GtkApplication *app;
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
