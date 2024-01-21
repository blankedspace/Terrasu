#include <string>
#include <codecvt>

#pragma once
inline std::wstring s2ws(const std::string& str)
{
  
    // Convert char array to wstring
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
    std::wstring wideString = converter.from_bytes(str);

    return wideString;
}
