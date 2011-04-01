Name "Tower Defense"

OutFile "setup_TowerDefense.exe"
InstallDir "$PROGRAMFILES\Tower Defense"
LicenseData "LICENSE.txt"
RequestExecutionLevel admin

Page license
Page directory
Page components
Page instfiles

Section "Core Components"
	SetOutPath "$INSTDIR"
	;TD
	File "build\bin\Towerdefense.exe"
	File "Towerdefense_Launcher.exe"
	
	;OpenSceneGraph
	File "OSG\bin\osg71-osg.dll"
	File "OSG\bin\osg71-osgAnimation.dll"
	File "OSG\bin\osg71-osgDB.dll"
	File "OSG\bin\osg71-osgFX.dll"
	File "OSG\bin\osg71-osgGA.dll"
	File "OSG\bin\osg71-osgParticle.dll"
	File "OSG\bin\osg71-osgSim.dll"
	File "OSG\bin\osg71-osgText.dll"
	File "OSG\bin\osg71-osgUtil.dll"
	File "OSG\bin\osg71-osgViewer.dll"
	File "OSG\bin\ot12-OpenThreads.dll"
	;OSG-plugins
	File "OSG\bin\osgPlugins-2.9.11\osgdb_3ds.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_bmp.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_osg.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osg.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osgfx.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osganimation.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_deprecated_osgparticle.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_freetype.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_jpeg.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_png.dll"
	File "OSG\bin\osgPlugins-2.9.11\osgdb_rgb.dll"
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
	File "models\creeps\*.png"
	
	SetOutPath "$INSTDIR\models\projectiles"
	File "models\projectiles\*.osg"
	File "models\projectiles\*.png"
	
	SetOutPath "$INSTDIR\models\shrubs"
	File "models\shrubs\*.osg"
	File "models\shrubs\*.png"
	
	SetOutPath "$INSTDIR\models\towers"
	File "models\towers\*.osg"
	File "models\towers\*.png"
	
	SetOutPath "$INSTDIR\models\trees"
	File "models\trees\*.osg"
	File "models\trees\*.png"
	File "models\trees\*.jpg"
	
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
	SetOutPath "$INSTDIR"
	CreateShortCut "$SMPROGRAMS\TowerDefense.lnk" "$INSTDIR\Towerdefense_Launcher.exe"
SectionEnd

Section "Desktop Shortcut"
	SetOutPath "$INSTDIR"
	CreateShortCut "$DESKTOP\TowerDefense.lnk" "$INSTDIR\Towerdefense_Launcher.exe"
SectionEnd