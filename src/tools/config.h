#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <map>
using namespace std;
class ConfigManager
{
	private:
	static std::map<string,string> properties;

		static void LoadResolutionValues();	
	public:
		static int H_RES;
		static int V_RES;
        static string gamedataPath;
        
		static void LoadConfigFile(string fileName);
		static void LoadConfigFile(char * fileName);
		static string  GetStringProperty(string propertyName);
		static int GetIntProperty(char * propertyName);
		static int GetIntProperty(string propertyName);
		static bool GetBoolProperty(char * propertyName);
		static bool GetBoolProperty(string propertyName);

};

#endif
