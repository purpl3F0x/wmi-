/*  
*   main.cpp
*   -------------------------
*   Enter Description
*   -------------------------
*   Stavros Avramidis	2/5/2019
*/

#include <string>
#include <iostream>

#include "library.h"
// comment(lib, "wmi.lib")


int main(){
  HRESULT hres;
  Wmi wmi;
  wmi.init();

  AdditionalFilters filters;
  filters.emplace("Dependent", ".+VEN_8086&DEV_A2A3&SUBSYS_873C1043&R.+");
  filters.emplace("Antecedent", ".+Port.+");


  std::vector<QueryObj> q_res;
  hres = wmi.query("SELECT * FROM Win32_PnPAllocatedResource", q_res, &filters);

  
  if (hres){
    std::cout << "query error:\t" << hres << std::endl;
    return 0;
  }

  std::cout << q_res.size() << "\n";

  for (auto &i: q_res){
    std::cout << i["Antecedent"] << std::endl;
  }

  return 0;
}
