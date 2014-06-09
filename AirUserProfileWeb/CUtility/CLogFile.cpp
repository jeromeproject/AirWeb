/**
 *  Logger.cpp
 *
 *  Copyright (C) 2008  David Andrs <pda@jasnapaka.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include "CLogFile.h"

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

TCHAR CLogger::Filename[1024] = _T("message.log");
int CLogger::Level = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogger::CLogger() {
}

CLogger::CLogger(TCHAR* filename, int level/* = 1*/) {
	Open(filename, level);
}

void CLogger::Open(TCHAR* filename, int level/* = 1*/) {
		_tcscpy(Filename, filename);
        Level = level;
}

CLogger::~CLogger() {
}

void CLogger::Log(int level, TCHAR *format, ...) {
		FILE *fp;
        // if the level of a message to log if bigger than watched level => exit (do not log)
        if (level > Level)
                return;
		fp = _tfopen(Filename, _T("a+b"));
		if(!fp)
				return;

		TCHAR datestr[30], timestr[30];
        TCHAR txt[4096];
		TCHAR msg[4096];

		va_list arglist;
        va_start(arglist, format);
        _vstprintf_s(msg, sizeof(msg), format, arglist);
        va_end(arglist);

		struct tm newtime;
		__time32_t aclock;
		TCHAR date_buff[64];
		errno_t errNum;
		_time32( &aclock );   // Get time in seconds.
		_localtime32_s( &newtime, &aclock );   // Convert time to struct tm form.
		//errNum = _tasctime_s(date_buff, sizeof(date_buff), &newtime);
		_tctime32_s( date_buff, 26, & aclock );
		date_buff[24] = 0;
		//printf( "Current time is %s\n", date_buff );
		//GetDateFormat(LOCALE_USER_DEFAULT, NULL, &now, _T("yyyy-MM-dd"), datestr, 20);
        //int l = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &now, _T("HH:mm:ss"), timestr, 20);
		//int swprintf_s(wchar_t *buffer, size_t sizeOfBuffer, const wchar_t *format [,argument]...);
        //_stprintf_s(txt, sizeof(txt), _T("%s %s - %s\r\n"), datestr, timestr, msg);
        _stprintf_s(txt, sizeof(txt), _T("%s - %s\r\n"), date_buff, msg);
		fwrite(txt, 1, _tcslen(txt), fp);

		fclose(fp);

}
