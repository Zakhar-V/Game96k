md "Bin\Compressed"

echo =========================================================
echo CRINKLER
rem ThirdParty\crinkler.exe /OUT:"Bin/Compressed/Game_crinkler.exe" /REPORT:"Bin/Release/Game.cm.html" /PRINT:IMPORTS /ORDERTRIES:10000 /COMPMODE:SLOW /LARGEADDRESSAWARE:NO /SUBSYSTEM:WINDOWS /ENTRY:main kernel32.lib user32.lib gdi32.lib opengl32.lib dxguid.lib ole32.lib dsound.lib "Bin/Release/Obj/Game/Main.obj"
ThirdParty\crinkler.exe /OUT:"Bin/Compressed/Game_crinkler.exe" /REPORT:"Bin/Release/Game.cm.html" /PRINT:IMPORTS /COMPMODE:INSTANT /LARGEADDRESSAWARE:NO /SUBSYSTEM:WINDOWS /ENTRY:main kernel32.lib user32.lib gdi32.lib opengl32.lib dxguid.lib ole32.lib dsound.lib "Bin/Release/Obj/Game/Main.obj"

echo =========================================================
echo KKRUNCHY
ThirdParty\kkrunchy_k7.exe --new --out "Bin/Compressed/Game_kkrunchy".exe "Bin/Release/Game.exe"

echo =========================================================
echo UPX
del /q /s "Bin/Compressed/Game_upx.exe"
ThirdParty\upx.exe -9 -f -v -o "Bin/Compressed/Game_upx.exe" "Bin/Release/Game.exe"
pause

