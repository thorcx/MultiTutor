@echo on

rem UE引擎路径
set UE_Path="D:\UE4Source\UE_4.16\Engine\Binaries\Win64\UE4Editor.exe"
rem 启动工程路径
set ProjectPath="D:\UE418\MultiTutor\MultiTutor.uproject"

rem Server的IP
set ServerIP=192.168.5.26

set StartParams= %ServerIP% -game -log

rem 调用命令
%UE_Path% %ProjectPath% %StartParams%

exit /B 0