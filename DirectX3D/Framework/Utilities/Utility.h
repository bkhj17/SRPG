#pragma once

namespace Utility
{
    string ToString(wstring str);
    wstring ToWString(string str);

    vector<string> SplitString(string origin, const string& tok);

    //확장자 반환 함수
    wstring GetExtension(wstring file);
}