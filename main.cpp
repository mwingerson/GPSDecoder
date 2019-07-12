

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include "GPSDecoder.h"
#include <unistd.h>
#include <SerialPort.h>
#include <SerialStream.h>

#include <thread>
using namespace LibSerial;

int main(int argc, char** argv )
{

  std::cout << "PROG START" << std::endl;
  std::string paramInput = "/dev/ttyACM0";

  GPSDecoder GPSWorker;

  if(!GPSWorker.initDecoder(paramInput))
  {
    std::cout << "Failed to initialize GPSWorker. Closing" << std::endl;

    return 0;
  }

  std::thread GPSThread(&GPSDecoder::run, std::ref(GPSWorker));

  for(int i=0; i<200;i++)
  {
    std::system("clear");
    std::cout << "Search and Rescue Menu V0.1" << std::endl;
    GPSWorker.printGGAData();
    usleep(100000);
  }

  GPSWorker.runGPSWorker = 0;

  while(!GPSThread.joinable())
  {
    std::cout << "Trying to join thread. Joinable: " << GPSThread.joinable() << std::endl;
  }

	GPSThread.join();

  std::cout << "PROG END" << std::endl;
	return 0;
}
