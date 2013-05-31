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
#include "mavsprintf.h"

#define BUF_MAX 256

enum platform_code{
	ANDROID = 0,
	IOS = 1,
	WINDOWSPHONE7 = 2
};

enum eHttp{
	GET = 0,
	POST
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

#endif /* UTIL_H_ */
