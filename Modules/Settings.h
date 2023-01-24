#pragma once
#ifndef __SETTINGS_H
    #define __SETTINGS_H
    #include <stdio.h>
    #include <string>
    // define the paths & foldernames to inherited by children classes
    #define SETTINGS_PATH_PREFIX        "./__dist__/"
    #define SETTINGS_FOLDERNAME_DEBUG   "debug/"
    #define SETTINGS_FOLDERNAME_TMP     "tmp/"

    using namespace std;

    class Settings{
        protected:
            const static string PATH_PREFIX;
            const static string FOLDERNAME_DEBUG;
            const static string FOLDERNAME_TMP;

        public:
            static string PathDebug(){
                return Settings::PATH_PREFIX + Settings::FOLDERNAME_DEBUG;
            }
            static string PathTmp(){
                return Settings::PATH_PREFIX + Settings::FOLDERNAME_TMP;
            }
    };
    // initialise static members
    // static members cannot be initialised inside a class but only outside
    const string Settings::PATH_PREFIX          = SETTINGS_PATH_PREFIX;
    const string Settings::FOLDERNAME_DEBUG     = SETTINGS_FOLDERNAME_DEBUG;
    const string Settings::FOLDERNAME_TMP       = SETTINGS_FOLDERNAME_TMP;
#endif