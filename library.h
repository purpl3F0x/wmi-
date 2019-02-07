#pragma once

#include <string>
#include <vector>
#include <map>
#include <regex>

#include <comdef.h>
#include <Wbemidl.h>
#include <Windows.h>

#pragma comment(lib, "wbemuuid.lib")


typedef std::map<std::string, std::string> QueryObj;
typedef std::map<std::string, std::regex> AdditionalFilters;
typedef std::pair<std::string, std::regex> AdditionalFilter;

class Wmi {
/*Wmi Base class*/
 public:
  Wmi();
  ~Wmi();

  HRESULT init();   // Initialises connection to WMI host
  HRESULT query(std::string queryStr,
                std::vector<QueryObj>& queryVectorOut,
                const AdditionalFilters* filters = nullptr);

 private:
  IWbemLocator *pLoc = nullptr;
  IWbemServices *pSvc= nullptr;
};
