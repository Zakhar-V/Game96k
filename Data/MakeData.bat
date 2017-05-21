

echo =========================================================
echo MODEL CONVERTER

Temp\ModelConverter.exe Models Models.blend Test.blend

echo =========================================================
echo PACKER
rem Temp\Packer.exe /c /i EditorData.inl Common.glsl Generic-VS.glsl Generic-PS.glsl Quad-GS.glsl
Temp\Packer.exe /c /i GameData.inl Common.glsl Generic-VS.glsl Generic-PS.glsl Quad-GS.glsl Models


del /f /s /q Temp
rmdir Temp
pause
