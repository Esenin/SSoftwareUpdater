SSoftwareUpdater
================

##Simple updater

###usage:
updater -unit [unit-name] -version [version] -url [url-to-file]

	"unit"
		windows
		linux
		osx
		etc.. whatever yours
	
	"version"  - version  e.g. 2.4.0a
		new version must be lexicographical greater than old to start updating
	
	"url"  - url with instructions file in XML or JSON(not ready)
		you can find file format below
	
	"-soft"  - download file, but run setup only when next time main program started

###example
	updater -unit windows -version 0.3.2 -url http://127.0.0.1/updates.xml
	updater -unit windows -version 2.8a -url http://127.0.0.1/updates.xml -soft

### [XML file example (updates on server side)] (https://github.com/Esenin/SSoftwareUpdater/blob/master/updates.xml)