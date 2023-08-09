#pragma once
#ifndef __SDN_LAB_SETTINGS_H
    #define __SDN_LAB_SETTINGS_H
    #define __SDN_LAB_SETTINGS_NN_MEMORY_SIZE 4096
    #include <fstream>
    #include <stdio.h>
    #include <string>
    // headers - 3rd party
    #include "Modules/Toolbox/Error.h"                                  // Error
    namespace SdnLab{
        class Settings{
            private:
                // project related
                const std::string SETTINGS_FILE_PATH = "./Components/Settings.txt";
                const std::string PATH_PREFIX        = "./_dist/";
                std::string PROJECTNAME              = "NULL";
                const std::string FOLDERNAME_DEBUG   = "debug/";
                const std::string FOLDERNAME_TMP     = "tmp/";
                const std::string FOLDERNAME_REPORT  = "report/";
                // overwrite the setting
                void _Overwrite(){
                    std::fstream file_in(Settings::SETTINGS_FILE_PATH, std::fstream::out);
                    file_in << this->PROJECTNAME << '\n';
                    file_in.close();
                };

            public:
                // Report - Memory cost report (StationList & Station)
                const std::string REPORT_MEMORY_COST = "memory.txt";
                // Report - Throughput
                const std::string REPORT_THROUGHPUT_SUFFIX                  = ".csv";
                // Report - Throughput - Minstrel
                const std::string REPORT_THROUGHPUT_MINSTREL_SNN            = "throuput_minstrel_snn_p";
                const std::string REPORT_THROUGHPUT_MINSTREL_SNN_VINCENT    = "throuput_minstrel_snn_vincent_p";
                const std::string REPORT_THROUGHPUT_MINSTREL_SNN_PLUS       = "throuput_minstrel_snn_plus_p";
                const std::string REPORT_THROUGHPUT_MINSTREL_AI_DIST        = "throuput_minstrel_ai_dist_p";
                // track files
                const std::string TRACK_FILE_FORM_SUFFIX = ".csv";
                // yans-wifi-phy.cc
                const std::string TRACK_FILE_YANS_WIFI_PHY                  = "track_yas-wifi-phy.csv";
                const std::string TRACK_FILE_YANS_WIFI_PHY_DATA             = "track_yas-wifi-phy_data.csv";
                const std::string TRACK_FILE_YANS_WIFI_PHY_BEACON           = "track_yas-wifi-phy_beacon.csv";
                // ap-wifi-mac.cc
                const std::string TRACK_FILE_AP_WIFI_MAC_RECE               = "track_ap-wifi-mac_rece_";
                const std::string TRACK_FILE_AP_WIFI_MAC_RECE_ADDR          = "track_ap-wifi-mac_rece_addr.csv";
                const std::string TRACK_FILE_AP_WIFI_MAC_STALIST            = "track_ap-wifi-mac_stationlist-";
                const std::string TRACK_FILE_AP_WIFI_MAC_NN_INPUT           = "track_ap-wifi-mac_NN-INPUT-";
                const std::string TRACK_FILE_AP_WIFI_MAC_NN_OUTPUT          = "track_ap-wifi-mac_NN-OUTPUT-";

                // warning messages
                const std::string ERR_WIFI_MANAGER_UNDEFINED = "=======\n[ERR] wifi manager is not define.\n======";

                /**
                 * constructor 
                 */
                Settings(){
                    std::ifstream file_out(Settings::SETTINGS_FILE_PATH);
                    if (file_out.is_open()){
                        file_out >> this->PROJECTNAME;
                        file_out.close();
                    }else{
                        _Overwrite();
                    }
                };

                /**
                 * Set the Project Name
                 * @strProjectName: the name of the project (supporting const char *)
                 */
                void SetProjectName(std::string strProjectName){
                    SetProjectName(strProjectName.c_str());
                };
                void SetProjectName(const char * strProjectName){
                    if (strProjectName){
                        this->PROJECTNAME = strProjectName;     // set
                        if(this->PROJECTNAME.back() != '/'){
                            this->PROJECTNAME.append("/");
                        }
                        _Overwrite();                           // overwritten setings
                    }
                };
                void SetProjectName(char * strProjectName){
                    if (strProjectName){
                        this->PROJECTNAME = strProjectName;     // set
                        if(this->PROJECTNAME.back() != '/'){
                            this->PROJECTNAME.append("/");
                        }
                        _Overwrite();                           // overwritten setings
                    }
                };
                /* return a path */
                // project
                std::string PathProject(){
                    std::string projectName = "";
                    if (this->PROJECTNAME != "NULL"){
                        projectName = this->PROJECTNAME; 
                    }
                    return this->PATH_PREFIX + projectName;
                };
                // project-debug
                std::string PathProjectDebug(){
                    std::string projectName = "";
                    if (this->PROJECTNAME != "NULL"){
                        projectName = this->PROJECTNAME; 
                    }
                    return this->PATH_PREFIX + projectName + this->FOLDERNAME_DEBUG;
                };
                // project-tmp
                std::string PathProjectTmp(){
                    std::string projectName = "";
                    if (this->PROJECTNAME != "NULL"){
                        projectName = this->PROJECTNAME; 
                    }
                    return this->PATH_PREFIX + projectName + this->FOLDERNAME_TMP;
                };
                // project - report
                std::string PathProjectReport(){
                    std::string projectName = "";
                    if (this->PROJECTNAME != "NULL"){
                        projectName = this->PROJECTNAME; 
                    }
                    return this->PATH_PREFIX + projectName + this->FOLDERNAME_REPORT;
                }

                /*** Static functions to support ***/
                /**
                 * get the STA number in Traffic file
                 * <WARNING> 
                 * This method returns STAIndexMax - STAIndexMin. Therefore, we need to make sure that the station number is in sequence
                 */
                static unsigned int StaNumInTraffic(std::string TrafficPath){
                    // sta id
                    unsigned int staIdTemp = 0;
                    unsigned int staIdMin = 0;
                    unsigned int staIdMax = 0;
                    double traffic;
                    // read from the file
                    std::ifstream file(TrafficPath);
                    // line
                    std::string line;
                    std::stringstream lineStream;
                    if (file.is_open()) {
                        while(getline(file, line)){
                            lineStream << line;
                            lineStream >> staIdTemp >> traffic;
                            if (staIdTemp < staIdMin){
                                staIdMin = staIdTemp;
                            }
                            if (staIdTemp > staIdMax){
                                staIdMax = staIdTemp;
                            }
                            lineStream.clear();
                        }
                        file.close();
                    }else{
                        Toolbox::Error err("/Components", "SystemModel.h", "SystemModel", "StaNumInTraffic", "The traffic file path cannot open.");
                        err.SetType2IllegalParameters();
                        throw err;
                    }
                    // return
                    return staIdMax - staIdMin + 1;
                }
        };
    }
#endif