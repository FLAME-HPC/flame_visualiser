# Only deploy on fresh builds once
macdeployqt Flame\ Visualiser.app/
hdiutil create -format UDBZ -quiet -srcfolder Flame\ Visualiser.app Flame\ Visualiser.dmg