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

![5-mac-addr][futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_5-mac-addr]
There are 4 scenarios
![4-to-ds-from-ds][futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_4-to-ds-from-ds]
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


[futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_5-mac-addr]:data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAaIAAABaCAYAAAD6vVKvAAAABGdBTUEAALGPC/xhBQAAAAFzUkdCAK7OHOkAAAAJcEhZcwAADsMAAA7DAcdvqGQAAA9KSURBVHja7Z09duo8EIaHby1wCw4rcFYAt0mVNp1ThiZdynRpTBm6tFRpAiuIV8BJcWEv/o78KxtpJMs2GPy+59zioniQZNkz+mGeURRFEUEQBEHQhfQfugCCIAiCI4IgCILgiCAIgiAIjgiCIAiCI4IgCIIgOCIIgiAIjqgXOq7objSip92V9GZc3yfa1S2D6vdnj7R7Uo3RI63uRjQaJf/uVkfLMrZDYPPSNsWYvFvREU9nu4qstI18IvF7o/SfFwWHqHsdgsgjivxtdB2K6+tH27plvVX1vp/5Xpylzw5R4Lm36xB4Eaku3voReUGkfEy4MvZ2wGYfbG59Ut9zyFk1ZkQeBYdIOC46BETLxzNEBeNn+oki+pgjYLicfNpGyX0XT+B6gVmdPGN7XM5oqxigx397otkfGqsuY8rYr4PNXticf2zJX7/RCtOiS8yIpFlQNVJNZy66qDmOIPJyu+vka+TP4wi0EqmIv/Wyyhls+oFcbhdtl+tfaV/l+7j2yWVsXbj+rJR58tSUK8uivFqzGnHfpXpVokTXNjj3JzOW6tjM+iUeS5UZX53ZfmncyePzxGZST66Mu0ew2TOb3EwYcpKTIypPTSsvK/ZvmZdc1dlpl02qfyfbsahL9UVadyyVnHCydJXbsC7j6mJug67OxvY4OSL9S1rbhrit1cBF84Kv22c2lVdcp7/MdWlONV7tXlTsS4y5R7DZE5tXu9R+E0tzIS0nycbdYh/QIVuOOP6jPa1pMco29ha0LrZx6WvtUfAyV819mes4zeneD2nzfUxXR95o7d/T3NKm//qcT7fnH5bLfrunfNNyNFlSmH/+RWsvIFXz2DKuLoY2TKYerRcjGilOcHBl6ZfES2z1ljqlpbnDA20md6UlCVUbjt8bCv1Xeh4XS6yvfki/hwb9yY0lzqZNvzhrRn/GLZt0ukeweVab8Zj+QzPa0z8sz5371Fy6R3QIyAuX9Ji9jQ6/FHoBHbKXVfzvp3gJ6eR6nRhbLwHR5puOdKTvDRUvpwY2GS9ET4t9vj8Wt7/LO2Jow/j5J/ns/it56UovV66snYfvLz00bnwX/cnb7LxfIAg6lyMqotvPwKNw+Z5sWs/vyQ+X9K58tic09UJaqgrZ6yxeiLSh7903beiB/o5bsKnf0aS9/Mp7l6LtyZS8cEPJ5OxIq0fLMtbLWrZBRHrihbv+Oj08oCtLZw3O7+HjN21C8yxg/PeBPHkz97iit7VH00mD/uTGEmfT2C9j+jMj2juFth1ExE3vEWx2bzMfcx3MiOGI6viiV/JpTW/xm2ZOH4eA9ovizP0oP2c/puefAwX7RVGW/y6Euy47wz+hZUjJskppMI3p+XVGy8WSZtLSEG/TdRaQLCtly5Jv04D8klOmtGxCv69buzLD0qO5X4olqNn2I1mWZMuaSFomnGzo4WBhU5x23M7yPkuuS2d1rv3JjSXOpkW/xDPs5ST9mzvL01DlJWJoWEqWn+8JB3rb0Qg8IghyfRut6G7yS6/RB15Ig9KOnkZvND00XfaH4IggqBVfdEeT31eK8GO34bihpxEtaIt73qKQaw6CGkgchNjSgnD+YTBeCE4IMyIIgiAIMyIIgiAIgiOCIAiC4IggCIIgCI4IgiAIgiPqUucG4/URxAfY3vn6s0cCbG9ANgHbs8RApBlsqSY+oTGUrHUw3rm/D7A9wPYA24NNwPbay74tJeI8BHtanCOqPDcYDyC+MwmwPW7GBtje8GwOHrbn5uWTiM8GRucKJdOB8TJeTyBdL8PJdIA09+/j2sfXBbA9wPYA24NNwPbaAuMpppTFA+MOcjMvYejAeNI0Nh4UtktXDt/HQt5q1AWwPcD2ANuDTcD2moLxtHNRZ5Bbo6WdbOlCoAPKu7xaQJpT80yQN64u/G40YHuA7SkE2B5gezg1Z30yJFnuHDcCuXVw1ui8ELtrqSdge63bBGwPgi7siHZPghPk0/28KcitCZRMNzuT6nkCSKv/fSzkrYt6ArZX/z4Atmc1+wYYr8c2adiwPXtHFC5pki5TLPYius4YLK4gt/Q5VkLJTGA83duKA6Q5fh8HeXOeBQC2B9ie+j4BtjdcDRm2h+zbENSrtxFge8PUsGF7cEQQ1DtfBNje4NzQwGF7yDUHQT0TYHuD80KDh+1hRgRBEARhRgRBEATBEUEQBEEQHBEEQRAERwRBEARBcESN1UfAXaO2AIyX/M4CuIibuZtPo9s+FQhoZbkvDOC//+zs3Em/PHehFvamR+JfxKsfAK4MajtIqI6lS/R7/8Z1kbGhVn+U+lSdxSFrq7VdzqbF95n6XRxXLp1WlhPManL3tdoGV5sW9cSYqJSJLCWzJU2YL7JyRHmCx0ikRykyJ//09SfAANz1V+m9iSQwHvnbi9yrfo1rMeN7JPo8UODVfFE9ikwMGbSSaPlYiT53TzTZzMj32rC5o6c4pZGcPb1G5K8E/4kEs1RkZI8UkLhW2+Bq06KeGBPKMWEE/9XnpZzyMjiYmR4uZoDKMeAxLwgSpo0XRIFftqsD3HHAMiPMzABk04LcAMarP54MEMKthK2vAhHrj0F+XLN9xgD16t376nUMy8imr0+YWBnjSGO3rs0Kmyx7dmzvvwr8dwLaPOGftdwGZ5uGehoAjFw9hzAmOB5Ta3tE6w3RZ+ott/6avnaZJ5Si3zjxpuwp17QUqUzSyDhcvudlIttx7mErUWq4/KV7EY2ES9pMD3QIvDzTcfJ9pxFEEv0mn/vbMgqBK0u8fREJiCiIFuWpp67tnPT9Uo0uZPwAV8bVhW8D19dcWfN9AhFeyjnVTH29psXXfd52Wj5a3Qd+DDLRKNdnWUqWvNwuN5zrddZxq+CzSIkzRR/vg89G+ctkmzLuWrRlsnmgwLfNNJ7wnx7+Visjks9G9PqbJokV9/inYFK13QZ3m1w9zc/m0MdEkkH/S/nsteaIlEA2MoHq9FA5FjyWd6pqULe9RMtD5di22641Dw6MV6Q1KV1n7GtpvMQcIxlQ6DoGXe67AahneBHXv86yr8Vyi7zslfYxGzzUtVl9ef48059ajVBhDpJ9kOxlLNIb5dnhu2iDs02unuZnU21zQGOCAf91fGrOHVjWG/CYASp37n7pog3nB+MdafVGJwO5V33dSV06jJfExnO8fyEhPN7WgmmSOlIJcWI4waS3Kd4ls9jm2/SQ5kaTIJnOlRdMqiLIiPcTYn5WF21oYFNbT4yJpmOiW0dkCxczeXwdrK3+9jQDLNOU2ULl2uqXAYDxjqtH2jy8nC4/1Onr3XsBZ+xiDLJ1YYB6rFyv4/s6ztYtItTSkk6yjFQ4UWnpWVr2qmcz7Rd5JcL2PuTSgf+kz+N7JqCHXbShmU11PS2fTaXNAY0JDvzX2mGFrXljNjlkkF1fsRVv9GX/TzbRVBvMxYZXtuGWHWA4KK/jN+alAwlcWXUTUtqY49puu2Fd7pfy4Ql/u7UuY+uibYO+r/kyx8MKJxu6Fbvaem6TAyqae+c2BusdVihvoFf7xvawgu66avsU/a3qa1N/mja8XWxyz5DTYYXTA0Pqe9liG1xsGurJPZv8szKMMcEdVkD2bQiCzrleBPDfIMWD/+CIIAg6+94FwH8Dc0MG8B9yzUFQq1HfSJmtwSUDwa0K4L/BeSEj+A8zIgiCIOiiwowIgiAIgiOCIAiC4IggCIIgCI4IgiAIgiOCCDC6QoDR3dTdBIxuWO8wy7Q91+WIqkAznL28sgcUMDpNjQCjA4zu9saEBYzu+hxR/GvoDT0cpDxF+DHa9QgwOmbGBxgdYHS3OSaMMLo+yT4/GJNTygQzU+aTM0PltLAoA7AMMDrA6ACjA4wOMDo+v1uf9J9tRP0pvPFEkS68OluKwWP2Swc6mJkeFmUG1ekEGB1gdIDR5Q81YHQDh9Fd34xIlX029cYqj1tkQraYEW11UbtmBqbMAmuZAViK7EtRkDLKsijj2mCoZ96Piou5skYSbTkNmZh6Vmct5WiOzfat6+saM7PTutTL9GwcSzbRr9WYktpR6mNHuxWb2TMWR/CpMV0Wa9uM+XnGZ8lePs47aIO7TUM9mWcTY6LJc9PHGdHJGv8FgVDOwDLA6ACj6+DgBWB03fYLYHTnGxPXNCM62TOq7h+V/i9744y5YTMjKkdBpx7eIaKo1DNZH07rUiqr1JMrM87qLOvJRXKqMoc9ooLZVKeelWi6EpXxs0FNX7OROlcXbkzYR9S1o1+WS2OKvl1YNyqblaj2ZP/BIfpnn9su2tCEg6Opp+HZNO07DWJMWM8SLys7R3SyzKKGM6mWyWRYVBVKxkPlGPAYCywDjA4wOsDoAKMDjO6aDisg+zYE3boAoxuoeBhdnwRHBEGD8EWA0Q3ODRlgdH0SUvxANxD1AUZnEmB0g/NCV+OEMCOCIAiCMCOCIAiC4IggCIIgCI4IgiAIgiOCIAiCIDii2rooDAvguFsSwHED0pWB4+CI8hcuA1XjoHlOQD3VkdwOHyCA47gaARwHcNztjYkrA8fBEeUqIGYiZ8g6S+vPQfOcgXpz+jiBp3X4q3CA45iAAOA4gONuc0xcFTgOjkgjb0qTiy4xpJHOZEmhJkJ0C3bSB1t+KVVmTCd2pYhUnrnESz0r+driQRBlTrMvQ13Kdu1fxurrREBgSA+i7GvBkKlkCg5/6VDq4z0Fny80bcPm7ovWXkAvWfzzvaGQCjaSse3vS6LgpRLoTGjqrekthyGJ73igAuvTchucbRrq2eBZuf0xMacX4bzeMSu6Ike0pkU2KOMBk8KpOGgeV9YkInOEYdm9kAGOAziOAI4DOA46p5zw0pqU8VVonm1ZLaiXMwwL4DhAwgCOAzjuOsFxty63pbnxX3rwdPsbanDVxYF6NuvNAMe1vIIKcFzn/QJw3DDAcVia0w3+GSnv6fEf7cmj6aRmma0mU/LCDX0fs83KdvaIjqtH2jy8nC4/zO/tH+zdOy1Dn+5NaxhxP5T3KEKbSrJ1EXsFocN6t+t1fF/H2Z7FAYDSkk6yjFQ40WTD2xdLR9KyVz2bab+QtLxmex9y7Um9iid9no/dLtrQzKa6nk2elQGNibi/NO8yqI9LcxpAGgfNMwH1rL+PTqfdTjAsgOMAjgM4DuC46wPH3bqQfRuCAI4bqK4HHHfrgiOCIIDjhumGrggcd+tCrrlBRH0Ax5kEcNzgvBCcEGZEEARBEIQZEQRBEARHBEEQBA1d/wPwUIZUC20RIgAAAABJRU5ErkJggg==
[futher-reading_mac-layer_mac-frame-header_to-ds-from-ds_4-to-ds-from-ds]:data:image/png;base64,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