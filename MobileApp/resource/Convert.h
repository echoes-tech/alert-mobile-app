/*
 * Convert.h
 *
 *  Created on: 24 May 2010
 *      Author: orac7
 */

// There are too many ways to convert between numeric and string types in MoSync.
// mastring.h, madmath.h, mastdlib.h and maxtoa.h all have functions for conversions.
// I just use this so I've got it all in one place...

#ifndef CONVERT_H_
#define CONVERT_H_

#include <MAUtil/String.h>
#include <matime.h>
using namespace MAUtil;

struct STime {
	int sec;
	int min;
	int hour;
	int day;
	int mon;
	String sMon; //Jan, Feb, ...
	int year;
};

bool operator>(STime const&a, STime const&b);

class Convert
{
	public:
		Convert();
		~Convert();


		static STime toSTime(String& input);
		static int toInt(String& input);
		static int toInt(const char* input);
		static double toDouble(String& input);
		static double toDouble(const char* input);
		static time_t toDateTime(String& input);

		static String toString(int input);
		static String toString(double input);
		static String toString(long long input);

		//toString(const byte* input) converts multibyte charactes to a
		//a single char agains Windows-1252 as best it can
		static String toString(const byte* input, size_t count);
		static String toString(bool input);

		static int hexToInt(const char* input);

		static void formatJSONBeforeParse(String& input, int nbBackslash = 1);

		static char* tr(MAHandle stringResource);

};

#endif /* CONVERT_H_ */
