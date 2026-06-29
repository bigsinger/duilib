@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul

rem 本脚本用于在本机 VS2026 环境下编译 DuiLib 和 DuiDesigner。
rem 直接双击会显示菜单；也可以使用命令行参数：
rem   build_all.bat designer [Debug/Release] [Win32/x64]
rem   build_all.bat duilib [Debug/Release/lib_debug/lib_release] [Win32/x64]
rem   build_all.bat all

set "ROOT=%~dp0.."
for %%I in ("%ROOT%") do set "ROOT=%%~fI"
set "MSBUILD=D:\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

if not exist "%MSBUILD%" (
    echo 未找到 MSBuild：%MSBUILD%
    echo 请确认 VS2026 已安装，或修改本脚本中的 MSBUILD 路径。
    pause
    exit /b 1
)

if /i "%~1"=="designer" goto cmd_designer
if /i "%~1"=="duilib" goto cmd_duilib
if /i "%~1"=="all" goto cmd_all
if not "%~1"=="" goto usage

:menu
echo.
echo ===== DuiLib 自动编译 =====
echo 1. 编译 DuiDesigner Release Win32（默认验证）
echo 2. 编译 DuiLib DLL Release Win32
echo 3. 编译 DuiLib DLL Debug Win32
echo 4. 编译 DuiLib 静态库 lib_release Win32
echo 5. 编译 DuiLib 静态库 lib_debug Win32
echo 6. 编译全部 Release Win32
echo 7. 编译 DuiLib 全矩阵（Win32/x64）
echo 0. 退出
set /p "CHOICE=请选择："
if "%CHOICE%"=="1" (
    call :build_designer Release Win32
    goto end
)
if "%CHOICE%"=="2" (
    call :build_duilib Release Win32
    goto end
)
if "%CHOICE%"=="3" (
    call :build_duilib Debug Win32
    goto end
)
if "%CHOICE%"=="4" (
    call :build_duilib lib_release Win32
    goto end
)
if "%CHOICE%"=="5" (
    call :build_duilib lib_debug Win32
    goto end
)
if "%CHOICE%"=="6" (
    call :build_all_release_win32
    goto end
)
if "%CHOICE%"=="7" (
    call :build_duilib_matrix
    goto end
)
if "%CHOICE%"=="0" exit /b 0
echo 无效选择。
goto menu

:cmd_designer
set "CFG=%~2"
set "PLAT=%~3"
if "%CFG%"=="" set "CFG=Release"
if "%PLAT%"=="" set "PLAT=Win32"
call :build_designer "%CFG%" "%PLAT%"
goto end

:cmd_duilib
set "CFG=%~2"
set "PLAT=%~3"
if "%CFG%"=="" set "CFG=Release"
if "%PLAT%"=="" set "PLAT=Win32"
call :build_duilib "%CFG%" "%PLAT%"
goto end

:cmd_all
call :build_all_release_win32
goto end

:build_all_release_win32
call :build_duilib Release Win32
if errorlevel 1 exit /b 1
call :build_designer Release Win32
if errorlevel 1 exit /b 1
exit /b 0

:build_duilib_matrix
for %%C in (Debug Release lib_debug lib_release) do (
    for %%P in (Win32 x64) do (
        call :build_duilib %%C %%P
        if errorlevel 1 exit /b 1
    )
)
exit /b 0

:build_designer
echo.
echo 编译 DuiDesigner %~1/%~2
"%MSBUILD%" "%ROOT%\DuiDesigner.sln" /m:1 /p:Configuration=%~1 /p:Platform=%~2 /p:PreferredToolArchitecture=x64 /v:minimal
exit /b %ERRORLEVEL%

:build_duilib
echo.
echo 编译 DuiLib %~1/%~2
"%MSBUILD%" "%ROOT%\DuiLib.sln" /m:1 /p:Configuration=%~1 /p:Platform=%~2 /p:PreferredToolArchitecture=x64 /v:minimal
exit /b %ERRORLEVEL%

:usage
echo 用法：
echo   build_all.bat designer [Debug/Release] [Win32/x64]
echo   build_all.bat duilib [Debug/Release/lib_debug/lib_release] [Win32/x64]
echo   build_all.bat all
exit /b 1

:end
set "RESULT=%ERRORLEVEL%"
echo.
if "%RESULT%"=="0" echo 编译完成。
if not "%RESULT%"=="0" echo 编译失败，错误码：%RESULT%
if "%DUILIB_BUILD_NO_PAUSE%"=="1" exit /b %RESULT%
pause
exit /b %RESULT%
