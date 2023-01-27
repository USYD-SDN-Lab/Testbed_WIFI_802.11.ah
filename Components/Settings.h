#pragma once
#ifndef __SETTINGS_H
    #define __SETTINGS_H
    #include <stdio.h>
    #include <string>
    // define the paths & foldernames to inherited by children classes
    #define SETTINGS_PATH_PREFIX        "./__dist__/"
    #define SETTINGS_PROJECTNAME        ""
    #define SETTINGS_FOLDERNAME_DEBUG   "debug/"
    #define SETTINGS_FOLDERNAME_TMP     "tmp/"

    using namespace std;

    class Settings{
        private:
            const static string PATH_PREFIX;
            const static string FOLDERNAME_DEBUG;
            const static string FOLDERNAME_TMP;
            static string PROJECT_NAME;

        public:
            static void setProjectName(const char * strProjectName){
                if (!strProjectName){
                    Settings::PROJECT_NAME = "";
                }else{
                    Settings::PROJECT_NAME = strProjectName;
                    if(Settings::PROJECT_NAME.back() != '/'){
                        Settings::PROJECT_NAME.append("/");
                    }
                }
            }
            /**
             * return this project folder path
             */
            static string PathProject(){
                return Settings::PATH_PREFIX + Settings::PROJECT_NAME;
            }
            /**
             * return project-debug folder path
             */
            static string PathProjectDebug(){
                return Settings::PATH_PREFIX + Settings::PROJECT_NAME + Settings::FOLDERNAME_DEBUG;
            }
            /**
             * return project-tmp folder path
             */
            static string PathProjectTmp(){
                return Settings::PATH_PREFIX + Settings::PROJECT_NAME + Settings::FOLDERNAME_TMP;
            }
    };
    // initialise static members
    // static members cannot be initialised inside a class but only outside
    const string Settings::PATH_PREFIX          = SETTINGS_PATH_PREFIX;
    const string Settings::FOLDERNAME_DEBUG     = SETTINGS_FOLDERNAME_DEBUG;
    const string Settings::FOLDERNAME_TMP       = SETTINGS_FOLDERNAME_TMP;
    string Settings::PROJECT_NAME               = SETTINGS_PROJECTNAME;
#endif