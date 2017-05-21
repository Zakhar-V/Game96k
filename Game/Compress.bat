md "Bin\Compressed"

echo =========================================================
echo KKRUNCHY
ThirdParty\kkrunchy_k7.exe --new --out "Bin/Compressed/Game_kkrunchy".exe "Bin/Release/Game.exe"

echo =========================================================
echo UPX
del /q /s "Bin/Compressed/Game_upx.exe"
ThirdParty\upx.exe -9 -f -v -o "Bin/Compressed/Game_upx.exe" "Bin/Release/Game.exe"
pause
