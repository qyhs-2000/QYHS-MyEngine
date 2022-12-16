#pragma once
#include <string>
class Application
{
public:
	static const std::string& dataPath() { return data_Path; }
	static void setDataPath(std::string dataPath)
	{
		data_Path = dataPath;
	}
private:
	static std::string data_Path;

};