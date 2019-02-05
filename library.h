#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <comdef.h>
#include <Wbemidl.h>
#include <Windows.h>

#pragma comment(lib, "wbemuuid.lib")

/* TODO: add parameters on init to enable remote connections
 * */


typedef std::map<std::string, std::string> QueryObj;


class Wmi {
/*Wmi Base class*/
 public:
  Wmi();
  ~Wmi();

  HRESULT init();   // Initialises connection to WMI host
  HRESULT query(std::string queryStr, std::vector<QueryObj>& queryVectorOut);

 private:
  IWbemLocator *pLoc = nullptr;
  IWbemServices *pSvc= nullptr;
};
