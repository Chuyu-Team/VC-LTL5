@echo off

::
::  VC-LTL�Զ����������ã������㽫���ļ��������Ƶ���Ĺ�����ʹ�ã����ļ����Զ�ʶ��ǰ�����Ƿ����VC-LTL�������Զ�Ӧ�á�
::
::  ʹ�÷�����
::    1. ���� vcvars32.bat/vcvars64.bat Ȼ��ִ�д˽ű���������ɺ��Զ��޸�include�Լ�lib��������������nmake�Լ���cl�û�����VC-LTL��
::
::  VC-LTLĬ������˳��
::	  1. ��VC-LTL helper for nmake.cmd�����ڸ�Ŀ¼���� %~dp0
::	  2. ��VC-LTL helper for nmake.cmd�����ڸ�Ŀ¼�µ�VC-LTLĿ¼���� %~dp0VC-LTL
::	  3. ��VC-LTL helper for nmake.cmd�����ڸ�Ŀ¼���� %~dp0..
::	  3. ��VC-LTL helper for nmake.cmd�����ڸ�Ŀ¼�µ�VC-LTLĿ¼���� %~dp0..\VC-LTL
::    4. ��ǰĿ¼���� %cd%
::    5. ��ǰĿ¼�µ�VC-LTLĿ¼���� %cd%\VC-LTL
::    6. ��ǰĿ¼�ĸ�Ŀ¼�µ�VC-LTLĿ¼���� %cd%\..\VC-LTL
::    7. ע���HKEY_CURRENT_USER\Code\VC-LTL@Root
::
::  ��VC-LTL��������һ��λ�ü��ɣ�VC-LTL���ܱ��Զ����á�
::
::  ������Ĭ������˳������������޸Ĵ��ļ�����Ҳ����ֱ��ָ��%VC_LTL_Root%���������������Ե�ȥ����VC-LTL��
::




:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::VC-LTL����::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::������С����ϵͳ�汾��Ŀǰ���ð汾Ϊ 5.1.2600.0    6.0.6000.0��Ĭ�ϣ�    6.2.9200.0     10.0.10240.0    10.0.19041.0
::ע�⣺VC-LTL����YY-Thunks����������޷���������ϵͳ�������Ҫ֧��Windows XP����ֵ����Ϊ5.1.2600.0��
::set WindowsTargetPlatformMinVersion=5.1.2600.0

::VC-LTLʹ�õ�CRTģʽ��SupportLTL����ֵΪ��
::  * false������VC_LTL
::  * true��Ĭ��ֵ����VC-LTL�Զ���Ӧ������С���ݰ汾>=10.0ʱʹ��ucrtģʽ������ϵͳʹ��msvcrtģʽ��
::  * msvcrt��ʹ��msvcrt.dll��ΪCRT��ע�⣺msvcrtģʽ���ܲ���ȫ֧������ucrt���¹��ܡ�����setloacl��֧��UTF8��
::  * ucrt��ʹ��ucrtbase.dll��ΪCRT��ע�⣺����ϵͳ������Ҫ����VC-LTL.Redist.Dlls.zip����лmsvcr14x��Ŀ�ṩ����XPϵͳ��ucrtbase.dll�� 
::�����Ҫ����XPʱҲʹ��ucrt����ָ�� SupportLTL=ucrt��
::set SupportLTL=ucrt

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::





if "%VC_LTL_Root%" NEQ "" goto LoadConfig

::����VC-LTLĿ¼

::�ű��ļ���Ŀ¼����VC-LTL��
if exist "%~dp0_msvcrt.h" set VC_LTL_Root=%~dp0&& goto LoadConfig

::�ű��ļ���Ŀ¼�´���VC-LTL��
if exist "%~dp0VC-LTL\_msvcrt.h" set VC_LTL_Root=%~dp0VC-LTL&& goto LoadConfig

::�ű��ļ���Ŀ¼����VC-LTL��
if exist "%~dp0..\_msvcrt.h" set VC_LTL_Root=%~dp0..&& goto LoadConfig

::�ű��ļ���Ŀ¼����VC-LTL��
if exist "%~dp0..\VC-LTL\_msvcrt.h" set VC_LTL_Root=%~dp0..\VC-LTL&& goto LoadConfig

::��ǰ��Ŀ¼����VC-LTL��
if exist "%cd%\_msvcrt.h" set VC_LTL_Root=%cd%&& goto LoadConfig

::��ǰ��Ŀ¼����VC-LTL��
if exist "%cd%\VC-LTL\_msvcrt.h" set VC_LTL_Root=%cd%\VC-LTL&& goto LoadConfig

::��ǰ��Ŀ¼����VC-LTL��
if exist "%cd%\..\VC-LTL\_msvcrt.h" set VC_LTL_Root=%cd%\..\VC-LTL&& goto LoadConfig


::��ȡע��� HKCU\Code\VC-LTL@Root
for /f "tokens=1,2*" %%i in ('reg query "HKCU\Code\VC-LTL" /v Root ') do set VC_LTL_Root=%%k

if "%VC_LTL_Root%" == "" goto:eof


:LoadConfig

call "%VC_LTL_Root%\config\config.cmd"
