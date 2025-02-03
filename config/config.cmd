::@echo off
::
::  �벻Ҫֱ��ʹ�ô˽ű���Ӧ��ʹ��VC-LTL helper for nmake.cmd
::

call:InitMuiStrings


if /i "%VC_LTL_Helper_Load%" == "true" goto:eof

set InternalSupportLTL=%SupportLTL%
if /i "%InternalSupportLTL%" == "false" goto:eof
if /i "%InternalSupportLTL%" == "" (set InternalSupportLTL=true)

set "VC_LTL_Root=%~dp0"
set "VC_LTL_Root=%VC_LTL_Root:~0,-7%"

if "%INCLUDE%" == "" echo %ERROR_VC_LTL_CANNOT_FOUND_INCLUDE_ENV%&&goto:eof

if "%LIB%" == "" echo %ERROR_VC_LTL_CANNOT_FOUND_LIB_ENV%&&goto:eof



if "%VisualStudioVersion%" == "14.0" goto CheckPlatform
if "%VisualStudioVersion%" == "15.0" goto CheckPlatform
if "%VisualStudioVersion%" == "16.0" goto CheckPlatform
if "%VisualStudioVersion%" == "17.0" goto CheckPlatform

echo %ERROR_VC_LTL_NOT_SUPPORT_PLATFORM_TOOLSET%&&goto:eof

:CheckPlatform

if /i "%Platform%" == "" goto Start_VC_LTL

if /i "%Platform%" == "x86" goto Start_VC_LTL

if /i "%Platform%" == "x64" goto Start_VC_LTL

if /i "%Platform%" == "arm" goto Start_VC_LTL

if /i "%Platform%" == "arm64" goto Start_VC_LTL

echo %ERROR_VC_LTL_NOT_SUPPORT_PLATFORM%

goto:eof


:Start_VC_LTL

::VC-LTL���İ汾�ţ�����4.X��������3.X����ֵ�������ڼ������жϡ�
set LTL_CoreVersion=5

set VC_LTL_Helper_Load=true

set LTLPlatform=%Platform%

if "%LTLPlatform%" == "" set LTLPlatform=Win32
if /i "%LTLPlatform%" == "x86" set LTLPlatform=Win32
if /i "%LTLPlatform%" == "arm64" set InternalSupportLTL=ucrt

call:FoundBestTargetPlatform

if /i "%InternalSupportLTL%" == "true" if "%InternalLTLCRTVersion:~0,3%"=="10." (set InternalSupportLTL=ucrt)
if /i "%InternalSupportLTL%" == "true" (set InternalSupportLTL=msvcrt)

if /i "%InternalSupportLTL%" == "msvcrt" if "%InternalLTLCRTVersion:~0,3%"=="10." (set InternalLTLCRTVersion=6.2.9200.0)
if /i "%InternalSupportLTL%" == "ucrt" if not "%InternalLTLCRTVersion:~0,3%"=="10." (set InternalLTLCRTVersion=10.0.10240.0)

if not exist "%VC_LTL_Root%TargetPlatform\%InternalLTLCRTVersion%\lib\%LTLPlatform%" echo %ERROR_VC_LTL_FILE_MISSING%&&goto:eof



echo #######################################################################
echo #                                                                     #
echo # 8b           d8  ,ad8888ba,         88     888888888888 88          #
echo # `8b         d8' d8"'    `"8b        88          88      88          #
echo #  `8b       d8' d8'                  88          88      88          #
echo #   `8b     d8'  88                   88          88      88          #
echo #    `8b   d8'   88          aaaaaaaa 88          88      88          #
echo #     `8b d8'    Y8,         """""""" 88          88      88          #
echo #      `888'      Y8a.    .a8P        88          88      88          #
echo #       `8'        `"Y8888Y"'         88888888888 88      88888888888 #
echo #                                                                     #
echo #######################################################################

echo VC-LTL Path : %VC_LTL_Root%
echo VC Tools Version : %VCToolsVersion%
echo %InternalSupportLTL% Mode : %InternalLTLCRTVersion%
echo Platform : %LTLPlatform%



::�޸�Include
set INCLUDE=%VC_LTL_Root%TargetPlatform\header;%VC_LTL_Root%TargetPlatform\%InternalLTLCRTVersion%\header;%INCLUDE%
set LIB=%VC_LTL_Root%TargetPlatform\%InternalLTLCRTVersion%\lib\%LTLPlatform%;%LIB%

goto:eof


::�������TargetPlatform
:FoundBestTargetPlatform

set InternalLTLCRTVersion=%WindowsTargetPlatformMinVersion%

if "%InternalLTLCRTVersion%" == "" goto FoundBestTargetPlatformDefault

for /f "tokens=3 delims=." %%i in ('echo %InternalLTLCRTVersion%') do set LTLWindowsTargetPlatformMinVersionBuild=%%i

if "%LTLWindowsTargetPlatformMinVersionBuild%" == "" goto FoundBestTargetPlatformDefault

if %LTLWindowsTargetPlatformMinVersionBuild% GEQ 19041 set InternalLTLCRTVersion=10.0.19041.0&&goto:eof

if %LTLWindowsTargetPlatformMinVersionBuild% GEQ 10240 set InternalLTLCRTVersion=10.0.10240.0&&goto:eof
if /i "%LTLPlatform%" == "arm64" set InternalLTLCRTVersion=10.0.10240.0&&goto:eof

if %LTLWindowsTargetPlatformMinVersionBuild% GEQ 9200 set InternalLTLCRTVersion=6.2.9200.0&&goto:eof
if /i "%LTLPlatform%" == "arm" set InternalLTLCRTVersion=6.2.9200.0&&goto:eof

if %LTLWindowsTargetPlatformMinVersionBuild% GEQ 6000 set InternalLTLCRTVersion=6.0.6000.0&&goto:eof

if /i "%LTLPlatform%" == "x64" set InternalLTLCRTVersion=5.2.3790.0&&goto:eof

::����
set InternalLTLCRTVersion=5.1.2600.0

goto:eof

:FoundBestTargetPlatformDefault

if /i "%LTLPlatform%" == "arm64" set InternalLTLCRTVersion=10.0.10240.0&&goto:eof

if /i "%LTLPlatform%" == "arm" set InternalLTLCRTVersion=6.2.9200.0&&goto:eof

::������ǰ��XPģʽ
if /i "%SupportWinXP%" NEQ "true" set InternalLTLCRTVersion=6.0.6000.0&&goto:eof

if /i "%LTLPlatform%" == "x64" set InternalLTLCRTVersion=5.2.3790.0&&goto:eof

set InternalLTLCRTVersion=5.1.2600.0

goto:eof

::��ȡ��ǰ��Ծ�Ĵ���ҳ������ʶ�����Ի���
:GetCodePage
for /f "tokens=*" %%s in ('chcp') do set __codepage__=%%s


:TryFindCodePage
::Ϊ�գ���ô����ѭ��
if "%__codepage__%" == "" goto:eof

echo %__codepage__% | findstr "^[0-9]" > nul

::ƥ�䵽ֻ�������ֵģ�˵ʶ��ɹ����˳�ѭ��
if %ERRORLEVEL% == 0 goto:eof

::ɾ����һ���ַ���
set "__codepage__=%__codepage__:~1%"

goto:TryFindCodePage

goto:eof


::��ʼ�����������Դ
:InitMuiStrings

call:GetCodePage

set __LangID__=1033
if "%__codepage__%" == "936" (set __LangID__=2052)


call "%~dp0%__LangID__%\config.strings.cmd"


goto:eof
