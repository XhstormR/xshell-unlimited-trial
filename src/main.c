#define _WIN32_WINNT _WIN32_WINNT_WIN8

#include <stdio.h>
#include <windows.h>

static struct _SYSTEMTIME systemtime;

static void initDate(unsigned short *date) {
  GetLocalTime(&systemtime);
  date[0] = systemtime.wYear;
  date[1] = systemtime.wMonth;
  date[2] = systemtime.wDay;
}

static void printHEX(unsigned char *data, int len) {
  for (int i = 0; i < len; i++) {
    printf("%02X", data[i]);
  }
}

int main() {
  HMODULE hMod = LoadLibrary("nslicense.dll");
  typedef int (*PFN1)(void *, void *);
  PFN1 pfn1 = (PFN1)GetProcAddress(hMod, "NSLICENSE_DateToMagicCode");

  unsigned short date[3] = {0};
  unsigned char code[22] = {0};

  initDate(date);

  pfn1(date, code);

  printHEX(code, 22);
  printf("\n");
  printHEX(code, 16);
  printHEX((unsigned char *)&systemtime, 4);
  printHEX((unsigned char *)&systemtime + 6, 2);
  printf("\n");

  FreeLibrary(hMod);

  HKEY hkey;
  RegCreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\NetSarang\\Xshell\\6", &hkey);
  RegSetValueEx(hkey, "MagicCode", 0, REG_BINARY, code, 22);
  RegCloseKey(hkey);

  RegDeleteTreeA(HKEY_CURRENT_USER, "Software\\Microsoft\\NetLicense");

  return 0;
}

BOOL WINAPI DllMain(HANDLE hModule, DWORD ul_reason_for_call,
                    LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    main();
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

// DLL 劫持自动激活需要编译为 32 位 dwmapi.dll

// https://docs.microsoft.com/zh-cn/windows/win32/WinProg/using-the-windows-headers
