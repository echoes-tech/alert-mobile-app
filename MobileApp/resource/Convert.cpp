/*
 * Convert.cpp
 *
 *  Created on: 24 May 2010
 *      Author: orac7
 */
#include "Convert.h"
#include <mavsprintf.h>
#include "snprintf.h"
#include <mawstring.h>
#include <wchar.h>


bool operator>(STime const&a, STime const&b)
 {
	if (a.year > b.year) {
		return true;
	} else if (a.year == b.year && a.mon > b.mon) {
		return true;
	} else if (a.year == b.year && a.mon == b.mon && a.day > b.day) {
		return true;
	} else if (a.year == b.year && a.mon == b.mon && a.day == b.day
			&& a.hour > b.hour) {
		return true;
	} else if (a.year == b.year && a.mon == b.mon && a.day == b.day
			&& a.hour == b.hour && a.min > b.min) {
		return true;
	} else if (a.year == b.year && a.mon == b.mon && a.day == b.day
			&& a.hour == b.hour && a.min == b.min && a.sec > b.sec) {
		return true;
	}

	return false;
}

Convert::Convert()
{}

Convert::~Convert()
{}

STime Convert::toSTime(String& input)
{
	STime sTime;

	input.remove(0, input.findFirstOf(' ', 0) + 1);
//	lprintfln(input.c_str());
	sTime.sMon = input.substr(0, input.findFirstOf(' ', 0));
//	lprintfln(sTime.sMon.c_str());
	if( sTime.sMon == "Jan" ){ sTime.mon = 1;}
	else if( sTime.sMon == "Feb" ){ sTime.mon = 2;}
	else if( sTime.sMon =="Mar" ){ sTime.mon = 3;}
	else if( sTime.sMon =="Apr" ){ sTime.mon = 4;}
	else if( sTime.sMon =="May" ){ sTime.mon = 5;}
	else if( sTime.sMon =="Jun" ){ sTime.mon = 6;}
	else if( sTime.sMon =="Jul" ){ sTime.mon = 7;}
	else if( sTime.sMon =="Aug" ){ sTime.mon = 8;}
	else if( sTime.sMon =="Sep" ){ sTime.mon = 9;}
	else if( sTime.sMon =="Oct" ){ sTime.mon = 10;}
	else if( sTime.sMon =="Nov" ){ sTime.mon = 11;}
	else if( sTime.sMon =="Dec" ){ sTime.mon = 12;}
	else{sTime.mon = 1;}

//	lprintfln(sTime.mon.c_str());

	input.remove(0, input.findFirstOf(' ', 0) + 1);
	sTime.day = Convert::toInt(input.substr(0, input.findFirstOf(' ', 0)).c_str());
//	lprintfln(sTime.day.c_str());

	input.remove(0, input.findFirstOf(' ', 0) + 1);
	sTime.hour = Convert::toInt(input.substr(0, input.findFirstOf(':', 0)).c_str());
//	lprintfln(sTime.hour.c_str());

	input.remove(0, input.findFirstOf(':', 0) + 1);
	sTime.min = Convert::toInt(input.substr(0, input.findFirstOf(':', 0)).c_str());
//	lprintfln(sTime.min.c_str());

	input.remove(0, input.findFirstOf(':', 0) + 1);
	sTime.sec = Convert::toInt(input.substr(0, input.findFirstOf(' ', 0)).c_str());
//	lprintfln(sTime.sec.c_str());

	input.remove(0, input.findFirstOf(' ', 0) + 1);
	sTime.year = Convert::toInt(input.c_str());
//	lprintfln(sTime.year.c_str());

	return sTime;
}

int Convert::toInt(const char* digit)
{
   int sign = 1;
   int result = 0;

   // check sign
   if (*digit == '-')
   {
	  sign = -1;
	  digit++;
   }

   //--- get integer portion
   while (*digit >= '0' && *digit <='9')
   {
	  result = (result * 10) + *digit-'0';
	  digit++;
   }

   //--- set to sign given at the front
   return result * sign;
}

int Convert::toInt(String& input)
{
	return toInt(input.c_str());
}

double Convert::toDouble(String& input)
{
	return toDouble(input.c_str());
}

double Convert::toDouble(const char* input)
{
   int sign = 1;
   double result = 0;
   const char* digit = input;

   // check sign
   if (*digit == '-')
   {
	  sign = -1;
	  digit++;
   }

   //--- get integer portion
   while (*digit >= '0' && *digit <='9')
   {
	  result = (result * 10) + *digit-'0';
	  digit++;
   }

   //--- get decimal point and fraction, if any.
   if (*digit == '.')
   {
	  digit++;
	  double scale = 0.1;
	  while (*digit >= '0' && *digit <='9') {
		 result += (*digit-'0') * scale;
		 scale *= 0.1;
		 digit++;
	  }
   }

   //--- error if we're not at the end of the number
   if (*digit != 0) {
	  return 0.00;
   }

   //--- set to sign given at the front
   return result * sign;
}

time_t Convert::toDateTime(String& input)
{
	return (time_t)toInt(input);
}

String Convert::toString(bool input)
{
	if(input)
		return "1";
	else
		return "0";
}

String Convert::toString(int input)
{
	char buffer[128];
	snprintf(buffer, 128, "%d", input);

	return buffer;
}

String Convert::toString(long long input)
{
	char buffer[128];
	snprintf(buffer, 128, "%d", input);

	return buffer;
}

String Convert::toString(double input)
{
	char buffer[128];
	snprintf(buffer, 128, "%f", input);

	return buffer;
}

String Convert::toString(const byte* src, size_t count)
{
	//At least enough space, even if every byte turns out to be a single character
	char output[count + 1];
	int ctr = 0;
	while(*src != 0)
	{
		if((*src & 0x80) == 0)
		{
			//ASCII-compatible character
			output[ctr++] = *src++;
		}
		else if((*src & 0xE0) == 0xC0)
		{
			// 2 bytes
			byte b1 = *src++;
			byte b2 = *src++;
			if((b2 & 0xC0) != 0x80)
					break;
			output[ctr++] = ((b1 & 0x1F) << 6) | (b2 & 0x3F);
		}
		else if((*src & 0xF0) == 0xE0)
		{
			// 3 bytes
			byte b1 = *src++;
			byte b2 = *src++;
			byte b3 = *src++;
			if((b2 & 0xC0) != 0x80)
					break;
			if((b3 & 0xC0) != 0x80)
					break;
			output[ctr++] = ((b1 & 0x0F) << 12) | ((b2 & 0x1F) << 6) |
					(b3 & 0x3F);
		}
	}
	output[ctr] = '\0';

	return output;
}

int Convert::hexToInt(const char* input)
{

	int v = 0;
	while (char c = *input++)
	{
		if (c < '0') return 0; //invalid character
		if (c > '9') //shift alphabetic characters down
		{
		if (c >= 'a') c -= 'a' - 'A'; //upper-case 'a' or higher
		if (c > 'Z') return 0; //invalid character
		if (c > '9') c -= 'A'-1-'9'; //make consecutive with digits
		if (c < '9' + 1) return 0; //invalid character
		}
		c -= '0'; //convert char to hex digit value
		v = v << 4; //shift left by a hex digit
		v += c; //add the current digit
	}

	return v;
}


void Convert::formatJSONBeforeParse(String &input, int nbBackslash)
{
    int compt = 0;
    while( compt != -1)
    {
        if(input.find("\\",compt) < input.size() && input.find("\\",compt) >= 0 )
        {
            for(int i=0; i<nbBackslash; i++)
            {
            	input.insert(input.find("\\", compt), "\\");
            }
            compt = input.find("\\",compt); //compt = l'indexe ou se trouve le premier caractÃ©re "\" dans la string.
                                            // recherche dans la string Ã  partir de l'indexe compt;
            compt = compt + nbBackslash*2;//value.find_first_not_of("\\", compt+1);
        }
        else
        {
            compt=-1;
        }
    }
}

char* Convert::tr(MAHandle stringResource) {
	// Get the length of the string data.
	int length = maGetDataSize(stringResource);

	// Allocate space for the string data plus the
	// null termination character.
	char* buffer = new char[length + 1];

	// Read data.
	maReadData(stringResource, buffer, 0, length);

	// Null terminate the string.
	buffer[length] = '\0';

	// Return the string.
	return buffer;
}


/* URLEncodes string with uppercase values, as specified by oAuth */
String Convert::URLencode(const String& c)
{
	lprintfln("URLEncoding");
    String escaped = "";
    String charCode = "";
    int max = c.length();
    for(int i=0; i<max; i++)
    {
    	/* I found this code on the net, and I'm afraid I can't remember where.  I've adapted it to allow for
    	 * Twitter's very specific requirements for URL encoding.  The following characters are always unencoded.
    	 * This differs from many other implementations.
    	 * Also, escaped values MUST be in upper case hex, so %2F is valid, and %2f is not.  This is because a URLEncoded string
    	 * is hashed, and obviously %2f will hash to a different value.
    	 * SJP
    	 */
    	//"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~"
        if ( (48 <= c[i] && c[i] <= 57) ||//0-9 - SJP:  This is someone else's Yoda coding, but I've not got a problem with it.
             (65 <= c[i] && c[i] <= 90) ||//abc...xyz
             (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
             (c[i]=='~' || c[i]=='_' || c[i]=='.' || c[i]=='-')
        )
        {
        	//LOG("Appending char %c", c[i]);
            escaped.append( &c[i], 1);
        }
        else
        {
            char buffer[4];

            sprintf(buffer, "%%%02X\0", c[i]);
            escaped.append(buffer, 3);
        }
    }
    return escaped;
};

String Convert::URLdecode(const String& c)
{
	String unescaped = "";
	String encoded  = "";
    int max = c.length();
    int i = 0;
    while(i < max)
    {
        if (c[i] != '%')
        {
            unescaped.append( &c[i++], 1);
        }
        else
        {
        	encoded.append(c.c_str() + i, 2);
        	int val = Convert::hexToInt(encoded.c_str());
        	char ec = (char) val;
        	const char* cc = (const char*)&ec;
        	unescaped.append(cc, 1);
        }
    }
    return unescaped;
};

String Convert::HTMLdecode(String& input)
{
	int compt = 0;
	    while( compt != -1)
	    {
	        if(input.find("<br />",compt) < input.size() && input.find("<br />",compt) >= 0 )
	        {
	            	input.remove(input.find("<br />", compt), 6);

	        }
	        else
	        {
	            compt=-1;
	        }
	        lprintfln(input.c_str());
	    }
}

