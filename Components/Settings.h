#pragma once
#ifndef __SDN_LAB_SETTINGS_H
    #define __SDN_LAB_SETTINGS_H
    #define __SDN_LAB_SETTINGS_NN_MEMORY_SIZE 4096
    #include <fstream>
    #include <stdio.h>
    #include <string>
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
                // NN
                const std::string PATH_SNN           = "./_build/snn";
                // overwrite the setting
                void _Overwrite(){
                    std::fstream file_in(Settings::SETTINGS_FILE_PATH, std::fstream::out);
                    file_in << this->PROJECTNAME << '\n';
                    file_in.close();
                };

            public:
                // Memory cost report (StationList & Station)
                const std::string REPORT_MEMORY_COST = "memory.txt";
                const std::string REPORT_MEMORY_COST_BEACON = "memory-";
                const std::string REPORT_MEMORY_COST_BEACON_SUFFIX = ".csv";
                // yans-wifi-phy.cc
                const std::string TRACK_FILE_YANS_WIFI_PHY                  = "track_yas-wifi-phy.csv";
                const std::string TRACK_FILE_YANS_WIFI_PHY_DATA             = "track_yas-wifi-phy_data.csv";
                const std::string TRACK_FILE_YANS_WIFI_PHY_DATA_BEACON      = "track_yas-wifi-phy_data-beacon.csv";
                // ap-wifi-mac.cc
                const std::string TRACK_FILE_AP_WIFI_MAC_RECE = "track_ap-wifi-mac_rece.csv";
                const std::string TRACK_FILE_AP_WIFI_MAC_RECE_ADDR = "track_ap-wifi-mac_rece_addr.csv";

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
                void SetProjectName(const char * strProjectName){
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
                // project - report - StationList (per beacon)
                std::string PathProjectReportStationListPerBeacon(unsigned int i){
                    std::string projectName = "";
                    if (this->PROJECTNAME != "NULL"){
                        projectName = this->PROJECTNAME; 
                    }
                    return this->PATH_PREFIX + projectName + this->FOLDERNAME_REPORT + this->REPORT_MEMORY_COST_BEACON + std::to_string(i) + this->REPORT_MEMORY_COST_BEACON_SUFFIX;
                }
        };
    }
#endif