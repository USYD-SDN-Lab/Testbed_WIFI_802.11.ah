# WIFI 802.11ah (ns-3)
- [Installation and usage instructions](#installation-and-usage-instructions)
  - [Clone the project from git](#clone-the-project-from-git)
  - [Follow the instructions on https://www.nsnam.org/wiki/Installation to prepare all dependencies. <br>](#follow-the-instructions-on-httpswwwnsnamorgwikiinstallation-to-prepare-all-dependencies-br)
  - [Clean](#clean)
  - [Configure waf](#configure-waf)
  - [Build](#build)
  - [Run the simulation (if you use ahVisualizer start it first):](#run-the-simulation-if-you-use-ahvisualizer-start-it-first)
- [RAW related parameters:](#raw-related-parameters)
- [Wi-Fi mode parameters](#wi-fi-mode-parameters)
- [Other parameters](#other-parameters)
- [TIM and page slice parameters](#tim-and-page-slice-parameters)
- [Further reading](#further-reading)
- [Protocol Stack](#protocol-stack)
  - [MacLow -> MacRxMiddle](#maclow---macrxmiddle)
  - [MacLow -> MacRxMiddle -> RegularWifiMac & RegularWifiMac-> DcaTxop/DcaManager -> MacLow](#maclow---macrxmiddle---regularwifimac--regularwifimac--dcatxopdcamanager---maclow)
- [**Additive/modified files & folders from the original fork, maintainer must keep those files & folders**](#additivemodified-files--folders-from-the-original-fork-maintainer-must-keep-those-files--folders)
  - [General Modified Files](#general-modified-files)
  - [Removed files](#removed-files)
  - [3rd Party Modules](#3rd-party-modules)
  - [Components - all self-defined components](#components---all-self-defined-components)
  - [Updated Source File (adding new functions)](#updated-source-file-adding-new-functions)
    - [`src/wave/model`](#srcwavemodel)
    - [`src/wifi/model`](#srcwifimodel)
- [Potential Problems](#potential-problems)

This repository is vessal of the IEEE802.11ah (Wi-Fi HaLow) protocol for the NS-3 network simulator, which comes from [LeTian and his associates](https://github.com/imec-idlab/IEEE-802.11ah-ns-3). Its NS-3 version is `3.23` with multiple modules updated to `3.25` to incorporate congestion control fixes for TCP traffic.
* This module includes support for:
	* Restricted Access Window (RAW) with interface for dynamic configuration
	* Traffic Indication Map (TIM) segmentation 
	* Energy consumption model
	* Adaptive Modulation and Coding Scheme (MCS)
## Installation and usage instructions
### Clone the project from git
### Follow the instructions on https://www.nsnam.org/wiki/Installation to prepare all dependencies. <br>
For `Ubuntu 18.04`, those are:<br>
(please note that `sudo apt-get -y install gsl-bin libgsl2 libgsl2:i386` is changed into `sudo apt-get -y install gsl-bin` because the other two packets are missing in `Ubuntu 18.04`)
```sh
sudo apt-get update
sudo apt-get -y install gcc g++ python python-dev
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
### Clean
```sh
./waf clean
```
### Configure waf
* Default
```
CXXFLAGS="-std=c++11" ./waf configure --disable-examples --disable-tests
```
* Using macro definition:
The macro definition is added in `CXXFLAGS`. For example, `CXXFLAGS="-Dxxx=yy"`, `xxx` is the macro definition and `yy` is the replacement of `xxx`. Please note that the replacement is not necessary especially in the conditional compilation.<br>
	* `Debug`
	`DEBUG_SDN` to activate debug mode, `DDEBUG_PATH_PREFIX` tells the path to store the 
	```sh
	CXXFLAGS="-std=c++11 -DDEBUG_SDN" ./waf configure --disable-examples --disable-tests
	```
	In the debug mode, we have other macro definitions: the physical layer data packet size to track `-DDEBUG_SDN_PHY_PACKET_SIZE_DATA=166`; the physical layer beacon packet size to track`-DDEBUG_SDN_PHY_PACKET_SIZE_BEACON=71`  
	```sh
	CXXFLAGS="-std=c++11 -DDEBUG_SDN -DDEBUG_SDN_PHY_PACKET_SIZE_DATA=166" ./waf configure --disable-examples --disable-tests
	```
### Build
```sh
./waf
```
### Run the simulation (if you use [ahVisualizer](https://github.com/imec-idlab/ahVisualizer) start it first):
* Test
```sh
./waf --run test
```
```sh
./waf --run "test --seed=1 --simulationTime=60 --payloadSize=256"
```
* Rate control Algorithm (RCA)<br>
The 802.11ah nodes can always use the same MCS as specified by "Wi-Fi mode parameters" when "ConstantRateWifiManager" is used. The nodes can also adapt the MCSs dynamically when rata control algorithm is used. Details about Rate control Algorithms can be found on https://www.nsnam.org/docs/models/html/wifi-design.html#rate-control-algorithms.
	* An example of using MinstrelWifiManager is given in scratch/rca, where one mobile station continuously transmits 100-byte UDP packets to the AP, while moving from -500 m to 500 m relative to the centrally placed AP at the speed of 1 m/s.
	```sh
	./waf --run "rca --seed=1 --simulationTime=100 --payloadSize=100 --pageSliceLength=1 --pageSliceCount=0"
	```
	* Contentions 2 (static)<br>
	generate RAW
	```sh
	./waf --run "RAW-generate --NRawSta=128 --NGroup=32 --NumSlot=1 --RAWConfigPath='./OptimalRawGroup/RawConfig-rca-contention-2.txt' --beaconinterval=1000000 --pageSliceCount=2 --pageSliceLen=1"
	```
	run the simulation
	```sh
	./waf --run "rca --seed=1 --simulationTime=100 --payloadSize=100 --BeaconInterval=1000000 --rho=250 --pagePeriod=2 --pageSliceLength=1 --pageSliceCount=2 --RAWConfigFile='./OptimalRawGroup/RawConfig-rca-contention-2.txt'"
	```
	* Contentions 2 (static) - Vincent (Obsolete)<br>
	run the simulation
	```sh
	./waf --run "rca --seed=1 --simulationTime=10 --payloadSize=100 --BeaconInterval=500000 --rho=250 --pagePeriod=4 --pageSliceLength=4 --pageSliceCount=4 --RAWConfigFile='./OptimalRawGroup/RawConfig-rca-contention-2-vincent.txt' --TrafficPath='./OptimalRawGroup/traffic/data-contention-2.txt'"
	```

## RAW related parameters:
* NRawSta:             Number of stations supporting RAW. NRawSta equals the largest AID specified in RAWConfigFile.
* RAWConfigFile:       RAW configuration is stored in this file.

The default RAWConfigFile "./OptimalRawGroup/RawConfig-32-2-2.txt" sets 2 RPSs (RAW Parameter Sets) to be broadcasted with beacons in cycles (e.g. Beacon1: RPS1, Beacon2: RPS2, Beacon3: RPS1, Beacon4: RPS2, ...). The first RPS contains 2 RAW groups for 63 stations, each RAW group contains 2 RAW slots. The second RPS contains 1 RAW group for 4 stations (64-67), where the RAW group contains 3 slots. This file consists of 6 lines:

```
2
2
0	  1	  1	  200	  2	  0	  1	29
0	  1	  1	  200	  2	  0	  30	63
1
0	  1	  1	  220	  3	  0	  64	67
```
line 1: NRPS = number of RPS elements

line x: NRAW_k = number of RAW Groups in the RPS k; x = (NRAW_0 + NRAW_1 + ... + NRAW_(k-1)) + 1 + k; NRAW_0 = 0 

        for RPS number k=1, line x=0+1+1=2 contains information on number of RAW groups in the RPS
        
        for RPS number k=2 and NRAW_1=2 (from line 2), line x=0+2+1+2=5 contains information on number of RAW groups in the RPS
        
line 3, line 4 and line 6: configuration of each individual RAW group, including

          * RawControl           Whether RAW can be accessed by any stations within the RAW group or only the paged stations within the RAW group.  
          * CrossSlotBoundary    Whether STAs are allowed to transmit after the assigned RAW slot boundary.
          * SlotFormat           Format of RAW slot count.                 
          * NRawSlotCount        Used for calculation of RAW slot duration.   
          * NRawSlotNum          Number of slots per RAW group.                     
          * Page                 Page index of the subset of AIDs.
          * Aid_start            Station with the lowest AID allocated in the RAW.
          * Aid_end              Station with the highest AID allocated in the RAW.


Notes:   
          1. RawControl, currently set to 0, RAW can be accessed by any stations within the RAW group.            
          2. CrossSlotBoundary, currenty set to 1, only cross slot boundary are supported.                    
          3. RAW slot duration = 500 us + NRawSlotCount * 120 us, NRawSlotCount is y = 11(8) bits   length when SlotFormat is set to 1(0), and NRawSlotNum is (14 - y) bits length.                                     
          4. The above  RAWConfigFile assumes BeaconInterval is 102400 us. Users can adjust the parameters based on their own needs.                       


## Wi-Fi mode parameters
* DataMode:           Data mode.  
* datarate:           Data rate of data mode.  
* bandWidth:          BandWidth of data mode.  

Note: Relation between the above 3 parameters and MCS is described in file "MCStoWifiMode".       

## Other parameters
* SimulationTime:     Simulatiom time in seconds after all stations get associated with AP.  
* payloadSize:        Size of payload.                   
* BeaconInterval:     Beacon interval time in us.    
* UdpInterval:        Traffic mode, station send one packet every UdpInterval seconds.  
* Nsta:               Number of total stations.  
* rho:                Maximal distance between AP and stations.   
* seed:               Seed of RandomVariableStream.
* TrafficPath:        Include traffic of each stations, packet sending interval can be automatically calcualted based on payloadSize. The above TrafficPath "./OptimalRawGroup/traffic/data-32-0.82.txt" contains traffic of 32 stations, and the total traffic is 0.82 Mbps.
* S1g1MfieldEnabled:     Packet using 1 Mhz bandwidth if set to "true".


## TIM and page slice parameters
* pagePeriod:       Number of Beacon Intervals between DTIM beacons that carry Page Slice element for the associated page
* pageIndex:        Index of the page (0-3) whose slices are served during beacon intervals within a page period, default value is 0
* pageSliceLength:  Number of blocks in each TIM for the associated page except for the last TIM (1-31)
* pageSliceCount:   Number of TIMs in a single page period (0-31), value 0 has special meaning
* blockOffset:      The 1st page slice starts with the block with blockOffset number, default value is 0
* timOffset:        Offset in number of Beacon Intervals from the DTIM that carries the first page slice of the page, default value is 0

    Example run: ./waf --run "test --pagePeriod=4 --pageSliceLength=8 --pageSliceCount=4"
    
    * pagePeriod=4: every 4th beacon is DTIM beacon that carries Page Slice element for pageIndex=0
    * pageSliceLength=8: each page slice cosists of 8 blocks, meaning that each page slice (PS) accomodates up to 512 stations (8 blocks * 8 subblocks * 8 stations)
      * (slice0: 1-512, slice1: 513-1024, slice2: 1025-1536, slice3: 1537-2048)
      * The last page slice can have different length.
   * pageSliceCount=4: 4 TIMs are scheduled in one page period.
    
    Note: RAW configuration must be in line with TIM and page configuration. If a RAW group is reserved for a station in beacon interval that does not correspond to its TIM, station will be asleep during that RAW.
    
    To configure a single page slice (whole page encoded in a single page slice), it is neccessary to set pageSliceCount to 0 and pageSliceLength to 1.

## Further Reading
For more information on the implementation of the IEEE 802.11ah module for ns-3, check [recent WNS3 paper on ResearchGate](https://www.researchgate.net/publication/324910418_Extension_of_the_IEEE_80211ah_ns-3_Simulation_Module).
> Le Tian, Amina Sljivo, Serena Santi, Eli De Poorter, Jeroen Hoebeke, Jeroen Famaey. **Extension of the IEEE 802.11ah NS-3 Simulation Module.** Workshop on ns-3 (WNS3), 2018.

### Mac Layer
#### Mac Frame Header
* `Type` & `Subtype`

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
| Data | 10(2) | | |
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
There are 5 Mac address: source address (SA), destination address (DA), transmitter address (TA), receiver address (RA) and basic service set identifier (BSSID).
> BSSID is a Layer 2 identifier of the BSS

![5-mac-addr](/Img/futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_5-mac-addr.png)
There are 4 scenarios
![4-to-ds-from-ds](/Img/futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_4-to-ds-from-ds.png)
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
	Typical mesh environment.<br>
	Address 1 – RA, f0:5c:19:65:64:30 – AP BSSID
	Address 2 – TA, f0:5c:19:65:60:f1 – AP BSSID
	Address 3 – DA, cc:44:63:1b:2d:fa – mobile client’s MAC address
	Address 4 – SA, 74:8e:f8:4f:02:76

## Protocol Stack
### MacLow -> MacRxMiddle
In `src/wave/model/ocb-wifi-mac.cc`
```c++
void OcbWifiMac::EnableForWave (Ptr<WaveNetDevice> device)
{
  ...
  m_low->SetRxCallback (MakeCallback (&MacRxMiddle::Receive, m_rxMiddle));
  ...
}
```
### MacLow -> MacRxMiddle -> RegularWifiMac & RegularWifiMac-> DcaTxop/DcaManager -> MacLow
In `src/wifi/model/regular-wifi-mac.cc`
```c++
egularWifiMac::RegularWifiMac ()
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

## **Additive/modified files & folders from the original fork, maintainer must keep those files & folders**
### General Modified Files
* `.gitignore`
* `README.md`
* `LICENSE`
### Removed files
`optimal-RAW-algorithm-fixedraw.sh`, `README`, `RELEASE_NOTES`, `run.pbs`
### 3rd Party Modules
* [Modules/Toolbox](https://github.com/USYD-SDN-Lab/Toolbox)
### Components - all self-defined components
* `Setting.h` : set the configuration across all layers and components
* `PacketContext.h`: the context of a packet across the physical layer to the MAC layer
### Updated Source File (adding new functions)
#### `src/wave/model`
* `ocb-wifi-mac.cc`
```c++
// RegularWifiMac::Receive: 	add a NULL PacketContext
void OcbWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr){
	...
	RegularWifiMac::Receive (packet, hdr, NULL);
}
```
#### `src/wifi/model`
* `wifi-phy.h`
```c++
#include "Components/PacketContext.h"
// RxOkCallback:: add PacketContext as an extra parameter
//typedef Callback<void, Ptr<Packet>, double, WifiTxVector, enum WifiPreamble> RxOkCallback;
typedef Callback<void, Ptr<Packet>, double, WifiTxVector, enum WifiPreamble, PtrPacketContext> RxOkCallback;
```
* `wifi-phy-state-helper.h`
```c++
#include "Components/PacketContext.h"	// add PacketContext header for its C/C++ file
```
* `wifi-phy-state-helper.c`
```c++
// SwitchFromRxEndOk: 						add PacketContext as an extra parameter
// WifiPhy::RxOkCallback m_rxOkCallback: 	add PacketContext as an extra input
void WifiPhyStateHelper::SwitchFromRxEndOk (Ptr<Packet> packet, double snr, WifiTxVector txVector, enum WifiPreamble preamble PtrPacketContext packetContext){
	...
	if (!m_rxOkCallback.IsNull ())
	{
		m_rxOkCallback (packet, snr, txVector, preamble, packetContext);
	}

}
```
* `yans-wifi-phy.cc`
```c++
// extra headers
#include "Modules/Toolbox/FileManager.h"
#include "Components/Settings.h"
#include "Components/PacketContext.h"
// extra namespaces
using namespace Toolbox;
...
// YansWifiPhy::EndReceive: 	create the PacketContext
// SwitchFromRxEndOk: 			transfer the packet to the upper layer
// <DEBUG>: 					DEBUG_SDN, DEBUG_SDN_PHY_PACKET_SIZE_DATA, DEBUG_SDN_PHY_PACKET_SIZE_BEACON control printing
void YansWifiPhy::EndReceive (Ptr<Packet> packet, enum WifiPreamble preamble, uint8_t packetType, Ptr<InterferenceHelper::Event> event)
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
	unsigned int mcs_in = PacketContext::ModeName2MCS(modeName);
	// whether is received
	bool isReceived = false;
	// create the context
	PtrPacketContext packetContext = PacketContext::Create(packetSize, startTime, endTime, per, snr, rxPower, interferePower, modeName);
	...
	// decide whether this packet is received or not
	if (m_plcpSuccess == true){
		...
		if (m_random->GetValue () > snrPer.per){
			...
			// set packetContext to be received
			isReceived = true;
			packetContext->SetIsReceived(true);
			// notify the upper layer that this packet is received
			m_state->SwitchFromRxEndOk (packet, snrPer.snr, event->GetTxVector (), event->GetPreambleType (), packetContext);
		}
		else{
			...
		}
		// record data
		#ifdef DEBUG_SDN
			fm.Open(settings.PathProjectDebug() + settings.TRACK_FILE_YANS_WIFI_PHY);
			fm.AddCSVItem(packetSize);
			fm.AddCSVItem(snr);
			fm.AddCSVItem(per);
			fm.AddCSVItem(rxPower);
			fm.AddCSVItem(interferePower);
			fm.AddCSVItem(modeName);
			fm.AddCSVItem(mcs_in);
			fm.AddCSVItem(isReceived, true);
			fm.Close();
			#ifdef DEBUG_SDN_PHY_PACKET_SIZE_DATA
			if (packetSize == DEBUG_SDN_PHY_PACKET_SIZE_DATA){
				fm.Open(settings.PathProjectDebug() + settings.TRACK_FILE_YANS_WIFI_PHY_DATA);
				fm.AddCSVItem(packetSize);
				fm.AddCSVItem(snr);
				fm.AddCSVItem(per);
				fm.AddCSVItem(rxPower);
				fm.AddCSVItem(interferePower);
				fm.AddCSVItem(modeName);
				fm.AddCSVItem(mcs_in);
				fm.AddCSVItem(isReceived, true);
				fm.Close();
			}
			#ifdef DEBUG_SDN_PHY_PACKET_SIZE_BEACON
				if (packetSize == DEBUG_SDN_PHY_PACKET_SIZE_DATA || packetSize == DEBUG_SDN_PHY_PACKET_SIZE_BEACON){
				fm.Open(settings.PathProjectDebug() + settings.TRACK_FILE_YANS_WIFI_PHY_DATA_BEACON);
				fm.AddCSVItem((int)packetSize);
				fm.AddCSVItem(snr);
				fm.AddCSVItem(per);
				fm.AddCSVItem(rxPower);
				fm.AddCSVItem(interferePower);
				fm.AddCSVItem(modeName);
				fm.AddCSVItem(mcs_in);
				fm.AddCSVItem(isReceived, true);
				fm.Close();
				}
			#endif
			#endif
		#endif
	}
	else{
		...
	}
	...
}
```
* `mac-low.h`
```c++
// extra headers
#include "Components/PacketContext.h"
...
// add PacketContext as extra parameters
typedef Callback<void, Ptr<Packet>, const WifiMacHeader*, PtrPacketContext> MacLowRxCallback;
...
void SetRxCallback (Callback<void,Ptr<Packet>,const WifiMacHeader *, PtrPacketContext> callback);
...
void DeaggregateAmpduAndReceive (Ptr<Packet> aggregatedPacket, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, PtrPacketContext packetContext);
...
void ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, bool ampduSubframe, PtrPacketContext packetContext);
```
* `mac-low.c`
```c++
// extra headers
#include "Modules/Toolbox/FileManager.h"
#include "Components/Settings.h"
#include "Components/PacketContext.h"
// extra namespaces
using namespace Toolbox;
...
// MacLow::DeaggregateAmpduAndReceive: 	add PacketContext as an extra parameter
// ReceiveOk: 							add PacketContext as an extra parameter
void MacLow::DeaggregateAmpduAndReceive (Ptr<Packet> aggregatedPacket, double rxSnr, WifiTxVector txVector, WifiPreamble preamble, PtrPacketContext packetContext){
	...
	if (aggregatedPacket->RemovePacketTag (ampdu)){
		...
      	if (firsthdr.GetAddr1 () == m_self){
			...
          	if (firsthdr.IsAck () || firsthdr.IsBlockAck () || firsthdr.IsBlockAckReq ()){
              	ReceiveOk ((*n).first, rxSnr, txVector, preamble, ampduSubframe, packetContext);
            }else if (firsthdr.IsData () || firsthdr.IsQosData ()){
            	...
              	ReceiveOk ((*n).first, rxSnr, txVector, preamble, ampduSubframe, packetContext);
				...
            }
			...
        }
		...
    }
  	else{
    	ReceiveOk (aggregatedPacket, rxSnr, txVector, preamble, ampduSubframe, packetContext);
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
                      	m_rxCallback ((*last).first, &(*last).second, NULL);
                      	last++;
                    }
                  	m_rxCallback ((*last).first, &(*last).second, NULL);
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
                  m_rxCallback ((*lastComplete).first, &(*lastComplete).second, NULL);
                  lastComplete++;
                }
              	m_rxCallback ((*lastComplete).first, &(*lastComplete).second, NULL);
              	lastComplete++;
            }
          	guard = (*i).second.IsMoreFragments () ? (guard + 1) : ((guard + 16) & 0xfff0);
        }
		...
    }
}
```
* `mac-rx-middle.h`
```c++
// extra headers
#include "Components/PacketContext.h"
...
// add PacketContext as extra parameters
typedef Callback<void, Ptr<Packet>, const WifiMacHeader*, PtrPacketContext> ForwardUpCallback;
...
// add PacketContext as extra parameters
void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext);
```
* `mac-rx-middle.c`
```c++
// MacRxMiddle::Receive: 		add PacketContext as an extra parameter
// m_callback: 					add PacketContext as an extra parameter
void MacRxMiddle::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext){
	...
	m_callback (agregate, hdr, packetContext);
}
```
* `regular-wifi-mac.h`
```c++
#pragma once
...
// extra headers
#include "Components/PacketContext.h"
...
virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext);
```
* `regular-wifi-mac.c`
```c++
void RegularWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext){
	...
}
```
* `adhoc-wifi-mac.cc`
This type of Wifi is a temporary non-centralised network. Usually, it is used for local network users to share files.
```c++
// RegularWifiMac::Receive:: add a NULL PacketContext
void AdhocWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr){
	...
	RegularWifiMac::Receive (packet, hdr, NULL);
}
```
* `ap-wifi-mac.h`
```c++
// extra headers
#include "Components/PacketContext.h"			// add PacketContext header for its C/C++ file
...
// add PacketContext as an extra parameter
virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext);
```
* `ap-wifi-mac.c`
```c++
// ApWifiMac::Receive: 		add PacketContext as an extra parameter
void ApWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext){
}
```
* `sta-wifi-mac.h`
```c++
// extra headers
#include "Components/PacketContext.h"			// add PacketContext header for its C/C++ file
...
// add PacketContext as an extra parameter
virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext);
```
* `sta-wifi-mac.c`
```c++
// StaWifiMac::Receive: 		add PacketContext as an extra parameter
void StaWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr, PtrPacketContext packetContext){
}
```

## Potential Problems
* `MacLow::RxCompleteBufferedPacketsWithSmallerSequence` unknow when to be called and why it is not called
* `MacLow::RxCompleteBufferedPacketsUntilFirstLost` unkown when to be called and why it is not called
* `src/wifi/model/adhoc-wifi-mac.cc`
This change should not be remove when `RegularWifiMac::Receive` supports default `NULL` PacketContext
```c++
// RegularWifiMac::Receive:: add a NULL PacketContext
void AdhocWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr){
	...
	RegularWifiMac::Receive (packet, hdr, NULL);
}
```
* `src/wave/model/ocb-wifi-mac.cc`
This change should not be remove when `RegularWifiMac::Receive` supports default `NULL` PacketContext
```c++
// RegularWifiMac::Receive: 	add a NULL PacketContext
void OcbWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr){
	...
	RegularWifiMac::Receive (packet, hdr, NULL);
}
```