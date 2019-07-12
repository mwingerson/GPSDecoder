

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

  //std::cout << "Before GPSWorker init" << std::endl;
  GPSDecoder GPSWorker;
  //std::cout << "After GPSWorker init" << std::endl;
  if(!GPSWorker.initDecoder(paramInput))
  {
    std::cout << "Failed to initialize GPSWorker. Closing" << std::endl;

    return 0;
  }

  std::thread GPSThread(&GPSDecoder::run, std::ref(GPSWorker));

  while(GPSWorker.runGPSWorker)
  {
    //std::system("clear");
    //std::cout << "Search and Rescue Menu V0.1" << std::endl;
    usleep(100000000);
    GPSWorker.runGPSWorker = 0;
  }

  while(!GPSThread.joinable())
  {
  std::cout << "Trying to join thread. Joinable: " << GPSThread.joinable() << std::endl;
  }

  std::cout << "before try" << std::endl;
  try
  {
	   GPSThread.join();
  }
  catch(...)
  {
    std::cout << "ERROR" << std::endl;
  }
  std::cout << "after try" << std::endl;

  std::cout << "PROG END" << std::endl;
	return 0;
}
