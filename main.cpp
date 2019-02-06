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
#pragma comment(lib, "wmi.lib")


int main(){
  HRESULT hres;
  Wmi wmi;
  wmi.init();

  std::vector<QueryObj> q_res;
  hres = wmi.query("SELECT * FROM Win32_PnPAllocatedResource WHERE Dependent.description LIKE '\%'", q_res);
  
  if (hres){
    std::cout << "query error:\t" << hres << std::endl;
    return 0;
  }

  std::cout << q_res.size() << "\n";

  for (auto &i: q_res){
    std::cout << i["DeviceID"];
  }


  return 0;
}
