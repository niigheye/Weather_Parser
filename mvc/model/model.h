class Model
{
public:
	virtual void Logic() = 0;
    
};
namespace settings
{
  std::string request_path = "../src/request.json";
  std::string token_path = "../src/token.txt";
};