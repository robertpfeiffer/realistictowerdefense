Name "Tower Defense"

OutFile "setup_TowerDefense.exe"
InstallDir "$PROGRAMFILES\Tower Defense"

Page license
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Core Components"
	SetOutPath $INSTDIR
	File "build/bin/TowerDefense.exe"
	;TODO: OSG-libraries
	
	SetOutPath "$INSTDIR/fonts"
	File "fonts/*.ttf"
	
	SetOutPath "$INSTDIR/maps"
	File "maps/*.tdmap"
	
	SetOutPath "$INSTDIR/models/creeps"
	File "models/creeps/*.osg"
	
	SetOutPath "$INSTDIR/models/towers"
	File "models/towers/*.osg"
SectionEnd