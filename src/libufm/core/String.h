#pragma once

#include <string>
#include <vector>

#include "../libufmdef.h"

METHOD String StringReplace(String subject, const String& search, const String& replace);
METHOD std::vector<String> StringSplit(String s, String delimiter);
METHOD const std::wstring AToWStr(const std::string& s);
