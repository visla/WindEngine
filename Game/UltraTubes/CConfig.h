#include <CString.h>

String GetConfigFileName(StringParam BaseName);
int GetConfigInt(StringParam ConfigParamName, int DefaultValue);
int GetConfigHexInt(StringParam ConfigParamName, int DefaultValue);
float GetConfigFloat(StringParam ConfigParamName, float DefaultValue);
String GetConfigString(StringParam ConfigParamName, StringParam DefaultValue);
