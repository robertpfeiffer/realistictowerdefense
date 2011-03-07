Name "Tower Defense"

OutFile "setup_TowerDefense.exe"
InstallDir "$PROGRAMFILES\Tower Defense"

Page license
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Core Components"
	SetOutPath "$INSTDIR"
	File "build\bin\Towerdefense.exe"
	File "OSG\bin\osg67-osg.dll"
	File "OSG\bin\osg67-osgParticle.dll"
	File "OSG\bin\osg67-osgUtil.dll"
	File "OSG\bin\osg67-osgDB.dll"
	File "OSG\bin\osg67-osgAnimation.dll"
	File "OSG\bin\osg67-osgText.dll"
	File "OSG\bin\osg67-osgGA.dll"
	File "OSG\bin\osg67-osgViewer.dll"
	File "OSG\bin\osg67-osgFX.dll"
	File "OSG\bin\ot12-OpenThreads.dll"
	
	;TODO: plugins
	
	SetOutPath "$INSTDIR\fonts"
	File "fonts\*.ttf"
	
	SetOutPath "$INSTDIR\maps"
	File "maps\*.tdmap"
	
	SetOutPath "$INSTDIR\models\creeps"
	File "models\creeps\*.osg"
	
	SetOutPath "$INSTDIR\models\towers"
	File "models\towers\*.osg"
SectionEnd