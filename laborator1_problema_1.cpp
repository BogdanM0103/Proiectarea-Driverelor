/**
Să se implementeze o aplicaţie ce afişează valoarea variabilei ImagePath a
tuturor subcheilor din HKLM\SYSTEM\CurrentControlSet\Services
*/
 
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
 
int _tmain()
{
    // Setăm modul de ieșire al consolei pentru suport Unicode
    _setmode(_fileno(stdout), _O_U16TEXT);
 
    HKEY hServicesKey;
    // Deschidem cheia de registru HKLM\SYSTEM\CurrentControlSet\Services
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        _T("SYSTEM\\CurrentControlSet\\Services"),
        0,
        KEY_READ,
        &hServicesKey) != ERROR_SUCCESS)
    {
        _tprintf(_T("Nu s-a putut deschide cheia de registru.\n"));
        return 1;
    }
 
    DWORD index = 0;
    TCHAR subKeyName[256];
    DWORD subKeyNameSize = sizeof(subKeyName) / sizeof(TCHAR);
 
    while (true)
    {
        // Resetăm dimensiunea bufferului pentru fiecare iterație
        subKeyNameSize = sizeof(subKeyName) / sizeof(TCHAR);
 
        // Enumerăm subcheile
        LONG enumResult = RegEnumKeyEx(hServicesKey,
            index,
            subKeyName,
            &subKeyNameSize,
            NULL,
            NULL,
            NULL,
            NULL);
 
        if (enumResult == ERROR_NO_MORE_ITEMS)
            break;
        else if (enumResult != ERROR_SUCCESS)
        {
            _tprintf(_T("Eroare la enumerarea subcheilor.\n"));
            break;
        }
 
        // Deschidem subcheia curentă
        HKEY hSubKey;
        if (RegOpenKeyEx(hServicesKey,
            subKeyName,
            0,
            KEY_READ,
            &hSubKey) == ERROR_SUCCESS)
        {
            TCHAR imagePath[512];
            DWORD imagePathSize = sizeof(imagePath);
            // Citim valoarea "ImagePath"
            LONG queryResult = RegQueryValueEx(hSubKey,
                _T("ImagePath"),
                NULL,
                NULL,
                (LPBYTE)imagePath,
                &imagePathSize);
 
            if (queryResult == ERROR_SUCCESS)
            {
                _tprintf(_T("Service: %s - ImagePath: %s\n"), subKeyName, imagePath);
            }
            else
            {
                _tprintf(_T("Service: %s - ImagePath: (nu există)\n"), subKeyName);
            }
            RegCloseKey(hSubKey);
        }
        else
        {
            _tprintf(_T("Service: %s - Nu s-a putut deschide subcheia.\n"), subKeyName);
        }
        index++;
    }
 
    RegCloseKey(hServicesKey);
    _tprintf(_T("Apăsați Enter pentru a închide...\n"));
    getchar();
 
    return 0;
}