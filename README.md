SSoftwareUpdater
================

###usage:
updater -unit [unit-name] -version [version] -url [url-to-file] [| -hard]

	"-unit"
		windows
		linux
		osx
		my-cool-module-name
		etc.. name of platform, modules or somethings yours
	
	"-version"  - version  e.g. 2.4.0a
		new version must be lexicographical greater than old to start updating
	
	"-url"  - url with instructions file in XML(recommended) or JSON(not ready)
		you can find file format below
	
	"-hard"  - download file and setup update right now.  Otherwise, it starts setup on next 

###example
	updater -unit windows -version 0.3.2 -url http://example.com/updates.xml
	updater -unit myCoolUnitName -version 2.8a -url http://127.0.0.1/updates.xml -hard

### [XML file example (updates on server side) (Click here!)] (https://github.com/Esenin/SSoftwareUpdater/blob/master/updates.xml)

###client usage

Execute updater with right params. Hook stdIO, then:
print filepath of your main application to Updater™  and wait for incomming stringline
It's recommended to terminate your application if you get "Terminate!" string, otherwise you should continue if it is "Resume!"

After installing patch Updater will restart your application


EOF