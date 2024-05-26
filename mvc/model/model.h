#pragma once
#include <string>
class Model
{
public:
	virtual void Logic() = 0;
    
};
namespace settings
{
  inline std::string request_path = "../src/request.json";
  inline std::string token_path = "../src/token.txt";
};