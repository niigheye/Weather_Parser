#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <stdio.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

namespace settings
{
  string request_path = "../src/request.json";
  string token_path = "../src/token.txt";
};

namespace request
{
  string city;
  string token;
  string units;
  string local;
}

static void print_hello(GtkWidget *widget, gpointer data)
{
  g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  /* create a new window, and set its title */
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new();

  /* Pack the container in the window */
  gtk_window_set_child(GTK_WINDOW(window), grid);

  button = gtk_button_new_with_label("Button 1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("Button 2");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

  button = gtk_button_new_with_label("Quit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

  gtk_window_present(GTK_WINDOW(window));
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *data)
{
  return fwrite(ptr, size, nmemb, data);
}

void createRequest(string &request)
{

  ifstream pass(settings::token_path);
  if (!pass)
  {
    cout << "Error: can not open the " << settings::token_path << endl;
    return;
  }
  pass >> request::token;
  request::city = "Moscow";
  request::units = "metric";
  request = string("https://api.openweathermap.org/data/2.5/forecast?"
                   "q=" +
                   request::city + "&units=" + request::units + "&lang=" +
                   request::local +
                   "&appid=" +
                   request::token);
  cout << "My request is " << request << endl;
  cout << request::token;

  pass.close();
}