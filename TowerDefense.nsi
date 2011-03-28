Name "Tower Defense"

OutFile "setup_TowerDefense.exe"
InstallDir "$PROGRAMFILES\Tower Defense"
LicenseData "LICENSE.txt"
RequestExecutionLevel admin

Page license
Page directory
Page components
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Core Components"
	SetOutPath "$INSTDIR"
	;TD
	File "build\bin\Towerdefense.exe"
	File "Towerdefense_Launcher.exe"
	;OpenSceneGraph
	File "OSG\bin\osg67-osg.dll"
	File "OSG\bin\osg67-osgAnimation.dll"
	File "OSG\bin\osg67-osgDB.dll"
	File "OSG\bin\osg67-osgFX.dll"
	File "OSG\bin\osg67-osgGA.dll"
	File "OSG\bin\osg67-osgParticle.dll"
	File "OSG\bin\osg67-osgSim.dll"
	File "OSG\bin\osg67-osgText.dll"
	File "OSG\bin\osg67-osgUtil.dll"
	File "OSG\bin\osg67-osgViewer.dll"
	File "OSG\bin\ot12-OpenThreads.dll"
	;OSG-plugins
	File "OSG\bin\osgPlugins-2.9.11\osgdb_3ds.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_bmp.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osg.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osganimation.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osgparticle.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_freetype.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_jpeg.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_png.dll"
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

Section "Startmenu Shortcut"
	CreateShortCut "$SMPROGRAMS\TowerDefense.lnk" "$INSTDIR\Towerdefense_Launcher.exe"
SectionEnd

Section "Desktop Shortcut"
	CreateShortCut "$DESKTOP\TowerDefense.lnk" "$INSTDIR\Towerdefense_Launcher.exe"
SectionEnd