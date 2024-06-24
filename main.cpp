#include <iostream>
#include<fstream>
#include <windows.h>
#include<vector>
#include<string>
#include <thread> 
#include <chrono>

using namespace std;

wstring stringTowstring(string str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    if (len == 0)
        return wstring(L"");
    wchar_t* wct = new wchar_t[len];
    if (!wct)
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wct, len);
    wstring wstr(wct);
    delete[] wct;
    wct = NULL;
    return wstr;
}


vector<string> InputNames(const string& fileName) {
    ifstream file(fileName);
    vector<string> result;
    if (!file.is_open()) { return result; }
    string line;
    while (getline(file, line)) {  
        //cout << line << endl;
        result.push_back(line);
    }
    return result;
}
void executeCmd(const wstring& name) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
   // TCHAR cmd[] = TEXT("notepad");
    if (!CreateProcess(NULL,   // No module name (use command line)
        const_cast<LPWSTR>(name.c_str()),  // Command line
        NULL,                  // Process handle not inheritable
        NULL,                  // Thread handle not inheritable
        FALSE,                 // Set handle inheritance to FALSE
        0,                     // No creation flags
        NULL,                  // Use parent's environment block
        NULL,                  // Use parent's starting directory 
        &si,                   // Pointer to STARTUPINFO structure
        &pi)                   // Pointer to PROCESS_INFORMATION structure
        ) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
        return ;
    }

    // Wait until child process exits. 
    //WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

}
int main() {
    //startSoftWare(L"notepad");
    vector<string> commands = InputNames("D:/DailySoftWares.txt");
    for (const string& cmd : commands) {
        wstring wcmd = stringTowstring(cmd);
        cout << "current starting software is：  " << cmd<<endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
        executeCmd(wcmd);
    }
    return 0;
}