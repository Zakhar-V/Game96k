echo =========================================================
echo PACKER
rem Temp\Packer.exe /c /i EditorData.inl Common.glsl Generic-VS.glsl Generic-PS.glsl Quad-GS.glsl
Temp\Packer.exe /c /i GameData.inl Common.glsl Generic-VS.glsl Generic-PS.glsl Quad-GS.glsl

del /f /s /q Temp
rmdir Temp
pause
