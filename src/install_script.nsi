; Name of the installer.
OutFile "rodoc-installer.exe"

; Set the default installation directory
InstallDir $PROGRAMFILES\rodoc

Icon "icon.ico"

; Default section
Section

; Set output path to the installation directory
SetOutPath $INSTDIR

; Copy files to the installation directory
File "D3Dcompiler_47.dll"
File "jsoncpp-25.dll"
File "opengl32sw.dll"
File "Qt6Cored.dll"
File "Qt6Guid.dll"
File "Qt6Networkd.dll"
File "Qt6Pdfd.dll"
File "Qt6Svgd.dll"
File "Qt6Widgetsd.dll"
File "rodoc.exe"
File "icon.ico"
File "msvcp140d.dll"
File "vcruntime140d.dll"
File "vcruntime140_1d.dll"
File "ucrtbased.dll"
File "msvcp140_1d.dll"
File "msvcp140_2d.dll"

SetOutPath "$INSTDIR\generic"
File /a "generic\qtuiotouchplugind.dll"

SetOutPath "$INSTDIR\iconengines"
File /a "iconengines\qsvgicond.dll"

SetOutPath "$INSTDIR\imageformats"
File /a "imageformats\qgifd.dll"
File /a "imageformats\qicnsd.dll"
File /a "imageformats\qicod.dll"
File /a "imageformats\qjpegd.dll"
File /a "imageformats\qpdfd.dll"
File /a "imageformats\qsvgd.dll"
File /a "imageformats\qtgad.dll"
File /a "imageformats\qtiffd.dll"
File /a "imageformats\qwbmpd.dll"
File /a "imageformats\qwebpd.dll"

SetOutPath "$INSTDIR\networkinformation"
File /a "networkinformation\qnetworklistmanagerd.dll"

SetOutPath "$INSTDIR\platforms"
File /a "platforms\qwindowsd.dll"

SetOutPath "$INSTDIR\styles"
File /a "styles\qwindowsvistastyled.dll"

SetOutPath "$INSTDIR\tls"
File /a "tls\qcertonlybackendd.dll"
File /a "tls\qopensslbackendd.dll"
File /a "tls\qschannelbackendd.dll"

; Create shortcuts
CreateShortCut "$DESKTOP\rodoc.lnk" "$INSTDIR\rodoc.exe" "" "$INSTDIR\icon.ico" 0
CreateShortCut "$SMPROGRAMS\rodoc\rodoc.lnk" "$INSTDIR\rodoc.exe" "" "$INSTDIR\icon.ico" 0

; End section
SectionEnd
