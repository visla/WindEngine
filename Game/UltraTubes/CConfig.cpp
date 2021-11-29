#include <CConfig.h>



String GetConfigFileName(StringParam BaseName)
{
	char IniFile[1000];
	GetCurrentDirectory(sizeof(IniFile), IniFile);
	int len;
	len = (int)strlen(IniFile);
	if (IniFile[len - 1] != '\\')
	{
		IniFile[len] = '\\';
		++len;
		IniFile[len] = '\0';
	}
	strcpy(&IniFile[len], BaseName.GetString());
	strcpy(&IniFile[strlen(IniFile)], ".ini");

	return (String)IniFile;
}



int GetConfigInt(StringParam ConfigParamName, int DefaultValue)
{
	char DefaultStrValue[100];
	sprintf(DefaultStrValue, "%d", DefaultValue);

	String StrValue;
	StrValue = GetConfigString(ConfigParamName, DefaultStrValue);

	int Value = 0;
	sscanf(StrValue.GetString(), "%d", &Value);

	return Value;
}



int GetConfigHexInt(StringParam ConfigParamName, int DefaultValue)
{
	char DefaultStrValue[100];
	sprintf(DefaultStrValue, "%.8X", DefaultValue);

	String StrValue;
	StrValue = GetConfigString(ConfigParamName, DefaultStrValue);

	int Value = 0;
	sscanf(StrValue.GetString(), "%8X", &Value);

	return Value;
}



float GetConfigFloat(StringParam ConfigParamName, float DefaultValue)
{
	char DefaultStrValue[100];
	sprintf(DefaultStrValue, "%.6f", DefaultValue);

	String StrValue;
	StrValue = GetConfigString(ConfigParamName, DefaultStrValue);

	float Value = 0.0f;
	sscanf(StrValue.GetString(), "%f", &Value);

	return Value;
}



String GetConfigString(StringParam ConfigParamName, StringParam DefaultValue)
{
	String ParamName = ConfigParamName;
	String IniFileName = ParamName.Extract('.', 0);
	String SectionName = ParamName.Extract('.', 1);
	String KeyName = ParamName.Extract('.', 2);
	String IniFile = GetConfigFileName(IniFileName);

	char StrValue[1000];
	int len = GetPrivateProfileString(SectionName.GetString(), KeyName.GetString(), DefaultValue, StrValue, sizeof(StrValue), IniFile.GetString());

	if (len == 0)
		return DefaultValue;
	else
		return String(StrValue);
}
