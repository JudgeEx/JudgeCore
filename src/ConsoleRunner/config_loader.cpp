#define _CRT_NONSTDC_NO_DEPRECATE
#include "config_loader.h"
#include <rapidjson/document.h>
using namespace rapidjson;
bool LoadConfig(const char *ConfigJSON)
{
    Document doc;
    doc.Parse(ConfigJSON);
    if (doc.HasParseError()) return false;
    config.Redirect = doc["Redirect"].GetBool();
    config.TimeLimit = doc["TimeLimit"].GetUint();
    config.MemoryLimit = doc["MemoryLimit"].GetInt();
    config.CommandLine = strdup(doc["CommandLine"].GetString());
    return true;
}
