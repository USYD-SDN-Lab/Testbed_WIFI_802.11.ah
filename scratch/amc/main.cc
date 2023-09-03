#include "main.h"

/**
 * transfer the noise figure from dB to Linear
 */
double NoiseFigureDB2Linear(double noiseFigure){
    return pow(10.0, noiseFigure/10.0);
};

/**
 * Get the thermal noise power in Watts (at @temperature in K, usually at 290K for 15 degree)
 * @bandwidth: the bandwidth of MCS
 */
double GetNoisePower(double noiseFigure, unsigned int bandwidth, double temperature){
    //thermal noise at 290K in J/s = W
    static const double BOLTZMANN = 1.3803e-23;
    //Nt is the power of thermal noise in W
    double Nt = BOLTZMANN * temperature * bandwidth;
    //receiver noise Floor (W) which accounts for thermal noise and non-idealities of the receiver
    return noiseFigure * Nt;
};


int main(int argc, char *argv[]) {
    double raOptimalBERThreshold[5] = {10e-4, 10e-5, 10e-6, 10e-7, 10e-8};              // BER threshold
    double temperature = 290;                                                           // temperature (15'C)
    double noiseFigure = NoiseFigureDB2Linear(6.8);                                     // noiseFigure 6.8 dB
    // file columns
    FileManager fm;
    // `MCS`, `BER threshold`, `SNR`, `RSSI`
    string path = "./_dist/amc_threshold.csv";                                          // the file to store the mcs threshold

    // open the file
    NS_ASSERT(fm.Open(path) == 200);
    // create the error rate model
    ObjectFactory m_errorRateModel;
    m_errorRateModel.SetTypeId("ns3::YansErrorRateModel");
    Ptr<ErrorRateModel> error = m_errorRateModel.Create<ErrorRateModel> ();
    // test the SNR threshold of each MCS
    WifiMode tmpWifiMode;
    int mcsIdx;
    int berThresholdId;
    double berThreshold;
    double snrThreshold;
    double noise;
    double rssi; 

    for(mcsIdx = 0; mcsIdx < Mcs::Len(); ++mcsIdx){
        tmpWifiMode = WifiMode(Mcs::GetModeNameAt(mcsIdx));
        for(berThresholdId = 0; berThresholdId < 5; ++berThresholdId){
            berThreshold = raOptimalBERThreshold[berThresholdId];
            snrThreshold = error->CalculateSnr(tmpWifiMode, berThreshold);
            noise = GetNoisePower(noiseFigure, tmpWifiMode.GetBandwidth(), temperature);
            rssi = snrThreshold*noise;
            // write
            fm.AddCSVItem(Mcs::FromModeName(Mcs::GetModeNameAt(mcsIdx)));
		    fm.AddCSVItem(berThreshold);
		    fm.AddCSVItem(snrThreshold);
		    fm.AddCSVItem(rssi, true);
        }
        //printf("MCS %d, SNR: %.4f, RSSI: %.20f\n", Mcs::FromModeName(Mcs::GetModeNameAt(mcsIdx)), snrThreshold, rssi);
    }
    fm.Close();
    return 0;
};