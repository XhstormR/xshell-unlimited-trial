@ echo off
for /f "skip=2" %%i in ('"%~dp0%main.exe"') do (
    reg add "HKLM\SOFTWARE\Wow6432Node\NetSarang\Xftp\6" /f /v MagicCode /t REG_BINARY /d "%%i"
    reg add "HKLM\SOFTWARE\Wow6432Node\NetSarang\Xshell\6" /f /v MagicCode /t REG_BINARY /d "%%i"
)
pause
