@echo off

rem UE����·��
set UE_Path="D:\UE4Source\UE_4.16\Engine\Binaries\Win64\UE4Editor.exe"
rem ��������·��
set ProjectPath="D:\UE418\MultiTutor\MultiTutor.uproject"

rem ������ͼ
set StartMap=/Game/Maps/Lobby

set StartParams=%StartMap% -game -log

rem ��������
%UE_Path% %ProjectPath% %StartParams%

exit /B 0

