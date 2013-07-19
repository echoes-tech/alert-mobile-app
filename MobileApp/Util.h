/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * util.h
 *
 *  Created on: 13 mai 2013
 *      Author: gdr
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <mastring.h>		// C string functions


#include <ma.h>
#include <maassert.h>
#include <conprint.h>
//#include <MAUtil/String.h>
#include <MAUtil/FileLister.h>

#include "mavsprintf.h"
#include "resource/Convert.h"

#define BUF_MAX 256

enum platform_code{
	ANDROID = 0,
	IOS = 1,
	WINDOWSPHONE7 = 2
};

enum eHttp{
	GET = 0,
	POST,
	DELETE
};

enum eFile{
	FILE_OPEN_ERROR = 0,
	FILE_NOT_EXIST,
	FILE_CLOSE
};

enum eScreenResolution{
    LARGE_SCREEN_RESOLUTION	= 0,
    SMALL_SCREEN_RESOLUTION = 1
};

/**
 * Detects the current platform
 * @return platform_code specific for Android, iPhone OS or WindowsPhone
 */
static int getPlatform()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);

	if(strcmp(platform, "Android") == 0)
	{
		return ANDROID;
	}
	else
	{
		if(strcmp(platform, "iPhone OS") == 0)
			return IOS;
	}
	return WINDOWSPHONE7;
}

static int getSystemConnection()
{
	char buf[64] = "";

		int res = maGetSystemProperty("mosync.network.type",buf, sizeof(buf));
		lprintfln("Connecté ? : %i: %s\n", res, buf);
		MAUtil::String tmp = buf;
		if(tmp == "none")
		{
//			TODO translate
			maPanic(1, "No connection detected");
			return -1;
		}
		return res;

}


static int getSystemLanguage()
{
	int language = 0;
	char buf[64] = "";
	int res = maGetSystemProperty("mosync.iso-639-1",buf, sizeof(buf));
	lprintfln("LANGUAGE : %i: %s\n", res, buf);
	MAUtil::String tmp = buf;
	if(tmp == "fr")
	{
		lprintfln(" DAns LANGUAGE : %i: %s\n", res, buf);
		language = 1;
	}
	return language;
}

static int getSystemProperty(const char* key, MAUtil::String& dst) {
    int size = maGetSystemProperty(key, NULL, 0);
    if(size < 0)
        return size;
    dst.resize(size-1);
    maGetSystemProperty(key, dst.pointer(), size);
    return size;
}

static MAUtil::String getLocalPath() {
    // Do this here to work around a MoRE bug.
    FileLister fl;
    fl.start("/");

    MAUtil::String path;
    // Try getting the local path.
    int result = getSystemProperty("mosync.path.local", path);
    // If it works, fine.
    if(result > 0) {
//    	maToast(path.c_str(), MA_TOAST_DURATION_LONG);
        printf("Got local path: %i\n", result);
        return path + "";
    }

    // Otherwise, get the first root directory.
    fl.start("");
    result = fl.next(path);
    MAASSERT(result > 0);
    return path;
}

static int saveToStore(const char *name, const String &data)
{
	lprintfln("saveToStore");
	MAHandle dataHandle = maCreatePlaceholder();
	if(maCreateData(dataHandle, data.length()) != RES_OK)
	{
		lprintfln("saveToStore1");
		return -1;
	}
	lprintfln("saveToStore2");
	maWriteData(dataHandle, data.c_str(), 0, data.length());
	lprintfln("saveToStore3");
	MAHandle myStore = maOpenStore(name, MAS_CREATE_IF_NECESSARY);

	if(myStore > 0)
	{
		lprintfln("saveToStore4");
		maWriteStore(myStore, dataHandle);
		lprintfln("saveToStore5");
		maCloseStore(myStore, 0);
		lprintfln("saveToStore6");
		return 1;
	}
	return -1;
}


static int readFromStore(const char *name, String &data)
{
	MAHandle store = maOpenStore(name, 0);
	if(store>0)
	{
		MAHandle dataHandle = maCreatePlaceholder();
//		int len = data.length();
		if( maReadStore(store, dataHandle) != RES_OUT_OF_MEMORY  )
		{
			int size = maGetDataSize(dataHandle);
			char temp[size + 1];
			temp[size] = '\0';
			maReadData(dataHandle, &temp, 0, size);

			data.clear();
			data = String(temp);

			return 1;
		}
	}
	return -1;
}

static eFile tryToRead(MAUtil::String &config) {
    // Construct the filename.
	if (getPlatform() != IOS) {
		MAUtil::String filename = getLocalPath() + "EA_mobile_app_conf.txt";

		// Open the file handle.
		lprintfln("OpenRead '%s'\n", filename.c_str());
		MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ);
		if (file < 0) {
			lprintfln("Error %i\n", file);
			return FILE_OPEN_ERROR;
		}

		// Check if the file exists.
		int res = maFileExists(file);
		MAASSERT(res >= 0);
		if (!res) {
			printf("File does not exist.\n");
			maFileClose(file);
			return FILE_NOT_EXIST;
		}

		// Get the file size.
		int size = maFileSize(file);
		lprintfln("Size: %i\n", size);
		MAASSERT(size >= 0);

		// Read the file data.
		static char data[600];
		MAASSERT(size < (int)sizeof(data));
		res = maFileRead(file, data, size);
		MAASSERT(res == 0);
		config = data;
		lprintfln(config.c_str());
//    maToast(data,MA_TOAST_DURATION_LONG);
		// Print some data.
//    data[300] = 0;
//    lprintfln("%s\n", data);

		// Close the file.
		lprintfln("Closing...\n");
		res = maFileClose(file);
		MAASSERT(res == 0);
	}
	else
	{
		if (readFromStore("EA_mobile_app_conf.txt",config)){
					return FILE_CLOSE;
				}
				else{
					return FILE_OPEN_ERROR;
				}

	}
    lprintfln("Done.\n");
    return FILE_CLOSE;
}

static eFile tryToWrite(MAUtil::String &login, MAUtil::String &tokenMobile, MAUtil::String &tokenAuthent, MAUtil::String &mode, long long &idMedia, bool vibrate, bool notification) {

	lprintfln("trytowrite");
	int res;
	MAHandle file;
	int plateforme = getPlatform();
	if (plateforme != IOS) {
		// Construct the filename.
		MAUtil::String filename = getLocalPath() + "EA_mobile_app_conf.txt";

		// Open the file handle.
		printf("OpenWrite '%s'\n", filename.c_str());
		file = maFileOpen(filename.c_str(),
				MA_ACCESS_READ_WRITE /*MAS_CREATE_IF_NECESSARY*/);
		if (file < 0) {
			printf("Error %i\n", file);
			return FILE_OPEN_ERROR;
		}

		// If the file exists...
	    res = maFileExists(file);
		MAASSERT(res >= 0);
		if (res) {
			// Truncate it, deleting any old data.
			printf("Truncating file...\n");
			res = maFileTruncate(file, 0);
			MAASSERT(res == 0);
		} else {
			// Otherwise, create it.
			printf("Creating file...\n");
			res = maFileCreate(file);
			MAASSERT(res >= 0);
		}
		// In either case, we now have an empty file at our disposal.
	}
	// Write data.
	String sVibrate = "true";
	String sNotification = "true";
	if (vibrate == false) {
		sVibrate = "false";
	}
	if (notification == false) {
		sNotification = "false";
	}
	lprintfln("trytowrite1");
	String tmp = "{\"login\" : \"";
	lprintfln("trytowrite1.01");
	String tmp01 = login ;
	lprintfln("trytowrite1.02");
	String tmp02 = "\",\"token_mobile\" : \"";
	lprintfln("trytowrite1.03");
	String tmp1	= tokenMobile;
		lprintfln("trytowrite1.1");
	String tmp2 = "\",\"token_authent\" : \"" + tokenAuthent;
		lprintfln("trytowrite1.2");
	String tmp3	= "\",\"authentication_mode\" : \"" + mode;
		lprintfln("trytowrite1.3");
	String tmp4	= "\",\"id_media_value\" : " + Convert::toString(idMedia);
		lprintfln("trytowrite1.4");
	String tmp5	= ",\"notification\" : " + sNotification + ",\"vibrate\" : ";
		lprintfln("trytowrite1.5");
	String tmp6	= sVibrate + "}";
		lprintfln("trytowrite1.6");
		tmp += tmp01 + tmp02 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6;
	lprintfln("trytowrite1.7");
	//    static const char data[] = tmp.c_str();
	if (plateforme != IOS) {
		res = maFileWrite(file, tmp.c_str(), tmp.size());
		lprintfln(tmp.c_str());
//    maToast(tmp.c_str(),MA_TOAST_DURATION_LONG);
//    res = maFileWrite(file, data, sizeof(data));
		MAASSERT(res == 0);

		// Close the file.
		printf("Closing...\n");
		res = maFileClose(file);
		MAASSERT(res == 0);
	}else{
		lprintfln("trytowrite2");
		if (saveToStore("EA_mobile_app_conf.txt",tmp)){
			lprintfln("trytowrite3");
			return FILE_CLOSE;
		}
		else{
			lprintfln("trytowrite4");
			return FILE_OPEN_ERROR;
		}
	}
    printf("Done.\n");
    return FILE_CLOSE;
}



//static void searchFileList(const char* path) {
//    // The buffer is static to conserve stack space.
//    static char buffer[1024];
//
//    MAHandle list = maFileListStart(path, "*", 0);
//    if(list < 0) {
//    	lprintfln("maFileListStart error %i", list);
//        return;
//    }
//    int size;
//    while((size = maFileListNext(list, buffer, sizeof(buffer))) > 0) {
//        // Since the program can take so long to run, we check for
//                // keypresses often.
//
//        if(size > (int)sizeof(buffer)) {
//        	lprintfln("Buffer too small!\n");
//            return;
//        }
//
//        // Print the path and name of the file we found.
//        lprintfln("%s%s\n", path, buffer);
//        maMessageBox("test",buffer);
//        // If we found a directory, print its contents.
//        if(buffer[size-1] == '/') {
//            MAUtil::String subDir(path);
//            subDir += buffer;
//            searchFileList(subDir.c_str());
//        }
//    }
//    maFileListClose(list);
//}



#endif /* UTIL_H_ */
