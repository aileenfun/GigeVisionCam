#include "StdAfx.h"
#include "propFromFile.h"
#include <libconfig.h++>
#include <iostream>
using namespace std;
using namespace libconfig;

propFromFile::propFromFile(void)
{
	 Config cfg;

  // Read the file. If there is an error, report it and exit.
  try
  {
    cfg.readFile("PropSettings.cfg");
  }
  catch(const FileIOException &fioex)
  {
    std::cerr << "I/O error while reading file." << std::endl;
    //return(EXIT_FAILURE);
  }
  catch(const ParseException &pex)
  {
    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
    //return(EXIT_FAILURE);
  }


  const Setting& root = cfg.getRoot();

  // Output a list of all books in the inventory.
  try
  {
    const Setting &Client = root["client"];
    int count = Client.getLength();

	int camCnt;
	long long hostIP;
	long long clientIP;
	long long MACaddr;
	int packetSize;
	int interval_time;

     // const Setting &book = books[i];
	const Setting &clientSet=Client;
	clientSet.lookupValue("camCnt",camCnt);
	clientSet.lookupValue("hostIP",hostIP);
	clientSet.lookupValue("clientIP",clientIP);
	clientSet.lookupValue("MACaddr",MACaddr);
	clientSet.lookupValue("packetSize",packetSize);
	clientSet.lookupValue("Interval_time",interval_time);
	std::cout<<camCnt<<" "<<hostIP<<"  "<<clientIP<<"  "<<MACaddr<<"  "<<packetSize<<"  "<<interval_time<<std::endl;
     
  }
  catch(const SettingNotFoundException &nfex)
  {
    // Ignore.
  }

  // Output a list of all books in the inventory.
  try
  {
    const Setting &cams = root["Cameras"]["camera"];
    int count = cams.getLength();

    /*cout << setw(6) << left << "name" << "  "
         << setw(6) << left << "serialNum" << "   "
         << setw(6) << left << "triggerMode" << "  "
         << setw(6)<<left<<"exp"<<" "
		 << setw(6) << left << "brightness" << "  "
		 << setw(6) << left << "mirror" << "  "
		 << setw(6) << left << "row" << "  "
		 << setw(6) << left << "col" << "  "
         << endl;*/

    for(int i = 0; i < count; ++i)
    {
      const Setting &cam = cams[i];

      // Only output the record if all of the expected fields are present.
      string name;
      int serialNum;
      int triggerMode;
	  int exp;
	  int brightness;
	  int mirror;
	  int row;
	  int col;

      if(!(cam.lookupValue("name", name)
           && cam.lookupValue("serialNum", serialNum)
           && cam.lookupValue("triggerMode", triggerMode)
           && cam.lookupValue("exp", exp)
		   && cam.lookupValue("brightness", brightness)
		   && cam.lookupValue("mirror", mirror)
		   && cam.lookupValue("exp", exp)
		   && cam.lookupValue("row", row)
		   && cam.lookupValue("col", col)))
        continue;

     /* cout << setw(6) << left << name << "  "
         << setw(6) << left << serialNum << "   "
         << setw(6) << left << triggerMode << "  "
         << setw(6)<<left<<exp<<" "
		 << setw(6) << left << brightness << "  "
		 << setw(6) << left << mirror << "  "
		 << setw(6) << left << row << "  "
		 << setw(6) << left << col << "  "
         << endl;*/
    }
  }
  catch(const SettingNotFoundException &nfex)
  {
    // Ignore.
  }
}


propFromFile::~propFromFile(void)
{
}
int propFromFile::ReadFile()
{
	return 0;
}