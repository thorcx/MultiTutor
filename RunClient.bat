@echo on

rem UE����·��
set UE_Path="D:\UE4Source\UE_4.16\Engine\Binaries\Win64\UE4Editor.exe"
rem ��������·��
set ProjectPath="D:\UE418\MultiTutor\MultiTutor.uproject"

rem Server��IP
set ServerIP=192.168.5.26

set StartParams= %ServerIP% -game -log

rem ��������
%UE_Path% %ProjectPath% %StartParams%

exit /B 0