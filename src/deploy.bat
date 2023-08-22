meson setup build/
meson compile -C build/

copy /Y build\subprojects\jsoncpp-1.9.5\jsoncpp-25.dll build\
copy /Y icon.ico build\
copy /Y install_script.nsi build\install_script.nsi
cd ./build/
windeployqt rodoc.exe

copy /Y C:\Windows\System32\msvcp140d.dll .
copy /Y C:\Windows\System32\msvcp140_1d.dll .
copy /Y C:\Windows\System32\msvcp140_2d.dll .
copy /Y C:\Windows\System32\vcruntime140d.dll .
copy /Y C:\Windows\System32\vcruntime140_1d.dll .
copy /Y C:\Windows\System32\ucrtbased.dll .

PAUSE