#include "String.h"

METHOD String StringReplace(String subject, const String& search, const String& replace)
{
    size_t pos = 0;

    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return subject;
}

METHOD std::vector<String> StringSplit(String s, String delimiter)
{
    std::vector<String> v;
    size_t pos = 0;
    String token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        v.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    v.push_back(s);

    return v;
}

METHOD std::wstring AToWStr(std::string s) {
    std::wstring ws;
    ws.assign(s.begin(), s.end());
    return ws;
}

METHOD std::string WToAStr(std::wstring s) {
    std::string str;
    str.assign(s.begin(), s.end());
    return str;
}
