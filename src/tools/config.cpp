#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "config.h"

map<string,string> ConfigManager::properties;
int ConfigManager::H_RES;
int ConfigManager::V_RES;
string ConfigManager::gamedataPath;

void ConfigManager::LoadConfigFile(char * fileName)
{
	ConfigManager::LoadConfigFile(string(fileName));
}
// Loads the configuration file into the properties map
void ConfigManager::LoadConfigFile(string fileName)
{
	std::ifstream configFile (fileName.c_str());
	while(configFile.good())
	{
		// ACHTUNG! We limit line width to 120 chars
		// Should be enough for all our conf needs...
		char lineRead [120];
		configFile.getline(lineRead,120);
		string lineString(lineRead);
		
		int eqPos = lineString.find_first_of('=');
		string name = lineString.substr(0,eqPos);	
		string value = lineString.substr(eqPos+1,lineString.size());
		
		pair<string,string> dataPair(name,value);
		properties.insert(dataPair);
				
		
	}
	configFile.close();
	LoadResolutionValues();
}

void ConfigManager::LoadResolutionValues()
{
	ConfigManager::H_RES = GetIntProperty("h_res");
	ConfigManager::V_RES = GetIntProperty("v_res");
    //FIXME this next line shouldn't be in a function named like this.
    ConfigManager::gamedataPath = GetStringProperty("gamedataPath");
}

string ConfigManager::GetStringProperty(string propertyName)
{
	return properties[string(propertyName)];
}

int ConfigManager::GetIntProperty(string propertyName)
{
	return atoi(properties[string(propertyName)].c_str());
}

int ConfigManager::GetIntProperty(char * propertyName)
{
	return ConfigManager::GetIntProperty(string(propertyName));
}

bool ConfigManager::GetBoolProperty(char * propertyName)
{
	return ConfigManager::GetBoolProperty(string(propertyName));
}
bool ConfigManager::GetBoolProperty(string propertyName)
{
	return (properties[string(propertyName)]=="true" )|| 
			(properties[string(propertyName)]=="True");
}
