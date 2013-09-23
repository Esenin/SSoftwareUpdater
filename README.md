SSoftwareUpdater
================

##Simple updater

###usage:
updater --platform <platform-name> --version <version-ID> --link <link-to-file>

	* "platform"
		windows
		linux
		osx
		etc.. whatever yours
	
	* "version"  - version ID e.g. version 2.4.0 -> 240 
		new versionID must be greater than old to start updating
	
	* "link"  - link with instructions file in XML or JSON(not ready)
		you can find file format below
	
	* "soft"  - will start setup with next run of main program

###example
1)
updater --platform windows --version 032 --link http://127.0.0.1/updates.xml
2) 
updater --platform windows --version 032 --link http://127.0.0.1/updates.xml --soft

### XML file example (updates on server side)

<updaterInformation>
    <versionId>032</versionId>
    <version>0.3.2</version>
    <platformFileList>
        <platformFile>
            <platform>windows</platform>
            <url>http://192.168.56.1:8080/Sea_battle0.3_Setup.exe</url>            
        </platformFile>
        <platformFile>
            <platform>linux</platform>
            <url>http://192.168.56.1:8080/Sea_battle0.3_Setup.rar</url>
        </platformFile>
        <platformFile>
            <platform>osx</platform>
            <url>http://192.168.56.1:8080/Sea_battle0.3_Setup.rar</url>
        </platformFile>
    </platformFileList>
</updaterInformation>

