#define _CRT_NONSTDC_NO_DEPRECATE
#include "make_output.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
using namespace rapidjson;
char *GetOutput(const LPPROCESS_RESULT lpResult)
{
    Document doc;
    doc.SetObject();
    auto &allocator = doc.GetAllocator();
    doc.AddMember("ReturnCode", lpResult->ReturnCode, allocator);
    doc.AddMember("ExecutionTime", lpResult->ExecutionTime, allocator);
    doc.AddMember("PeakWorkingSetSize", lpResult->PeakWorkingSetSize, allocator);
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    doc.Accept(writer);
    return strdup(buf.GetString());
}
