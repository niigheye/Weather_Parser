#pragma once
#include "weatherForecastModel.h"
#include "controller.h"
#include <iostream>

class AppController : public Controller
{
	WeatherForecastModel* model;

public:
	AppController(WeatherForecastModel* model)
	{
		this->model = model;
	}

	void GetInfo()
	{
		// model->SetCity("odessa");
		// do
		// {
		// 	std::string city;
		// 	std::cout << "Enter the name of another city (kyiv, chernivtsi): ";
		// 	std::cin >> city;
		// 	model->SetCity(city);
		// } while (true);
	}
};