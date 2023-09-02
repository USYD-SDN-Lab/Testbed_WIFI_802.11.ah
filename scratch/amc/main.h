#pragma once

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>                          // output data into a file
#include <unistd.h>                         // check whether a folder or a file exists using `access`
#include <sys/types.h>                      // create a folder using `mkdir`
#include <sys/stat.h>
#include "ns3/rps.h"
#include <utility>
#include <map>

// 3rd party headers
#include "Modules/Toolbox/FileManager.h"
// namespace - 3rd party
using namespace Toolbox;

// namespace
using namespace std;
using namespace ns3;

int main(int argc, char** argv);