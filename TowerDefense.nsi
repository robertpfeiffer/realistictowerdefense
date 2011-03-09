Name "Tower Defense"

OutFile "setup_TowerDefense.exe"
InstallDir "$PROGRAMFILES\Tower Defense"
LicenseData "LICENSE.txt"
RequestExecutionLevel admin

Page license
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Core Components"
	SetOutPath "$INSTDIR"
	;TD
	File "build\bin\Towerdefense.exe"
	File "build\bin\Towerdefense_Launcher.exe"
	;OpenSceneGraph
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
	;Qt-Dependencies
	File "libgcc_s_dw2-1.dll"
	File "mingwm10.dll"
	File "QtCore4.dll"
	File "QtGui4.dll"
	
	SetOutPath "$INSTDIR\fonts"
	File "fonts\*.ttf"
	
	SetOutPath "$INSTDIR\maps"
	File "maps\*.tdmap"
	
	SetOutPath "$INSTDIR\models\creeps"
	File "models\creeps\*.osg"
	
	SetOutPath "$INSTDIR\models\projectiles"
	File "models\projectiles\*.osg"
	
	SetOutPath "$INSTDIR\models\shrubs"
	File "models\shrubs\*.osg"
	
	SetOutPath "$INSTDIR\models\towers"
	File "models\towers\*.osg"
	
	SetOutPath "$INSTDIR\models\trees"
	File "models\trees\*.osg"
	
	SetOutPath "$INSTDIR\textures"
	File "textures\*.png"
	File "textures\*.jpg"
	
	SetOutPath "$INSTDIR\textures\billboards"
	File "textures\billboards\*.png"
	
	SetOutPath "$INSTDIR\textures\healthbar"
	File "textures\healthbar\*.png"
	
	SetOutPath "$INSTDIR\textures\lostvalley"
	File "textures\lostvalley\*.png"
SectionEnd