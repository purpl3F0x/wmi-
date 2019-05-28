/*  
*   main.cpp
*   -------------------------
*   proof of conecpt of the library
*   -------------------------
*   Stavros Avramidis	2/5/2019
*/

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include <conio.h>

#include "library.h"

#pragma comment(lib, "wmi.lib")

int main() {
  Wmi wmi;
  wmi.init();
  std::vector<QueryObj> q_res;

  // Get Motherboard name
  wmi.query("SELECT * FROM Win32_BaseBoard", q_res);
  std::cout << "------------------ MotherBoard -------------------" << std::endl;
  if (!q_res.empty()) {
    auto res = q_res[0];
    std::cout << "Manufacturer\t:\t" << res["Manufacturer"] << '\n';
    std::cout << "Product\t\t:\t" << res["Product"] << '\n';
    std::cout << "Version\t\t:\t" << res["Version"] << '\n';
  }

  // Get BIOS info
  q_res.clear();
  wmi.query("SELECT * FROM Win32_BIOS ", q_res);
  std::cout << "---------------------- BIOS ----------------------" << std::endl;
  if (!q_res.empty()) {
    auto res = q_res[0];
    std::cout << "Manufacturer\t:\t" << res["Manufacturer"] << '\n';
    std::cout << "Bios Version\t:\t" << res["Name"] << '\n';
    std::cout << "Description\t:\t" << res["Description"] << '\n';
  }

  // Get SMBUS info
  q_res.clear();
  std::string devID;
  wmi.query("SELECT * FROM Win32_PnPSignedDriver WHERE description LIKE '\%SMBUS\%'", q_res);
  std::cout << "---------------------- SMBUS ---------------------" << std::endl;
  if (!q_res.empty()) {
    auto res = q_res[0];
    std::cout << "Manufacturer\t:\t" << res["Manufacturer"] << '\n';
    std::cout << "DeviceID\t:\t" << res["DeviceID"] << '\n';
    std::cout << "Description\t:\t" << res["Description"] << '\n';
    std::cout << "Location\t:\t" << res["Location"] << '\n';
    devID = res["DeviceID"];
  }

  std::string rgx = ".+" + devID.substr(4, 33) + ".+";

  q_res.clear();
  AdditionalFilters filters;
  filters.emplace("Dependent", rgx);
  filters.emplace("Antecedent", ".*Port.*");
  wmi.query("SELECT * FROM Win32_PnPAllocatedResource", q_res, &filters);
  if (!q_res.empty()) {
    auto res = q_res[0];
    std::regex rgx(".*StartingAddress=\"(\\d+)\".*");
    std::smatch matches;
    if (std::regex_search(res["Antecedent"], matches, rgx)) {
      std::cout << "Starting Address:\t" << "0x" << std::hex << std::stoi(matches[1].str()) << "\n";
    }
  }

  std::vector<std::string> stacie = {
      "Stacy\'s mom has got it goin\' on",
      "She\'s all I want and I \'ve waited for so long",
      "Stacy can't you see you're just not the girl for me",
      "I know it might be wrong"
  };

}
