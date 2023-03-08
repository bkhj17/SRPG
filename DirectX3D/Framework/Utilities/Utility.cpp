#include "Framework.h"

string Utility::ToString(wstring str)
{
    string temp;
    temp.assign(str.begin(), str.end());

    return temp;
}

wstring Utility::ToWString(string str)
{
    wstring temp;
    temp.assign(str.begin(), str.end());
    return temp;
}

vector<string> Utility::SplitString(string origin, const string& tok, bool includeLast)
{
    vector<string> result;

    size_t cutAt = 0;

    while((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    if (includeLast && origin.size() > 0)
        result.push_back(origin);

    return result;
}

bool Utility::ExistDirectory(string file)
{
    DWORD fileValue = GetFileAttributesA(file.c_str());
    bool temp = (fileValue != INVALID_FILE_ATTRIBUTES 
        && (fileValue & FILE_ATTRIBUTE_DIRECTORY));
    return temp;
}

wstring Utility::GetExtension(wstring file)
{
    size_t index = file.find_last_of('.');
    if (index == string::npos)
        return file;

    return file.substr(index + 1);
}

string Utility::GetFileName(string file)
{
    size_t index = file.find_last_of('/');
    if (index == string::npos)
        return file;

    return file.substr(index+1);
}

string Utility::GetFileNameWithoutExtension(string file)
{
    string fileName = GetFileName(file);
    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

void Utility::CreateFolders(string file)
{
    //폴더 생성기
    vector<string> folders = SplitString(file, "/", false);

    string temp = "";
    for (string folder : folders) {
        temp += folder + "/";
        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}
