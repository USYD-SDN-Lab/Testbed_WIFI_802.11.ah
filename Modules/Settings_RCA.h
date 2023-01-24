#pragma once
#ifndef __SETTINGS_RCA_H
    #define __SETTINGS_RCA_H
    #include <stdio.h>
    #include <string>
    #include "Settings.h"

    using namespace std;

    class Settings_RCA : public Settings{
        protected:
            const static string FOLDERNAME_RCA;
        public:
            static string PathDebug(){
                return Settings::PATH_PREFIX + Settings_RCA::FOLDERNAME_RCA + Settings::FOLDERNAME_DEBUG;
            }
            static string PathTmp(){
                return Settings::PATH_PREFIX + Settings_RCA::FOLDERNAME_RCA + Settings::FOLDERNAME_TMP;
            }
    };
    // initialise static members
    // static members cannot be initialised inside a class but only outside
    const string Settings_RCA::FOLDERNAME_RCA = "rca/";
#endif