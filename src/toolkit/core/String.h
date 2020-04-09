#pragma once

#include <string>
#include <vector>

#include "../wfmtkdef.h"

METHOD String StringReplace(String subject, const String& search, const String& replace);
METHOD std::vector<String> StringSplit(String s, String delimiter);
METHOD std::wstring AToWStr(std::string s);
METHOD std::string WToAStr(std::wstring s);
