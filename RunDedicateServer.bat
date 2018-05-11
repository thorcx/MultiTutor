@echo off

rem UE引擎路径
set UE_Path="D:\UE4Source\UE_4.16\Engine\Binaries\Win64\UE4Editor.exe"
rem 启动工程路径
set ProjectPath="D:\UE418\MultiTutor\MultiTutor.uproject"

rem 启动地图
set StartMap=/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap

set StartParams=%StartMap% -server -log

rem 调用命令
%UE_Path% %ProjectPath% %StartParams%

exit /B 0

