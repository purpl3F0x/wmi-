#pragma once


// c++ libs
#include <string>
#include <vector>
#include <map>
#include <regex>

// c libs
#include <comdef.h>

// windows libs
#include <Wbemidl.h>
#include <Windows.h>

#pragma comment(lib, "wbemuuid.lib")

typedef std::map<std::string, std::string> QueryObj;
typedef std::map<std::string, std::regex> AdditionalFilters;
typedef std::pair<std::string, std::regex> AdditionalFilter;

class Wmi {
/**
 *  Wmi Base Class
 */
 public:
  Wmi();
  ~Wmi();
  /**
   * @breif Initialises connection to WMI host
   * @return HRESULT if any problem occurs
   */
  HRESULT init();

  /**
   * @brief makes a wmi query
   * @param queryStr    query sting (see https://docs.microsoft.com/en-us/windows/desktop/wmisdk/wql-sql-for-wmi)
   * @param queryVectorOut vector of QueryObj to hold results
   * @param filters optional pair of filters to be applied to WMi yields
   * @return HRESULT if any problem occurs else H
   */
  HRESULT query(const std::string queryStr,
                std::vector<QueryObj> &queryVectorOut,
                const AdditionalFilters *filters = nullptr);

 private:
  IWbemLocator *pLoc = nullptr;
  IWbemServices *pSvc = nullptr;
};
