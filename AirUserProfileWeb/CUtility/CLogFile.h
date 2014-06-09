/**
 *  Logger.h
 *
 */
#pragma once
#include "stdafx.h"

//////////////////////////////////////////////////////////
// class CLogger

class CLogger {
public:
        CLogger(TCHAR* filename, int level = 1);
        CLogger();
        virtual ~CLogger();

        static void Open(TCHAR* filename, int level = 1);
        static void Log(int level, TCHAR *str, ...);

		static TCHAR Filename[1024];
        static int Level;
};
