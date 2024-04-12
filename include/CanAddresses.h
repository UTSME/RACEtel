#ifndef CANADDRESSES_H
#define CANADDRESSES_H
//unused can ids provided for reference


const int CANID_MOTOR_POSITION_INFO = 0x0A5;// PM150DZ broadcast position info for resolver cali
const int CANID_MC_STATES = 0x0AA;          // PM150DZ broadcast internal states
const int CANID_MC_FAULTS = 0x0AB;          // PM150DZ broadcast faults
const int CANID_MC_DATA = 0x0B0;            // PM150DZ broadcast live data (latency)
const int CANID_BMS_DCLCCL = 0x202;         // PM150DZ broadcast discharge and charge currents for BMS (may not be implemented)

const int CANID_MC_RX = 0x0C0;              // VCU broadcast torque command
const int CANID_BRAKE = 0x245;              // VCU broadcast brake pressure
const int CANID_FAILURES = 0x250;           // VCU broadcast failures

const int CANID_ORIONBMS = 0x03B;           // Orion BMS broadcast main channel
const int CANID_ORIONBMS2 = 0x6B2;          // Orion BMS broadcast second channel

const int CANID_PDMDATA = 0x522;            // PDM broadcast Water Pump Current and diag
const int CANID_PDMSTATUS = 0x520;          // PDM broadcast states and faults

const int CANID_M150_SPEED = 0x400;         // M150 broadcast speed 
const int CANID_M150_TEMPERATURE = 0x502;   // M150 broadcast tractive system temps



#endif
