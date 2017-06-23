set OUTPUTDIR=%1
set APP_RELEASES=%2
echo "This is OUTPUTDIR: "%OUTPUTDIR%
echo "and AppReleases: "%APP_RELEASES%
if not exist %OUTPUTDIR%themes mkdir %OUTPUTDIR%themes
if not exist %APP_RELEASES%\VolumeCreator\themes mkdir %APP_RELEASES%\VolumeCreator\themes
xcopy ..\resources\themes\*.vsf %OUTPUTDIR%\themes /y
xcopy ..\resources\themes\*.vsf %APP_RELEASES%\VolumeCreator\themes /y
xcopy %OUTPUTDIR%VolumeCreator.exe %APP_RELEASES%\VolumeCreator /y
xcopy ..\*.txt %APP_RELEASES%\VolumeCreator /y
xcopy ..\*.txt %OUTPUTDIR% /y
xcopy %OUTPUTDIR%*.dll %APP_RELEASES%\VolumeCreator /y
xcopy %OUTPUTDIR%*.ini %APP_RELEASES%\VolumeCreator /y
xcopy %OUTPUTDIR%%OUTPUTFILENAME% %APP_RELEASES%\VolumeCreator /y
xcopy p:\redist\atVolumeCreator\*.dll %APP_RELEASES%\VolumeCreator /y
xcopy p:\redist\atVolumeCreator\*.bpl %APP_RELEASES%\VolumeCreator /y
xcopy p:\build\xe3_ide\bpl\*.bpl %APP_RELEASES%\VolumeCreator /y


