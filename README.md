# WIFI 802.11ah (ns-3)
- [1 Installation and usage instructions](#1-installation-and-usage-instructions)
  - [1.1 Clone the project from git](#11-clone-the-project-from-git)
  - [1.2 Install Anaconda](#12-install-anaconda)
  - [1.3 Train Nerual Network](#13-train-nerual-network)
  - [1.4 Add Python interface (for ns3-ai)](#14-add-python-interface-for-ns3-ai)
  - [1.5 Follow the instructions on https://www.nsnam.org/wiki/Installation to prepare all dependencies. <br>](#15-follow-the-instructions-on-httpswwwnsnamorgwikiinstallation-to-prepare-all-dependencies-br)
  - [1.6 Clean (Optional)](#16-clean-optional)
  - [1.7 Generate RAW (Optional)](#17-generate-raw-optional)
  - [1.8 Configure, Build & Run](#18-configure-build--run)
    - [1.8.1 Macros](#181-macros)
    - [1.8.2 Parameters](#182-parameters)
    - [1.8.3 Debug & Test](#183-debug--test)
    - [1.8.4 Run](#184-run)
- [2 Further Reading](#2-further-reading)
  - [2.1 Mac Layer](#21-mac-layer)
    - [2.1.1 Mac Frame Header](#211-mac-frame-header)
  - [2.2 A-MPDU vs A-MSDU](#22-a-mpdu-vs-a-msdu)
- [3 Protocol Stack](#3-protocol-stack)
  - [3.1 `WifiMacHelper` -> `APMac/StaMac`](#31-wifimachelper---apmacstamac)
  - [3.2 `MacLow -> MacRxMiddle -> RegularWifiMac` & `RegularWifiMac-> DcaTxop/DcaManager -> MacLow`](#32-maclow---macrxmiddle---regularwifimac--regularwifimac--dcatxopdcamanager---maclow)
  - [3/3 Uplink](#33-uplink)
  - [3.4 Beacon Broadcast](#34-beacon-broadcast)
- [4 **Additive/modified files & folders from the original fork, maintainer must keep those files & folders**](#4-additivemodified-files--folders-from-the-original-fork-maintainer-must-keep-those-files--folders)
  - [4.1 General Modified Files](#41-general-modified-files)
  - [4.2 Removed files](#42-removed-files)
  - [4.3 3rd Party Modules](#43-3rd-party-modules)
  - [4.4 Components - all self-defined components](#44-components---all-self-defined-components)
  - [4.5 Updated Source File (adding new functions)](#45-updated-source-file-adding-new-functions)
    - [4.5.1 Application - RCA](#451-application---rca)
    - [4.5.2 Rate Adaption](#452-rate-adaption)
    - [4.5.3 Mac High](#453-mac-high)
    - [4.5.4 Mac Middle](#454-mac-middle)
    - [4.5.5 Mac Middle <-> Mac Low](#455-mac-middle---mac-low)
    - [4.5.6 Mac Low](#456-mac-low)
    - [4.5.7 Phy](#457-phy)
    - [4.5.7 Channel](#457-channel)
- [5 Potential Problems](#5-potential-problems)

This repository is vessal of the IEEE802.11ah (Wi-Fi HaLow) protocol for the NS-3 network simulator, which comes from [LeTian and his associates](https://github.com/imec-idlab/IEEE-802.11ah-ns-3). Its NS-3 version is `3.23` with multiple modules updated to `3.25` to incorporate congestion control fixes for TCP traffic.
* This module includes support for:
	* Restricted Access Window (RAW) with interface for dynamic configuration
	* Traffic Indication Map (TIM) segmentation 
	* Energy consumption model
	* Adaptive Modulation and Coding Scheme (MCS)
## 1 Installation and usage instructions
### 1.1 Clone the project from git
### 1.2 Install Anaconda
After installing `Anaconda`, we need to create two environments.
* `ns3`<br>
This environment is used to compile `ns3`.
```sh
conda create -n ns3 python=2.7
```
* `ai`<br>
This environment is used to run scripts relying on `ns3-ai`
```sh
conda create -n ai python=3.9
# activate ai
conda activate ai
# pandas
pip install pandas
# tensorflow
conda install -c conda-forge cudatoolkit=11.2 cudnn=8.1.0
pip install tensorflow
```
### 1.3 Train Nerual Network
* train SNN (in `ai`)
```sh
python Components/AI_SNN_Train.py
```
### 1.4 Add Python interface (for ns3-ai)
```sh
# switch to ai
conda activate ai
# add interfaces
cd src/ns3-ai/py_interface
pip install . --user
```
### 1.5 Follow the instructions on https://www.nsnam.org/wiki/Installation to prepare all dependencies. <br>
For `Ubuntu 18.04`, those are:<br>
(please note that `sudo apt-get -y install gsl-bin libgsl2 libgsl2:i386` is changed into `sudo apt-get -y install gsl-bin` because the other two packets are missing in `Ubuntu 18.04`)
```sh
sudo apt-get update
sudo apt-get -y install gcc g++ python-dev
sudo apt-get -y install qt4-dev-tools libqt4-dev
sudo apt-get -y install mercurial
sudo apt-get -y install bzr
sudo apt-get -y install cmake libc6-dev libc6-dev-i386 g++-multilib
sudo apt-get -y install gdb valgrind
sudo apt-get -y install gsl-bin
sudo apt-get -y install flex bison libfl-dev
sudo apt-get -y install tcpdump
sudo apt-get -y install sqlite sqlite3 libsqlite3-dev
sudo apt-get -y install libxml2 libxml2-dev
sudo apt-get -y install libgtk2.0-0 libgtk2.0-dev
sudo apt-get -y install vtun lxc
```
### 1.6 Clean (Optional)
This step is to clean dynamic libraries (`*.so` files) in `build `. If you haven't built this project, you can jump this step.
```sh
./waf clean
```
### 1.7 Generate RAW (Optional)
If you have already created RAW configuration files, you should jump this step and use them in [run the simulation](#110-run-the-simulation-if-you-use-ahvisualizer-start-it-first)
```sh
# Contentions 2 (static)
./waf --run "RAW-generate --NRawSta=128 --NGroup=32 --NumSlot=1 --RAWConfigPath='./OptimalRawGroup/RawConfig-rca-contention-2.txt' --beaconinterval=1000000 --pageSliceCount=2 --pageSliceLen=1"
```
### 1.8 Configure, Build & Run
If you use [ahVisualizer](https://github.com/imec-idlab/ahVisualizer) start it first
#### 1.8.1 Macros
* File Control
	* `__SDN_LAB_PROJECTNAME` the project name
* For **rate control**
	* `__SDN_LAB_RA_CONST_RATE` to use the constant rate that is defined in `/scratch/Configuration.h`
	* `__SDN_LAB_RA_AMRR` to activate Adaptive Multi Rate Retry (AMRR).
	* `__SDN_LAB_RA_AARF` to activate Adaptive ARF (AARF). Here, ARF is *Auto Rate Fallback*.
	* For **Minstrel**
		* `__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE`: the rate that Minstrel looks for a new rate (default at 10)
		* `__SDN_LAB_RA_MINSTREL` to activate Minstrel.
		* `__SDN_LAB_RA_MINSTREL_SNN_VINCENT` to activate `SNN` in Vincent version (where the overhead of SNN is not considered)
		* `__SDN_LAB_RA_MINSTREL_SNN` to activate `SNN`
		* `__SDN_LAB_RA_MINSTREL_SNN_PLUS` to activate `SNN_PLUS`
		* `__SDN_LAB_RA_MINSTREL_AI_DIST` to activate `MINSTREL_AI_DIST` (where **AI** is implemented in **STAs** and **DIST** means *distributes*).
* `__SDN_LAB_DEBUG` to activate debug mode<br>
	**rate control** is set to `__SDN_LAB_RA_MINSTREL`<br>
	`src\wifi\ap-wifi-mac`: `ApWifiMac()`, `SendOneBeacon()`, `Receive()`
	`src\wifi\yans-wifi-phy`: `EndReceive()`
	* `-D__SDN_LAB_PHY_PACKET_SIZE_DATA=` to track physical layer data packet size
		* `-D__SDN_LAB_PHY_PACKET_SIZE_BEACON=` to ***additively*** track physical beacon packet size 
> The macro definition is added in `CXXFLAGS`. For example, `CXXFLAGS="-Dxxx=yy"`, `xxx` is the macro definition and `yy` is the replacement of `xxx`. Please note that the replacement is not necessary especially in the conditional compilation.
#### 1.8.2 Parameters
RAW configuration must be in line with TIM and page configuration. If a RAW group is reserved for a station in beacon interval that does not correspond to its TIM, station will be asleep during that RAW.
* RAW related parameters
	* `NRawSta`: Number of stations supporting RAW. NRawSta equals the largest AID specified in RAWConfigFile.
	* `RAWConfigFile`: RAW configuration is stored in this file.<br>
		The default RAWConfigFile "./OptimalRawGroup/RawConfig-32-2-2.txt" sets 2 RPSs (RAW Parameter Sets) to be broadcasted with beacons in cycles (e.g. Beacon1: RPS1, Beacon2: RPS2, Beacon3: RPS1, Beacon4: RPS2, ...). The first RPS contains 2 RAW groups for 63 stations, each RAW group contains 2 RAW slots. The second RPS contains 1 RAW group for 4 stations (64-67), where the RAW group contains 3 slots. This file consists of 6 lines:
		```
		2
		2
		0	  1	  1	  200	  2	  0	  1	29
		0	  1	  1	  200	  2	  0	  30	63
		1
		0	  1	  1	  220	  3	  0	  64	67
		```
		line 1: NRPS = number of RPS elements<be>
		line x: NRAW_k = number of RAW Groups in the RPS k; x = (NRAW_0 + NRAW_1 + ... + NRAW_(k-1)) + 1 + k; NRAW_0 = 0
		> for RPS number k=1, line x=0+1+1=2 contains information on number of RAW groups in the RPS

		> for RPS number k=2 and NRAW_1=2 (from line 2), line x=0+2+1+2=5 contains information on number of RAW groups in the RPS
			
		line 3, line 4 and line 6: configuration of each individual RAW group, including
		* `RawControl`: Whether RAW can be accessed by any stations within the RAW group or only the paged stations within the RAW group.  
		* `CrossSlotBoundary`: Whether STAs are allowed to transmit after the assigned RAW slot boundary.
		* `SlotFormat`: Format of RAW slot count.                 
		* `NRawSlotCount`: Used for calculation of RAW slot duration.   
		* `NRawSlotNum`: Number of slots per RAW group.                     
		* `Page`: Page index of the subset of AIDs.
		* `Aid_start`: Station with the lowest AID allocated in the RAW.
		* `Aid_end`: Station with the highest AID allocated in the RAW.
		Notes:   
		* `RawControl`: currently set to 0, RAW can be accessed by any stations within the RAW group.            
		* `CrossSlotBoundary`: currenty set to 1, only cross slot boundary are supported.                    
		* **RAW slot duration** = 500 us + `NRawSlotCount` * 120 us, `NRawSlotCount` is y = 11(8) bits length when `SlotFormat` is set to 1(0), and `NRawSlotNum` is (14 - y) bits length.                                     
		* The above RAWConfigFile assumes BeaconInterval is 102400 us. Users can adjust the parameters based on their own needs.
* Wi-Fi mode parameters
	* `DataMode`: Data mode.
	* `datarate`: Data rate of data mode.  
	* `bandWidth`: BandWidth of data mode.  
	> Relation between the above 3 parameters and MCS is described in file "MCStoWifiMode".
* Other parameters      
	* `SimulationTime`:     Simulatiom time in seconds after all stations get associated with AP.  
	* `payloadSize`:        Size of payload.                   
	* `BeaconInterval`:     Beacon interval time in us.    
	* `UdpInterval`:        Traffic mode, station send one packet every UdpInterval seconds.  
	* `Nsta`: Number of total stations.  
	* `rho`: Maximal distance between AP and stations.   
	* `seed`: Seed of RandomVariableStream.
	* `TrafficPath`: Include traffic of each stations, packet sending interval can be automatically calcualted based on payloadSize. The above TrafficPath "./OptimalRawGroup/traffic/data-32-0.82.txt" contains traffic of 32 stations, and the total traffic is 0.82 Mbps.
	* `S1g1MfieldEnabled`: Packet using 1 Mhz bandwidth if set to "true".
* TIM and page slice parameters
	* `pagePeriod`: Number of Beacon Intervals between DTIM beacons that carry Page Slice element for the associated page
	* `pageIndex`: Index of the page (0-3) whose slices are served during beacon intervals within a page period, default value is 0
	* `pageSliceLength`: Number of blocks in each TIM for the associated page except for the last TIM (1-31)
	* `pageSliceCount`: Number of TIMs in a single page period (0-31), value 0 has special meaning
	* `blockOffset`: The 1st page slice starts with the block with blockOffset number, default value is 0
	* `timOffset`: Offset in number of Beacon Intervals from the DTIM that carries the first page slice of the page, default value is 0
	For example, `./waf --run "test --pagePeriod=4 --pageSliceLength=8 --pageSliceCount=4"`<br>
	pagePeriod=4: every 4th beacon is DTIM beacon that carries Page Slice element for pageIndex=0<br>
    pageSliceLength=8: each page slice cosists of 8 blocks, meaning that each page slice (PS) accomodates up to 512 stations (8 blocks * 8 subblocks * 8 stations)<br>
    > (slice0: 1-512, slice1: 513-1024, slice2: 1025-1536, slice3: 1537-2048)

    > The last page slice can have different length.

	pageSliceCount=4: 4 TIMs are scheduled in one page period.<br>
> To configure a single page slice (whole page encoded in a single page slice), it is neccessary to set `pageSliceCount` to 0 and `pageSliceLength` to 1.

#### 1.8.3 Debug & Test
* Testing the protocol stack
	nominate the packet size in the physical layer
	```sh
	CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_PHY_PACKET_SIZE_DATA=166 -D__SDN_LAB_PHY_PACKET_SIZE_BEACON=71" ./waf configure --disable-examples --disable-tests
	```
	use 1 STA and simulate for 2 seconds
	```sh
	./waf --run "rca --seed=1 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"
	```
* Neural network based rate adaption algorithm<br>
	* Minstrel-SNN (Vincent)
		```sh
		CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_RA_MINSTREL_SNN_VINCENT -D__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE=25" ./waf configure --disable-examples --disable-tests
		```
		use 1 STA and simulate for 2 seconds
		```sh
		./waf --run "rca --seed=1 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"
		```
	* Minstrel-SNN
		```sh
		CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_RA_MINSTREL_SNN" ./waf configure --disable-examples --disable-tests
		```
		use 1 STA and simulate for 2 seconds
		```sh
		./waf --run "rca --seed=1 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"
		```
	* Minstrel-SNN+
		```sh
		CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_RA_MINSTREL_SNN_PLUS" ./waf configure --disable-examples --disable-tests
		```
		use 1 STA and simulate for 2 seconds
		```sh
		./waf --run "rca --seed=1 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"
		```
	* Minstrel-AI-Dist
		```sh 
		CXXFLAGS="-std=c++11 -D__SDN_LAB_DEBUG -D__SDN_LAB_RA_MINSTREL_AI_DIST" ./waf configure --disable-examples --disable-tests
		```
		use 1 STA and simulate for 2 seconds
		```sh
		./waf --run "rca --seed=1 --simulationTime=2 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"
		```
#### 1.8.4 Run
* Rate control Algorithm (RCA)<br>
	The 802.11ah nodes can always use the same MCS as specified by "Wi-Fi mode parameters" when "ConstantRateWifiManager" is used. The nodes can also adapt the MCSs dynamically when rata control algorithm is used. Details about Rate control Algorithms can be found on https://www.nsnam.org/docs/models/html/wifi-design.html#rate-control-algorithms.
	```sh
	./waf --run "rca --seed=1 --simulationTime=60 --payloadSize=256"
	```
* Vincent SNN & others rates<br>
	This recovers the simulation results in `Improving The Minstrel Rate Adaptation Algorithm using Shallow Neural Networks in IEEE 802.11ah`
	```sh
	./Scripts/vincent_scenario.sh
	```
	Open another terminal to activate `SNN`
	```sh
	python Components/AI_SNN.py
	```

## 2 Further Reading
For more information on the implementation of the IEEE 802.11ah module for ns-3, check [recent WNS3 paper on ResearchGate](https://www.researchgate.net/publication/324910418_Extension_of_the_IEEE_80211ah_ns-3_Simulation_Module).
> Le Tian, Amina Sljivo, Serena Santi, Eli De Poorter, Jeroen Hoebeke, Jeroen Famaey. **Extension of the IEEE 802.11ah NS-3 Simulation Module.** Workshop on ns-3 (WNS3), 2018.

### 2.1 Mac Layer
#### 2.1.1 Mac Frame Header
* `Type` & `Subtype`<br>
	| Type | Type Value | Subtype | Subtype Value |
	| -- | -- | -- | -- |
	| Management | 00 (0) | | |
	| Control | 01(1) | Control Wrapper | 0111(7) |
	| Control | 01(1) | Block Ack Request | 1000(8) |
	| Control | 01(1) | Block Ack | 1001(9) |
	| Control | 01(1) | PS-Poll | 1010(10) |
	| Control | 01(1) | RTC | 1011(11) |
	| Control | 01(1) | CTS | 1100(12) |
	| Control | 01(1) | ACK | 1101(13) |
	| Data | 10(2) | Data | 0000(0) |
	| Data | 10(2) | QoS Data | 1000(8) |
	| Data | 10(2) | QoS Data + CF-ACK | 1001(9) |
	| Data | 10(2) | QoS Data + QoS Data + CF-Poll | 1010(10) |
	| Data | 10(2) | QoS Data + CF-ACK + CF-Poll | 1011(11) |
	| Data | 10(2) | QoS Null (no data) | 1100(12) |
	| Data | 10(2) | QoS CF-Poll (no data) | 1110(14) |
	| Data | 10(2) | QoS CF-ACK + CF-Poll (no data) | 1111(8) |
	| Extension | 11(3) | | |
	* `PS-Poll`<br>
	The `PS-POLL Mechanism` is the Legacy Power save mechanism. As the name suggests `PS-POLL` stands for Power Save Polling. The Access point uses the TIM information element to indicate to the station that there is unicast data buffered for the WLAN station at the Access Point.<br>
	The station wakes up at Listen Interval/DTIM time to receive the beacon from the Access Point.<br>
	It then checks the TIM Information element to check whether its Association ID is set in The TIM Information element.<br>
	If the Association ID of the Station is set – then it sends a specific frame to retrieve data from the AP. **This Frame is termed as a PS-Poll frame**.<br>
	On receiving a `PS-POLL` frame from the 802.11 station, the AP would ACK the PS-Poll frame and then sends a single data packet to the Station. If there are more data packets queued for the 802.11 Station at the Access Point, the Access Point sets the `More Data bit` in the frame control to true. <br>
	The WLAN (802.11) station processes the received data frame and on processing the frame control field realizes that it has more data queued at the Access point. The WLAN (802.11) station will send another `PS-Poll` frame to retrieve another data packet. When the `More Data bit` is set to zero – the Access Point has no more data uffered for the WLAN station and the station can go to sleep.<br>
	In legacy Power save devices, for transmitting each `PS-Poll` frame the 802.11 station has to contend for the medium.
* `ToDS` & `FromDS`<br>
	There are 5 Mac address: 
	* `SA`: source address
	* `DA`: destination address
	* `TA`: transmitter address
	* `RA`: receiver address
	* `BSSID`: basic service set identifier.
	> BSSID is a Layer 2 identifier of the BSS

	![5-mac-addr](/Img/futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_5-mac-addr.png)<br><br>
	**There are 4 scenarios**<br>
	![4-to-ds-from-ds](/Img/futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_4-to-ds-from-ds.jpg)<br>
	* To DS = 0 and From DS = 0<br>
	A simple beacon frame, which is originated from the AP radio MAC address (BSSID, transmitter and source addresses) to all mobile clients (receiver and destination addresses). **Management, control and IBSS(ad-hoc)** frames are dedicated to wireless networking only.<br>
	Address 1 – RA=DA, ff:ff:ff:ff:ff:ff – broadcast to all mobile clients<br>
	Address 2 – TA=SA, 84:24:8d:c1:24:20 – AP BSSID<br>
	Address 3 – BSSID, 84:24:8d:c1:24:20 – AP BSSID<br>
	* To DS = 1 and From DS = 0<br>
	A wireless data frame is being sent from a mobile client to wired network. For example, ICMP echo request ping test from the mobile client  via the AP to the default gateway.<br>
	Address 1 – RA=BSSID, 84:24:8d:c1:24:20 – AP BSSID<br>
	Address 2 – TA=SA, cc:44:63:1b:2d:fa – mobile client’s MAC address<br> 
	Address 3 – DA,  74:8e:f8:4f:02:76
	* To DS = 0 and From DS = 1<br>
	A wireless data frame is being sent from the wired network to the mobile client associated to the AP.<br>
	Address 1 – RA=DA, cc:44:63:1b:2d:fa – mobile client’s MAC address<br>
	Address 2 – TA=BSSID, 84:24:8d:c1:24:20 – AP BSSID<br>
	Address 3 – SA,  74:8e:f8:4f:02:76
	* To DS = 1 and From DS = 1<br>
	Typical mesh environment (AP to AP).<br>
	Address 1 – RA, f0:5c:19:65:64:30 – AP BSSID<br>
	Address 2 – TA, f0:5c:19:65:60:f1 – AP BSSID<br>
	Address 3 – DA, cc:44:63:1b:2d:fa – mobile client’s MAC address<br>
	Address 4 – SA, 74:8e:f8:4f:02:76<br>
### 2.2 A-MPDU vs A-MSDU
![1](/Img/futher-reading_mac-layer_ampdu-vs-amsdu_1.jpg)
![2](/Img/futher-reading_mac-layer_ampdu-vs-amsdu_2.jpg)

## 3 Protocol Stack
### 3.1 `WifiMacHelper` -> `APMac/StaMac`
* `/scatch/rca/s1g-rca.cc`
	In the main function, we set `Mac Layer name` and `other parameters` and transfer this parameter to `WifiHelper::Install`<br>
	`S1gWifiMacHelper` inherits `QosWifiMacHelper`; `QosWifiMacHelper` has a memeber `ObjectFactory m_mac` that accepts `Mac Layer name` and `other parameters`.
	```c++
	...
	int main(int argc, char *argv[]){
		...
		WifiHelper wifi = WifiHelper::Default();
		S1gWifiMacHelper mac = S1gWifiMacHelper::Default();
		// set the configuration of this mac
		mac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid),
					"BeaconInterval", TimeValue (MicroSeconds(config.BeaconInterval)),
					"NRawStations", UintegerValue (config.NRawSta),
					"RPSsetup", RPSVectorValue (config.rps),
					"PageSliceSet", pageSliceValue (config.pageS),
					"TIMSet", TIMValue (config.tim));
		NetDeviceContainer apDevice = wifi.Install(phy, mac, wifiApNode);
		// set Sta Mac & Phy
		mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
		NetDeviceContainer staDevice = wifi.Install(phy, mac, wifiStaNode);
		...
	}
	```
	* `/src/wifi/helper/wifi-helper.cc`
		`macHelper.Create ()` use `ObjectFactory m_mac` to create the actual Mac object
		```c++
		NetDeviceContainer WifiHelper::Install (const WifiPhyHelper &phyHelper, const WifiMacHelper &macHelper, NodeContainer c) const{
			NetDeviceContainer devices;
			for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
				{
				Ptr<Node> node = *i;
				Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
				Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
				Ptr<WifiMac> mac = macHelper.Create ();
				Ptr<WifiPhy> phy = phyHelper.Create (node, device);
				mac->SetAddress (Mac48Address::Allocate ());
				mac->ConfigureStandard (m_standard);
				phy->ConfigureStandard (m_standard);
				device->SetMac (mac);
				device->SetPhy (phy);
				device->SetRemoteStationManager (manager);
				node->AddDevice (device);
				devices.Add (device);
				NS_LOG_DEBUG ("node=" << node << ", mob=" << node->GetObject<MobilityModel> ());
				}
			return devices;
		}
		```
		* `/src/wifi/helper/qos-wifi-mac-helper.cc`
			```c++
			Ptr<WifiMac> QosWifiMacHelper::Create (void) const{
				Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();

				Setup (mac, AC_VO, "VO_EdcaTxopN");
				Setup (mac, AC_VI, "VI_EdcaTxopN");
				Setup (mac, AC_BE, "BE_EdcaTxopN");
				Setup (mac, AC_BK, "BK_EdcaTxopN");

				return mac;
			}
			```
### 3.2 `MacLow -> MacRxMiddle -> RegularWifiMac` & `RegularWifiMac-> DcaTxop/DcaManager -> MacLow`
In `src/wifi/model/regular-wifi-mac.cc`<br>
When `ApWifiMac` and `StaWifiMac` initialise themselves, they will call their parent constructor at `m_rxMiddle->SetForwardCallback (MakeCallback (&RegularWifiMac::Receive, this));`. Here `this` points at the instances of `ApWifiMac` and `StaWifiMac` even in their parent constructor.
```c++
class ApWifiMac : public RegularWifiMac;
...
class StaWifiMac : public RegularWifiMac;
...
RegularWifiMac::RegularWifiMac ()
{
	...
	m_rxMiddle = new MacRxMiddle ();
	m_rxMiddle->SetForwardCallback (MakeCallback (&RegularWifiMac::Receive, this));

	m_txMiddle = new MacTxMiddle ();

	m_low = CreateObject<MacLow> ();
	m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));

	m_dcfManager = new DcfManager ();
	m_dcfManager->SetupLowListener (m_low);

	m_dca = CreateObject<DcaTxop> ();
	m_dca->SetLow (m_low);
	m_dca->SetManager (m_dcfManager);
	m_dca->SetTxMiddle (m_txMiddle);
	m_dca->SetTxOkCallback (MakeCallback (&RegularWifiMac::TxOk, this));
	m_dca->SetTxFailedCallback (MakeCallback (&RegularWifiMac::TxFailed, this));
	m_dca->GetQueue()->TraceConnect("PacketDropped", "", MakeCallback(&RegularWifiMac::OnQueuePacketDropped, this));
  	m_dca->TraceConnect("Collision", "", MakeCallback(&RegularWifiMac::OnCollision, this));
  	m_dca->TraceConnect("TransmissionWillCrossRAWBoundary", "", MakeCallback(&RegularWifiMac::OnTransmissionWillCrossRAWBoundary, this));
	...
}
```
### 3/3 Uplink
![uplink](/Img/sta_ap-receive.jpg)
### 3.4 Beacon Broadcast
![beacon broadcast](/Img/ap-beacon.jpg)

## 4 **Additive/modified files & folders from the original fork, maintainer must keep those files & folders**
### 4.1 General Modified Files
* `.gitignore`
* `README.md`
* `LICENSE`
### 4.2 Removed files
`optimal-RAW-algorithm-fixedraw.sh`, `README`, `RELEASE_NOTES`, `run.pbs`
### 4.3 3rd Party Modules
* [Modules/Toolbox](https://github.com/USYD-SDN-Lab/Toolbox)
* [Modules/RA_Minstrel_SNN](https://github.com/USYD-SDN-Lab/RA_Minstrel_SNN)
* [src/ns3-ai](https://github.com/hust-diangroup/ns3-ai)

### 4.4 Components - all self-defined components
* `Mac.h`: define the broadcast mac address
* `Mcs.h`: define the MCS we use and the index we use. (Also, Vincent's index format is preserved)
* `Setting.h` : set the configuration across all layers and components
* `PacketContext.h`: the context of a packet across the physical layer to the MAC layer
* `Station.h`: store the data associated with station and 
* `StationList.h`: combine all stations into a list to manage
* `NNData.h`: the data structure shared between *`Python`* and *`C/C++`*
### 4.5 Updated Source File (adding new functions)
#### 4.5.1 Application - RCA
* `scatch/rca/s1g-rca`<br>
	`s1g-rca.h`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/StationList.h"
	#include "Components/Station.h"
	...
	// extra namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	...
	/*** self-defined varables ***/
	FileManager fm;
	Settings settings;
	/*** self-defined functions ***/
	void PrintStatistics(double pastTime, unsigned int pastSentPackets, unsigned int pastSuccessfulPackets);
	```
	`s1g-rca.cc`
	```c++
	...
	/*** self-defined marcos ***/
	// look around rate
	#ifndef __SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE
		#define __SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE 10
	#endif
	// __SDN_LAB_SET_WIFIMANAGER(dr): 		define the wifimanager
	// __SDN_LAB_SET_STATISTIC_PATH(sets): 	define the file path to storage statistics
	#ifdef __SDN_LAB_DEBUG
		#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::MinstrelWifiManager")
		#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_MINSTREL
	#else
		#if defined(__SDN_LAB_RA_CONST_RATE)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", dr, "ControlMode", dr)
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_CONSTRATE
		#elif defined(__SDN_LAB_RA_AMRR) 
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::AmrrWifiManager")
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_AMRR
		#elif defined(__SDN_LAB_RA_AARF)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::AarfWifiManager")
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_AARF
		#elif defined(__SDN_LAB_RA_MINSTREL)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::MinstrelWifiManager");
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_MINSTREL + sets.__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE + sets.REPORT_THROUGHPUT_SUFFIX;
		#elif defined(__SDN_LAB_RA_MINSTREL_SNN)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::MinstrelWifiManager");
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_MINSTREL_SNN + sets.__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE + sets.REPORT_THROUGHPUT_SUFFIX;
		#elif defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::MinstrelWifiManager");
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_MINSTREL_SNN_VINCENT + sets.__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE + sets.REPORT_THROUGHPUT_SUFFIX;
		#elif defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::MinstrelWifiManager");
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_MINSTREL_SNN_PLUS + sets.__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE + sets.REPORT_THROUGHPUT_SUFFIX;
		#elif defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
			#define __SDN_LAB_SET_WIFIMANAGER(dr) wifi.SetRemoteStationManager("ns3::MinstrelWifiManager");
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) sets.PathProjectReport() + sets.REPORT_THROUGHPUT_MINSTREL_AI_DIST + sets.__SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE + sets.REPORT_THROUGHPUT_SUFFIX;
		#else
			#define __SDN_LAB_SET_WIFIMANAGER(dr) \
				cout << settings.ERR_WIFI_MANAGER_UNDEFINED << endl; \
				NS_ASSERT(false);
			#define __SDN_LAB_SET_STATISTIC_PATH(sets) ""
		#endif
	#endif
	...
	// print the throughput
	void PrintStatistics(double pastTime, unsigned int pastSentPackets, unsigned int pastSuccessfulPackets){
		// set the throughput file path based on the wifi manager
		string path = __SDN_LAB_SET_STATISTIC_PATH(settings);
		// do next when the path is not empty
		if(!path.empty()){
			// get the current time
			double curTime = Simulator::Now().GetSeconds();
			// calculate statistics
			// calculate statistics - total
			unsigned int totalSentPackets = 0;
			unsigned int totalSuccessfulPackets = 0; 
			double totalThroughput = 0;
			for (int i = 0; i < config.Nsta; i++){
				totalSentPackets += stats.get(i).NumberOfSentPackets;
				totalSuccessfulPackets += stats.get(i).NumberOfSuccessfulPackets;
			}
			totalThroughput = totalSuccessfulPackets * config.payloadSize * 8 / (curTime * 1000000.0);
			// calculate statistics - transient
			unsigned int curSentPackets = totalSentPackets - pastSentPackets;
			unsigned int curSuccessfulPackets = totalSuccessfulPackets - pastSuccessfulPackets; 
			double curThroughput = curSuccessfulPackets * config.payloadSize * 8 / ((curTime - pastTime) * 1000000.0);
			// write to file
			if(fm.Open(path) == 200){
				fm.AddCSVItem(curTime);
				// additive
				fm.AddCSVItem(curSentPackets);
				fm.AddCSVItem(curSuccessfulPackets);
				fm.AddCSVItem(curThroughput);				// Mbits/s
				// total
				fm.AddCSVItem(totalSentPackets);
				fm.AddCSVItem(totalSuccessfulPackets);
				fm.AddCSVItem(totalThroughput, true);		// Mbits/s
				fm.Close();
			}
			// schedule the next
			Simulator::Schedule(Seconds(1), &PrintStatistics, curTime, totalSentPackets, totalSuccessfulPackets);
		}
	}
	...
	int main(int argc, char *argv[]) {
		// config
		settings.SetProjectName("rca");
		FileManager::CreatePath(settings.PathProject());
		// config - create folders
		NS_ASSERT(FileManager::CreatePath(settings.PathProjectDebug()) == 200);		// debug
		NS_ASSERT(FileManager::CreatePath(settings.PathProjectTmp()) == 200);		// tmp
		NS_ASSERT(FileManager::CreatePath(settings.PathProjectReport()) == 200); 	// report
		// reset NSSFile location
		config.NSSFile = settings.PathProjectTmp() + config.trafficType + "_" + std::to_string(config.Nsta)
			+ "sta_" + std::to_string(config.NGroup) + "Group_"
			+ std::to_string(config.NRawSlotNum) + "slots_"
			+ std::to_string(config.payloadSize) + "payload_"
			+ std::to_string(config.totaltraffic) + "Mbps_"
			+ std::to_string(config.BeaconInterval) + "BI" + ".nss";
		...
		// set the rate adaption
		__SDN_LAB_SET_WIFIMANAGER(DataRate);
		...
		// schedule the througput
		Simulator::Schedule(Seconds(1), &PrintStatistics, 0, 0, 0);
	}
	```
* `scatch/rca/Configuration`<br>
	`Configuration.h`
	```c++
	// 3rd party headers
	#include <cmath>    // support abs
	...
	struct Configuration {
		/** self-defined parameters **/
		// locations in x and y axis
		// any -1 of these two location parameters means the location is given randomly (in the radius of `rho`) 
		double locX = -1;
		double locY = -1;
		// the speed configuration
		double mobilitySpeedMin = 0;
		double mobilitySpeedMax = 0;
		double mobilityAcceleration = 0;
		...	
	}
	```
	`Configuration.cc`
	```c++
	Configuration::Configuration(int argc, char *argv[]) {
		...
		/** self-defined parameters **/
		cmd.AddValue("locX", "location in X", locX);
		cmd.AddValue("locY", "location in Y", locY);
		cmd.AddValue("mobilitySpeedMin", "the minimal speed", mobilitySpeedMin);
		cmd.AddValue("mobilitySpeedMax", "the maximal speed", mobilitySpeedMax);
		cmd.AddValue("mobilityAcceleration", "the speed acceleration", mobilityAcceleration);
		...
		// parse input
    	cmd.Parse(argc, argv);
		// format input
    	this->mobilityAcceleration = abs(this->mobilityAcceleration);
	}
	```
#### 4.5.2 Rate Adaption
* `/src/wifi/model/wifi-remote-station-manager`<br>
	`wifi-remote-station-manager.h`
	```c++
	class WifiRemoteStationManager : public Object{
		...
		// NN based methods
		#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
			// set a MCS candidate as the initial (calling `DoSetMcsPredict` of its child's method)
    		void SetMcsPredict(unsigned int mcs);
    		// actually set a mcs to a station
    		virtual void DoSetMcsPredict(WifiRemoteStation *station, unsigned int mcs);
		#endif
		...
	}
	```
	`wifi-remote-station-manager.cc`
	```c++
	...
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
		// set a MCS candidate as the initial (calling `DoSetMcsPredict` of its child's method)
		void SetMcsPredict(Mac48Address address, const WifiMacHeader *header, unsigned int mcs){
			WifiRemoteStation *station = Lookup(address, header);
			DoSetMcsPredict(station, mcs);
		}
		// the father method does nothing
  		void WifiRemoteStationManager::DoSetMcsPredict(WifiRemoteStation *station, unsigned int mcs){};
	#endif
	...
	```
* `/src/wifi/model/wifi-remote-station-manager`<br>
	`wifi-remote-station-manager.h`
	```c++
	class MinstrelWifiManager : public WifiRemoteStationManager
	{
		/*** NN based methods ***/
		#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
			// set a MCS candidate as the initial
			virtual void DoSetMcsPredict(WifiRemoteStation *station, unsigned int mcs);
		#endif
	}
	```
	`wifi-remote-station-manager.cc`
	```c++
	// extra headers
	#include "Components/Mcs.h"
	// extra namespace
	using namespace SdnLab;
	...
	// extra macro
	// look around rate
	#ifndef __SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE
		#define __SDN_LAB_RA_MINSTREL_LOOK_AROUND_RATE 10
	#endif
	// find mcs
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
		#define __SDN_LAB_MINSTREL_WIFI_MANAGER_FIND_MCS_IDX(idx, station) \
			size_t i; \
			for(i = 0; i < station->m_state->m_operationalRateSet.size(); ++i){ \
				if(Mcs::FromModeName(station->m_state->m_operationalRateSet[i].GetUniqueName()) == station->mcs){ \
					break; \
				} \
			} \
			idx = i
	#else
		#define __SDN_LAB_MINSTREL_WIFI_MANAGER_FIND_MCS_IDX(idx, station) idx=GetNextSample (station)
	#endif
	...
	struct MinstrelWifiRemoteStation : public WifiRemoteStation
	{
		...
		unsigned int mcs;             // MCS predicted by NN
	}
	/*** NN based methods ***/
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
	// set a MCS candidate as the initial
	void MinstrelWifiManager::DoSetMcsPredict(WifiRemoteStation *station, unsigned int mcs){
		MinstrelWifiRemoteStation * curStation = (MinstrelWifiRemoteStation *) station;
		curStation->mcs = mcs;    
	}
	#endif
	...
	uint32_t MinstrelWifiManager::FindRate (MinstrelWifiRemoteStation *station){
		if ( (((100 * station->m_sampleCount) / (station->m_sampleCount + station->m_packetCount )) < m_lookAroundRate) && (coinFlip == 1) ){
			...
			__SDN_LAB_MINSTREL_WIFI_MANAGER_FIND_MCS_IDX(idx, station);
			...
		}
	}
	```
#### 4.5.3 Mac High
* `src/wifi/model/regular-wifi-mac`<br>
	`regular-wifi-mac.h`
	```c++
	#pragma once
	...
	// extra headers
	#include "Components/PacketContext.h"
	...
	// add packet context as an extra parameter
	virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, SdnLab::PacketContext context = SdnLab::PacketContext());
	```
	`regular-wifi-mac.c`
	```c++
	// 3rd party namespaces
	using namespace SdnLab;
	...
	void RegularWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PacketContext packetContext){
		...
	}
	```
* `src/wifi/model/ap-wifi-mac`<br>
	`ap-wifi-mac.h`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/PacketContext.h"
	#include "Components/StationList.h"
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
		#include "Components/NNData.h"
		#include "Components/OverheadSNN.h"
	#endif
	...
	// add PacketContext as an extra parameter
	virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, SdnLab::PacketContext context);
	...
	// self-defined parameters
	#ifdef __SDN_LAB_DEBUG
    	Toolbox::FileManager fm;                  // FileManger
    	SdnLab::Settings settings;                // Settings
    	SdnLab::StationList stationList = SdnLab::StationListFactory::Create(4424, 5);
	#else
    	SdnLab::StationList stationList = SdnLab::StationListFactory::Create(4259344, 8191);
	#endif
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
    	SdnLab::NNData nnData;
	#endif
	SdnLab::PacketContext context = SdnLab::PacketContext();
	```
	`ap-wifi-mac.c`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/Mac.h"
	// extra namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	/*** self defined macros ***/
	// debug
	#ifdef __SDN_LAB_DEBUG
		#define __SDN_LAB_AP_WIFI_MAC_REPROT_MEMORY_COST(stalist, set) \
			string path = set.PathProjectReport() + set.REPORT_MEMORY_COST; \
			StationListFactory::Summary(path); \
			cout << "Memory size for each station = " << stalist->GetStaMemSize() << " (bytes)" << '\n';
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_STATIONLIST(stalist, set) \
			if(stalist){ \
				Time time = Simulator::Now(); \
				string path = set.PathProjectDebug() + set.TRACK_FILE_AP_WIFI_MAC_STALIST + to_string(time.GetSeconds()) + set.TRACK_FILE_FORM_SUFFIX; \
				stalist->Summary2File(path); \
			}
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_DATA_TO_STATIONLIST(stalist, set) \
			if(stalist){ \
				Time time = Simulator::Now(); \
				string path = set.PathProjectDebug() + set.TRACK_FILE_AP_WIFI_MAC_NN_INPUT + to_string(time.GetSeconds()) + set.TRACK_FILE_FORM_SUFFIX; \
				stalist->Summary2File(path, __SDN_LAB_NNDATA_LEN); \
			}
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_DATA_FROM_STATIONLIST(stalist, set) \
			if(stalist){ \
				Time time = Simulator::Now(); \
				string path = set.PathProjectDebug() + set.TRACK_FILE_AP_WIFI_MAC_NN_OUTPUT + to_string(time.GetSeconds()) + set.TRACK_FILE_FORM_SUFFIX; \
				stalist->Summary2File(path, __SDN_LAB_NNDATA_LEN, true); \
			}
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_RECE(fm, set) \
			if(fm.Open((set.PathProjectDebug() + set.TRACK_FILE_AP_WIFI_MAC_RECE))==200){ \
				fm.AddCSVItem(context.IsEmpty()); \
				fm.AddCSVItem(macPacketSize); \
				fm.AddCSVItem(phyPacketSize); \
				fm.AddCSVItem(context.GetStartTime()); \
				fm.AddCSVItem(context.GetEndTime()); \
				fm.AddCSVItem(context.GetSnr()); \
				fm.AddCSVItem(context.GetRxPower()); \
				fm.AddCSVItem(context.GetMCSIn()); \
				fm.AddCSVItem(context.IsReceived(), true); \
				fm.Close(); \
			}
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_RECE_MAC_ADDR(set, macPacketSize, context) \
			std::fstream file; \
			file.open(set.PathProjectDebug() + set.TRACK_FILE_AP_WIFI_MAC_RECE_ADDR, std::fstream::in | std::fstream::app); \
			file << macPacketSize << ','; \
			file << context.GetSourMacAddr() << ','; \
			file << context.GetDestMacAddr() << ','; \
			file << context.GetTxMacAddr() << ','; \
			file << context.GetRxMacAddr() << ','; \
			file << context.GetStartTime() << ','; \
			file << context.GetEndTime() << ','; \
			file << context.GetMCSIn() << ','; \
			file << '\n'; \
			file.close();
	#else
		#define __SDN_LAB_AP_WIFI_MAC_REPROT_MEMORY_COST(stalist, set)
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_STATIONLIST(stalist, set)
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_DATA_TO_STATIONLIST(stalist, set)
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_DATA_FROM_STATIONLIST(stalist, set)
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_RECE(fm, set)
		#define __SDN_LAB_AP_WIFI_MAC_PRINT_RECE_MAC_ADDR(set, macPacketSize, context)
	#endif
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS)
		#define __SDN_LAB_PREDICT_AT_AP(stalist, context) \
			stalist->PredictMCS(); \
			context.Clear(); \
			context.SetOverhead(OverheadSNN::Create(stalist));
	#else
		#define __SDN_LAB_PREDICT_AT_AP(stalist, context)
	#endif

	// clear the station list
	ApWifiMac::ApWifiMac (){
		...
		// debug
		__SDN_LAB_AP_WIFI_MAC_REPROT_MEMORY_COST(this->stationList, this->settings);
	}
	ApWifiMac::~ApWifiMac{
		...
		// clear the station list
		StationList::Destory(this->stationList);
	}
	...
	// ApWifiMac::Receive: 		add PacketContext as an extra parameter
	void ApWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PacketContext context){
		...
		uint32_t macPacketSize = packet->GetSize();
		uint32_t phyPacketSize = 0;
		Mac48Address sourMacAddr = __SDN_LAB_MAC_BROADCAST_ADDR;    // set the source Mac default address is broadcast
		if (!context.isEmpty()){
			context.SetMacPacketSize(macPacketSize);
			context.SetAllMacAddr(hdr);
			phyPacketSize = context.GetPhyPacketSize();
			sourMacAddr = context.GetSourMacAddr();
		}
		// add context to StationList
		this->stationList->AddStationOrContext(context);
		// debug - print the packet information
  		__SDN_LAB_AP_WIFI_MAC_PRINT_RECE(this->filemanager, this->settings);
  		__SDN_LAB_AP_WIFI_MAC_PRINT_RECE_MAC_ADDR(this->settings, macPacketSize, context);
		...
	}
	// transpond the station list to the lower layer
	void ApWifiMac::SendOneBeacon (void){
		// predict the MCS for each station
  		__SDN_LAB_PREDICT_AT_AP(this->stationList, this->context);
  		// debug - print
  		__SDN_LAB_AP_WIFI_MAC_PRINT_STATIONLIST(this->stationList, this->settings);
  		__SDN_LAB_AP_WIFI_MAC_PRINT_DATA_TO_STATIONLIST(this->stationList, this->settings);
  		__SDN_LAB_AP_WIFI_MAC_PRINT_DATA_FROM_STATIONLIST(this->stationList, this->settings);
		...
		if (m_s1gSupported){
			...
			m_beaconDca->Queue (packet, hdr, context);
			...
		}
		else{
			...
			m_beaconDca->Queue (packet, hdr, context);
		}
		...
	}
	```
* `src/wifi/model/sta-wifi-mac`<br>
	`sta-wifi-mac.h`
	```c++
	// extra headers
	#include "Components/PacketContext.h"			// add PacketContext header for its C/C++ file
	#if defined(__SDN_LAB_DEBUG) || defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS) || defined(__SDN_LAB_RA_MINSTREL_AI_DIST)
		#include "Components/NNData.h"
		#include "Components/OverheadSNN.h"
	#endif
	...
	// add PacketContext as an extra parameter
	virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, SdnLab::PacketContext context);
	```
	`sta-wifi-mac.cc`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/Mac.h"
	// extra namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	// extra macros
	#if defined(__SDN_LAB_RA_MINSTREL_SNN_VINCENT) || defined(__SDN_LAB_RA_MINSTREL_SNN) || defined(__SDN_LAB_RA_MINSTREL_SNN_PLUS)
		#define __SDN_LAB_STA_MAC_ACCEPT_MCS_PREDICT(context) \
			OverheadSNN * overhead = (OverheadSNN *)(context.GetOverhead()); \
			if(overhead){ \
			if(overhead->Begin()){ \
				for(auto overheadData = overhead->Begin(); overheadData!= overhead->End(); ++overheadData){ \
				if(overheadData && overheadData->Match(GetAddress())){ \
					break; \
				} \
				} \
				if(overhead->End()->Match(GetAddress())){ \
				}\
			} \
			}
	#else
		#define __SDN_LAB_STA_MAC_ACCEPT_MCS_PREDICT(context)
	#endif
	...
	// StaWifiMac::Receive: 		add PacketContext as an extra parameter
	void StaWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PacketContext context){
		// accept the MCS prediction from a beacon
  		__SDN_LAB_STA_MAC_ACCEPT_MCS_PREDICT(context);
	}
	```
#### 4.5.4 Mac Middle
* `src/wifi/model/dca-txop`<br>
	`dca-txop.h`
	```c++
	#pragma once
	...
	// self-defined headers
	#include "Components/StationList.h"
	...
	// add PacketContext as an extra parameter
	void Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr, SdnLab::PacketContext context = SdnLab::PacketContext());
	```
	`dca-txop.cc`
	```c++
	// 3rd party namespaces
	using namespace SdnLab;
	...
	// add PacketContext as an extra parameter
	void DcaTxop::Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr, PacketContext context){
		...
		m_queue->Enqueue (packet, hdr, context);
		...
	}
	...
	// send the PacketContext to MacLow
	void DcaTxop::NotifyAccessGranted (void){
  		// init variables
  		PacketContext context;
		...
		if (m_currentPacket == 0){
			...
			m_currentPacket = m_queue->Dequeue (&m_currentHdr, context);
			...
		}
		...
		if (m_currentHdr.GetAddr1 ().IsGroup () || m_currentHdr.IsPsPoll ()){
			...
			Low ()->StartTransmission (m_currentPacket, &m_currentHdr, params, m_transmissionListener, context);
			...
		}else{
			if (NeedFragmentation ()){
				...
				Low ()->StartTransmission (m_currentPacket, &m_currentHdr, params, m_transmissionListener, context);
			}else{
				...
				Low ()->StartTransmission (m_currentPacket, &m_currentHdr, params, m_transmissionListener, context);
			}
		}
  	}
	// send the empty context
	DcaTxop::StartNext (void){
		...
		Low ()->StartTransmission (fragment, &hdr, params, m_transmissionListener);
	}
	```
* `src/wifi/model/mac-rx-middle.h`<br>
	`mac-rx-middle.h`
	```c++
	// extra headers
	#include "Components/PacketContext.h"
	...
	// add PacketContext as extra parameters
	typedef Callback<void, Ptr<Packet>, const WifiMacHeader*, SdnLab::PacketContext> ForwardUpCallback;
	...
	// add PacketContext as extra parameters
	void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, SdnLab::PacketContext);
	```
	`mac-rx-middle.c`
	```c++
	// 3rd party headers
	#include "Modules/Toolbox/FileManager.h"
	// self-defined headers
	#include "Components/Settings.h"
	// 3rd party namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	...
	// MacRxMiddle::Receive: 		add PacketContext as an extra parameter
	// m_callback: 					add PacketContext as an extra parameter
	void MacRxMiddle::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PacketContext context){
		...
		m_callback (agregate, hdr, context);
	}
	```
#### 4.5.5 Mac Middle <-> Mac Low
* `src/wifi/model/wifi-mac-queue`<br>
	`wifi-mac-queue.h`xt
	```c++
	#pragma once
	...
	// self-defined headers
	#include "Components/PacketContext.h"
	...
	// add PacketContext as an extra parameter
	struct Item{
		Item (Ptr<const Packet> packet, const WifiMacHeader &hdr, Time tstamp);
		Item (Ptr<const Packet> packet, const WifiMacHeader &hdr, Time tstamp, SdnLab::PacketContext context);
    	Ptr<const Packet> packet;
    	WifiMacHeader hdr;
    	Time tstamp;
    	SdnLab::PacketContext context;
	};
	...
	// add PacketContext as an extra parameter
	void Enqueue (Ptr<const Packet> packet, const WifiMacHeader &hdr);
	void Enqueue (Ptr<const Packet> packet, const WifiMacHeader &hdr, SdnLab::PacketContext context);
	// add PacketContext as an extra parameter
	Ptr<const Packet> Dequeue (WifiMacHeader *hdr);
	Ptr<const Packet> Dequeue (WifiMacHeader *hdr, SdnLab::PacketContext &context);
	```
	`wifi-mac-queue.cc`
	```c++
	// 3rd party namespaces
	using namespace SdnLab;
	...
	WifiMacQueue::Item::Item (Ptr<const Packet> packet, const WifiMacHeader &hdr, Time tstamp){
		this->packet = packet;
		this->hdr = hdr;
		this->tstamp = tstamp;
	}
	WifiMacQueue::Item::Item (Ptr<const Packet> packet, const WifiMacHeader &hdr, Time tstamp, PacketContext context){
		Item(packet, hdr, tstamp);
		this->context = context;
	}
	...
	// add PacketContext as an extra parameter
	void Enqueue (Ptr<const Packet> packet, const WifiMacHeader &hdr, PacketContext context){
		...
		m_queue.push_back (Item (packet, hdr, now, context));
		...
	}
	void WifiMacQueue::Enqueue (Ptr<const Packet> packet, const WifiMacHeader &hdr){
		Enqueue(packet, hdr, PacketContext());
	}
	...
	// add PacketContext as an extra parameter
	Ptr<const Packet> WifiMacQueue::Dequeue (WifiMacHeader *hdr){
		...
	}
	Ptr<const Packet> Dequeue (WifiMacHeader *hdr, SdnLab::PacketContext &context){
		...
		if (!m_queue.empty (){
			...
      		context = i.context;
			...
    	}
		...
	}
	```
#### 4.5.6 Mac Low
* `src/wifi/model/mac-low`<br>
	`mac-low.h`
	```c++
	// extra headers
	#include "Components/PacketContext.h"
	...
	// add PacketContext as extra parameters
	typedef Callback<void, Ptr<Packet>, const WifiMacHeader*, SdnLab::PacketContext> MacLowRxCallback;
	...
	void SendDataPacket (SdnLab::PacketContext context);
	...
	void ForwardDown (Ptr<const Packet> packet, const WifiMacHeader *hdr, WifiTxVector txVector, WifiPreamble preamble, SdnLab::PacketContext context=SdnLab::PacketContext());
	void SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, SdnLab::PacketContext context);
	...
	virtual void StartTransmission (Ptr<const Packet> packet, const WifiMacHeader* hdr, MacLowTransmissionParameters parameters, MacLowTransmissionListener *listener, SdnLab::PacketContext context=SdnLab::PacketContext());
	...
	void SetRxCallback (Callback<void,Ptr<Packet>,const WifiMacHeader *, SdnLab::PacketContext> callback);
	...
	void DeaggregateAmpduAndReceive (Ptr<Packet> aggregatedPacket, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, SdnLab::PacketContext context);
	...
	void ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, bool ampduSubframe, SdnLab::PacketContext context);
	```
	`mac-low.cc`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/PacketContext.h"
	// extra namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	...
	void MacLow::StartTransmission (Ptr<const Packet> packet, const WifiMacHeader* hdr, MacLowTransmissionParameters parameters, MacLowTransmissionListener *listener, PacketContext context){
		...
		if (m_txParams.MustSendRts ()){
			...
		}
		else{
			if (NeedCtsToSelf () && m_ctsToSelfSupported)
			{
				...
			}
			else{
				SendDataPacket (context);
			}
		}
	}
	...
	void MacLow::SendDataPacket (PacketContext context){
		...
		ForwardDown (m_currentPacket, &m_currentHdr, dataTxVector, preamble, context);
		...
	}
	...
	void MacLow::ForwardDown (Ptr<const Packet> packet, const WifiMacHeader* hdr, WifiTxVector txVector, WifiPreamble preamble, PacketContext context){
		...
		if (!m_ampdu || hdr->IsRts () || hdr->IsRts ()){
			m_phy->SendPacket (packet, txVector, preamble, 0, context);
		}else{
			...
			for (; queueSize > 0; queueSize--){
				...
				if (delay == Seconds (0)){
					...
					m_phy->SendPacket (newPacket, txVector, preamble, packetType, context);
				}else{
					Simulator::Schedule (delay, &MacLow::SendPacket, this, newPacket, txVector, preamble, packetType, context);
				}
				...
			}
		}
	}
	void MacLow::SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, SdnLab::PacketContext context){
		NS_LOG_DEBUG ("Sending MPDU as part of A-MPDU");
		m_phy->SendPacket (packet, txVector, preamble, packetType, context);
	}
	...
	// MacLow::DeaggregateAmpduAndReceive: 	add PacketContext as an extra parameter
	// ReceiveOk: 							add PacketContext as an extra parameter
	void MacLow::DeaggregateAmpduAndReceive (Ptr<Packet> aggregatedPacket, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, PacketContext context){
		...
		if (aggregatedPacket->RemovePacketTag (ampdu)){
			...
			if (firsthdr.GetAddr1 () == m_self){
				...
				if (firsthdr.IsAck () || firsthdr.IsBlockAck () || firsthdr.IsBlockAckReq ()){
					ReceiveOk ((*n).first, rxSnr, txVector, preamble, ampduSubframe, context);
				}else if (firsthdr.IsData () || firsthdr.IsQosData ()){
					...
					ReceiveOk ((*n).first, rxSnr, txVector, preamble, ampduSubframe, context);
					...
				}
				...
			}
			...
		}
		else{
			ReceiveOk (aggregatedPacket, rxSnr, txVector, preamble, ampduSubframe, context);
		}
	}
	...
	// m_rxCallback: add a NULL Packet Context (not being called in the current version)
	void MacLow::RxCompleteBufferedPacketsWithSmallerSequence (uint16_t seq, Mac48Address originator, uint8_t tid)
	{
		...
		if (it != m_bAckAgreements.end ())
		{
			...
			for (; i != (*it).second.second.end ()&& QosUtilsMapSeqControlToUniqueInteger ((*i).second.GetSequenceNumber (), endSequence) < mappedStart; )
			{
				if (guard == (*i).second.GetSequenceControl ())
				{
					if (!(*i).second.IsMoreFragments ())
					{
						while (last != i)
						{
							m_rxCallback ((*last).first, &(*last).second, PacketContext());
							last++;
						}
						m_rxCallback ((*last).first, &(*last).second, PacketContext());
						...
					}
					...
				}
				...
			}
			...
		}
	}
	// m_rxCallback: add a NULL Packet Context (not being called in the current version)
	void MacLow::RxCompleteBufferedPacketsUntilFirstLost (Mac48Address originator, uint8_t tid)
	{
		...
		if (it != m_bAckAgreements.end ())
		{
			...
			for (; i != (*it).second.second.end () && guard == (*i).second.GetSequenceControl (); i++)
			{
				if (!(*i).second.IsMoreFragments ())
				{
					while (lastComplete != i)
					{
					m_rxCallback ((*lastComplete).first, &(*lastComplete).second, PacketContext());
					lastComplete++;
					}
					m_rxCallback ((*lastComplete).first, &(*lastComplete).second, PacketContext());
					lastComplete++;
				}
				guard = (*i).second.IsMoreFragments () ? (guard + 1) : ((guard + 16) & 0xfff0);
			}
			...
		}
	}
	```
#### 4.5.7 Phy
* `src/wifi/model/wifi-phy.h`<br>
	```c++
	#include "Components/PacketContext.h"
	// RxOkCallback:: add PacketContext as an extra parameter
	typedef Callback<void, Ptr<Packet>, double, WifiTxVector, enum WifiPreamble, SdnLab::PacketContext> RxOkCallback;
	...
	virtual void SendPacket (Ptr<const Packet> packet, WifiTxVector txvector, enum WifiPreamble preamble, uint8_t packetType, SdnLab::PacketContext context) = 0;
	```
* `src/wifi/model/wifi-phy-state-helper`<br>
	`wifi-phy-state-helper.h`
	```c++
	#include "Components/PacketContext.h"	// add PacketContext header for its C/C++ file
	...
	// add PacketContext as an extra parameter
	void SwitchFromRxEndOk (Ptr<Packet> packet, double snr, WifiTxVector txVector, enum WifiPreamble preamble, SdnLab::PacketContext context);
	```
	`wifi-phy-state-helper.cc`
	```c++
	// 3rd party namespaces
	using namespace SdnLab;
	...
	// SwitchFromRxEndOk: 						add PacketContext as an extra parameter
	// WifiPhy::RxOkCallback m_rxOkCallback: 	add PacketContext as an extra input
	void WifiPhyStateHelper::SwitchFromRxEndOk (Ptr<Packet> packet, double snr, WifiTxVector txVector, enum WifiPreamble preamble, PacketContext context){
		...
		if (!m_rxOkCallback.IsNull ()){
			m_rxOkCallback (packet, snr, txVector, preamble, context);
		}
	}
	```
* `src/wifi/model/yans-wifi-phy`<br>
	`yans-wifi-phy.h`
	```c++
	virtual void SendPacket (Ptr<const Packet> packet, WifiTxVector txvector, enum WifiPreamble preamble, uint8_t packetType, SdnLab::PacketContext context);
	...
	void EndReceive (Ptr<Packet> packet, enum WifiPreamble preamble, uint8_t packetType, Ptr<InterferenceHelper::Event> event, SdnLab::PacketContext context);
	...
	void StartReceivePreambleAndHeader (Ptr<Packet> packet, double rxPowerDbm, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, Time rxDuration, SdnLab::PacketContext context = SdnLab::PacketContext());
	...
	// debug 
	#ifdef __SDN_LAB_DEBUG
		SdnLab::Settings settings;
	#endif
	```
	`yans-wifi-phy.cc`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/PacketContext.h"
	// extra namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	// extra macros
	#ifdef __SDN_LAB_DEBUG
	#define __SDN_LAB_YANS_WIFI_PHY_PRINT(path, filemanager) \
		if(filemanager.Open(path) == 200){ \
		filemanager.AddCSVItem(packetSize); \
		filemanager.AddCSVItem(startTime); \
		filemanager.AddCSVItem(endTime); \
		filemanager.AddCSVItem(snr); \
		filemanager.AddCSVItem(per); \
		filemanager.AddCSVItem(rxPower); \
		filemanager.AddCSVItem(interferePower); \
		filemanager.AddCSVItem(modeName); \
		filemanager.AddCSVItem(mcs_in); \
		filemanager.AddCSVItem(isReceived, true); \
		filemanager.Close(); \
		}\
	#define __SDN_LAB_YANS_WIFI_PHY_RECE_ALL_FILEPATH(set) (set.PathProjectDebug() + set.TRACK_FILE_YANS_WIFI_PHY)
	#else
	#define __SDN_LAB_YANS_WIFI_PHY_PRINT(path, filemanager)
	#define __SDN_LAB_YANS_WIFI_PHY_RECE_ALL_FILEPATH(set) ""
	#endif
	#ifdef __SDN_LAB_PHY_PACKET_SIZE_DATA
	#define __SDN_LAB_YANS_WIFI_PHY_RECE_DATA_FILEPATH(set) (set.PathProjectDebug() + set.TRACK_FILE_YANS_WIFI_PHY_DATA)
	#else
	#define __SDN_LAB_YANS_WIFI_PHY_RECE_DATA_FILEPATH(set) ""
	#define __SDN_LAB_PHY_PACKET_SIZE_DATA 0
	#endif
	#ifdef __SDN_LAB_PHY_PACKET_SIZE_BEACON
	#define __SDN_LAB_YANS_WIFI_PHY_RECE_DATA_BEACON_FILEPATH(set) (set.PathProjectDebug() + set.TRACK_FILE_YANS_WIFI_PHY_DATA_BEACON)
	#else
	#define __SDN_LAB_YANS_WIFI_PHY_RECE_DATA_BEACON_FILEPATH(set) ""
	#define __SDN_LAB_PHY_PACKET_SIZE_BEACON 0
	#endif
	...
	StartReceivePreambleAndHeader (Ptr<Packet> packet, double rxPowerDbm, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, Time rxDuration, PacketContext context){
		...
		switch (m_state->GetState ()){
			...
			case YansWifiPhy::IDLE:
				if (rxPowerW > m_edThresholdW){
					...
					m_endRxEvent = Simulator::Schedule (rxDuration, &YansWifiPhy::EndReceive, this, packet, preamble, packetType, event, context);
				}
		}
	}
	...
	// YansWifiPhy::EndReceive: 	create the PacketContext
	// SwitchFromRxEndOk: 			transfer the packet to the upper layer
	// <DEBUG>: 					DEBUG_SDN, DEBUG_SDN_PHY_PACKET_SIZE_DATA, DEBUG_SDN_PHY_PACKET_SIZE_BEACON control printing
	void YansWifiPhy::EndReceive (Ptr<Packet> packet, enum WifiPreamble preamble, uint8_t packetType, Ptr<InterferenceHelper::Event> event, PacketContext context)
	{
		...
		// packet context
		// calculate the packet size
		uint32_t packetSize = packet->GetSize();
		// calculate the start time and end time of this packet
		double startTime = event->GetStartTime().GetSeconds();
		double endTime = event->GetEndTime().GetSeconds();
		// calculate SNR & PER
		struct InterferenceHelper::SnrPer snrPer;
		snrPer = m_interference.CalculatePlcpPayloadSnrPer (event);
		double snr = snrPer.snr;
		double per = snrPer.per;
		// calculate the Rx power in Watt
		double rxPower = event->GetRxPowerW();
		// calculate the interference power (currently we suppose we don't know the interference power)
		double interferePower = -1;
		// get the mode name
		std::string modeName = event->GetPayloadMode().GetUniqueName();
		// MCS
		unsigned int mcs_in = ContextFactory::ModeName2MCS(modeName);
		// whether is received
		bool isReceived = false;
		// create the context
		context.SetPhyPacketSize(packetSize);
  		context.SetStartTime(startTime);
  		context.SetEndTime(endTime);
  		context.SetPer(per);
  		context.SetSnr(snr);
  		context.SetRxPower(rxPower);
  		context.SetMcsInAndBandwidth(modeName);
		...
		// decide whether this packet is received or not
		if (m_plcpSuccess == true){
			...
			if (m_random->GetValue () > snrPer.per){
				...
				// set packetContext to be received
				isReceived = true;
				packetContext.SetIsReceived(true);
				// notify the upper layer that this packet is received
				m_state->SwitchFromRxEndOk (packet, snrPer.snr, event->GetTxVector (), event->GetPreambleType (), packetContext);
			}
			else{
				...
			}
			// record data
			__SDN_LAB_YANS_WIFI_PHY_PRINT(__SDN_LAB_YANS_WIFI_PHY_RECE_ALL_FILEPATH(this->settings), fm);
			if (packetSize == __SDN_LAB_PHY_PACKET_SIZE_DATA){
				__SDN_LAB_YANS_WIFI_PHY_PRINT(__SDN_LAB_YANS_WIFI_PHY_RECE_DATA_FILEPATH(this->settings), fm);
			}
			if (packetSize == __SDN_LAB_PHY_PACKET_SIZE_BEACON){
				__SDN_LAB_YANS_WIFI_PHY_PRINT(__SDN_LAB_YANS_WIFI_PHY_RECE_DATA_BEACON_FILEPATH(this->settings), fm);
			}
		}
		else{
			...
		}
		...
	}
	...
	void YansWifiPhy::SendPacket (Ptr<const Packet> packet, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, PacketContext context){
		...
		m_channel->Send (this, packet, GetPowerDbm (txVector.GetTxPowerLevel ()) + m_txGainDb, txVector, preamble, packetType, txDuration, context);
	}
	```

#### 4.5.7 Channel
* `src/wifi/model/yans-wifi-channl`<br>
	`src/wifi/model/.h`
	```c++
	// extra headers
	#include "Components/PacketContext.h"
	...
	// add PacketContext as the extra parameter
	void Send (Ptr<YansWifiPhy> sender, Ptr<const Packet> packet, double txPowerDbm, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, Time duration, SdnLab::PacketContext context) const;
	...
	// add PacketContext as the extra parameter
	void Receive (uint32_t i, Ptr<Packet> packet, double *atts, WifiTxVector txVector, WifiPreamble preamble) const;
  	void Receive (SdnLab::PacketContext context, Ptr<Packet> packet, double *atts, WifiTxVector txVector, WifiPreamble preamble) const;
	```
	`yans-wifi-channl.cc`
	```c++
	// extra headers
	#include "Modules/Toolbox/FileManager.h"
	#include "Components/Settings.h"
	#include "Components/PacketContext.h"
	// extra namespaces
	using namespace Toolbox;
	using namespace SdnLab;
	...
	void Send (Ptr<YansWifiPhy> sender, Ptr<const Packet> packet, double txPowerDbm, WifiTxVector txVector, WifiPreamble preamble, uint8_t packetType, Time duration, PacketContext context) const{
		...
		for (PhyList::const_iterator i = m_phyList.begin (); i != m_phyList.end (); i++, j++){
			if (sender != (*i)){
				...
				if(context.isEmpty()){
					void (YansWifiChannel::*callback)(uint32_t, Ptr<Packet>, double *, WifiTxVector, WifiPreamble) const = NULL;
					callback = &YansWifiChannel::Receive;
					Simulator::ScheduleWithContext (dstNode, delay, callback, this, j, copy, atts, txVector, preamble);
				}else{
					context.SetNodeIndex(j);
					void (YansWifiChannel::*callback)(PacketContext, Ptr<Packet>, double *, WifiTxVector, WifiPreamble) const = NULL;
					callback = &YansWifiChannel::Receive;
					Simulator::ScheduleWithContext (dstNode, delay, callback, this, context, copy, atts, txVector, preamble);
				}
			}
		}
	}
	void YansWifiChannel::Receive (uint32_t i, Ptr<Packet> packet, double *atts, WifiTxVector txVector, WifiPreamble preamble) const {...}
	void YansWifiChannel::Receive (SdnLab::PacketContext context, Ptr<Packet> packet, double *atts, WifiTxVector txVector, WifiPreamble preamble) const{
		m_phyList[context.GetNodeIndex()]->StartReceivePreambleAndHeader (packet, *atts, txVector, preamble, *(atts + 1), NanoSeconds (*(atts + 2)), context);
		delete[] atts;
	}
	```


## 5 Potential Problems
* `MacLow::RxCompleteBufferedPacketsWithSmallerSequence` unknow when to be called and why it is not called
* `MacLow::RxCompleteBufferedPacketsUntilFirstLost` unkown when to be called and why it is not called
* `ApWifiMac::Receive` why does A-MSDU must be QoS?
* `MacLow -> MacRxMiddle -> RegularWifiMac::Receive` in `AdhocWifiMac` & `OcbWifiMac`<br>
	`src/wifi/model/adhoc-wifi-mac.cc`<br>
	This change should not be remove when `RegularWifiMac::Receive` supports default `NULL` PacketContext
	```c++
	// RegularWifiMac::Receive:: add a NULL PacketContext
	void AdhocWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr){
		...
		RegularWifiMac::Receive (packet, hdr, NULL);
	}
	```
	* `src/wave/model/ocb-wifi-mac.cc`<br>
	This change should not be remove when `RegularWifiMac::Receive` supports default `NULL` PacketContext<br>
	```c++
	// RegularWifiMac::Receive: 	add a NULL PacketContext
	void OcbWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr){
		...
		RegularWifiMac::Receive (packet, hdr, NULL);
	}
	```