# Only deploy on fresh builds once
echo "Deploying Qt app"
macdeployqt FLAME\ Visualiser.app/
mkdir FLAME\ Visualiser
cp -r FLAME\ Visualiser.app/ FLAME\ Visualiser/FLAME\ Visualiser.app
ln -s /Applications/ FLAME\ Visualiser/Applications
echo "Creating dmg"
hdiutil create -format UDBZ -quiet -srcfolder FLAME\ Visualiser FLAME\ Visualiser.dmg
rm -rf FLAME\ Visualiser
echo "Finished"