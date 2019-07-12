#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <SerialStream.h>

#include <iomanip>

using namespace LibSerial;

// GGA - essential fix data which provide 3D location and accuracy data.
//
//  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
//
// Where:
//      GGA          Global Positioning System Fix Data
//      123519       Fix taken at 12:35:19 UTC
//      4807.038,N   Latitude 48 deg 07.038' N
//      01131.000,E  Longitude 11 deg 31.000' E
//      1            Fix quality: 0 = invalid
//                                1 = GPS fix (SPS)
//                                2 = DGPS fix
//                                3 = PPS fix
// 			       4 = Real Time Kinematic
// 			       5 = Float RTK
//                                6 = estimated (dead reckoning) (2.3 feature)
// 			       7 = Manual input mode
// 			       8 = Simulation mode
//      08           Number of satellites being tracked
//      0.9          Horizontal dilution of position
//      545.4,M      Altitude, Meters, above mean sea level
//      46.9,M       Height of geoid (mean sea level) above WGS84
//                       ellipsoid
//      (empty field) time in seconds since last DGPS update
//      (empty field) DGPS station ID number
//      *47          the checksum data, always begins with *

struct GGAStruct
{
	std::string GGAfixTime;
	std::string GGALatitude;
	std::string GGALongitude;
	float GGALatitudeNum;
	float GGALongitudeNum;
	int gps_fix;
	int satNum;
	float horzDOP;
	float alt;
	float heightOfGeoid;
	int empty1;
	int empty2;
};

// $GPGLL,4916.45,N,12311.12,W,225444,A,*1D
//
// Where:
// 	 GLL          Geographic position, Latitude and Longitude
// 	 4916.46,N    Latitude 49 deg. 16.45 min. North
// 	 12311.12,W   Longitude 123 deg. 11.12 min. West
// 	 225444       Fix taken at 22:54:44 UTC
// 	 A            Data Active or V (void)
// 	 *iD          checksum data

struct GLLStruct
{
	std::string GLLLatitude;
	std::string GLLLongitude;
	std::string GLLfixTakenAt;
	std::string dataActive;
};

// $GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39
//
// Where:
// 	 GSA      Satellite status
// 	 A        Auto selection of 2D or 3D fix (M = manual)
// 	 3        3D fix - values include: 1 = no fix
// 																		 2 = 2D fix
// 																		 3 = 3D fix
// 	 04,05... PRNs of satellites used for fix (space for 12)
// 	 2.5      PDOP (dilution of precision)
// 	 1.3      Horizontal dilution of precision (HDOP)
// 	 2.1      Vertical dilution of precision (VDOP)
// 	 *39      the checksum data, always begins with *

struct GSAStruct
{
	std::string autoSelect;
	int GPSFix;
	std::vector<int> PRN;
	float PDOP;
	float HDOP;
	float VDOP;
};

// $GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75
//
// Where:
// 		GSV          Satellites in view
// 		2            Number of sentences for full data
// 		1            sentence 1 of 2
// 		08           Number of satellites in view
//
// 		01           Satellite PRN number
// 		40           Elevation, degrees
// 		083          Azimuth, degrees
// 		46           SNR - higher is better
// 				 for up to 4 satellites per sentence
// 		*75          the checksum data, always begins with *

struct GSVStruct
{
	int fullDataSentNum;
	int sentence;
	int sateliteInView;
	int satPRNNum;
	int elevation;
	int azimuth;
};

// $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
//
// Where:
//      RMC          Recommended Minimum sentence C
//      123519       Fix taken at 12:35:19 UTC
//      A            Status A=active or V=Void.
//      4807.038,N   Latitude 48 deg 07.038' N
//      01131.000,E  Longitude 11 deg 31.000' E
//      022.4        Speed over the ground in knots
//      084.4        Track angle in degrees True
//      230394       Date - 23rd of March 1994
//      003.1,W      Magnetic Variation
//      *6A          The checksum data, always begins with *

struct RMCStruct
{
	std::string RMCFixTaken;
	std::string RMCStatus;
	std::string RMCLatitude;
	std::string RMCLongitude;
	std::string RMCGNDSpeed;
	std::string RMCTrackAngle;
	std::string RMCMagneticVar;
};

// VTG - Velocity made good. The gps receiver may use the LC prefix instead of GP if it is emulating Loran output.
//
//   $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
//
// where:
//         VTG          Track made good and ground speed
//         054.7,T      True track made good (degrees)
//         034.4,M      Magnetic track made good
//         005.5,N      Ground speed, knots
//         010.2,K      Ground speed, Kilometers per hour
//         *48          Checksum

struct VTGStruct
{
	std::string VTGTrueTrack;
	std::string VTGMagTrack;
	std::string VTGGndSpdKnots;
	std::string VTGGndSpdkmph;
};


class GPSDecoder
{
public:
	GPSDecoder();
	~GPSDecoder();

	int initDecoder(std::string);
	int initGPS(std::string);
	int initFiles();
	void closeFile();
	void stopGPS();

	int GPSSentenceCheck(std::string);

	void printGGAData();
	void printGSAData();
	void printGSVData();
	void printGLLData();
	void printRMCData();
	int printKMLData();
	void readFFFData(char*);
	int readGGAData(char*);
	void readGSAData(char*);
	void readGSVData(char*);
	void readGLLData(char*);
	void readRMCData(char*);
	void readTXTData(char*);
	void readVTGData(char*);
	void crunchGPSSentence(std::string);

	void run();

	GGAStruct GGAData;
	GSAStruct GSAData;
	GSVStruct GSVData;
	GLLStruct GLLData;
	RMCStruct RMCData;
	VTGStruct VTGData;

	int iterator = 0;
	bool runGPSWorker = true;
	bool file_init = true;
	bool cleared_buffer=false;
	bool GPSClosed = false;


private:
	std::ofstream file;

		SerialStream UARTStream;
};
