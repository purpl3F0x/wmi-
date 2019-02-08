Simple WMI wrapper for c++

Use:
```cpp
  HRESULT hres;
  Wmi wmi;
  wmi.init();

  std::vector<QueryObj> q_res;
  hres = wmi.query("SELECT * FROM Win32_PnPAllocatedResource '\%'", q_res);
  
  if (hres){
    std::cout << "query error:\t" << hres << std::endl;
    return 0;
  }

  std::cout << q_res.size() << "\n";

  for (auto &i: q_res){
    std::cout << i["DeviceID"];
  }
```



> Stavros Avramidis

_Never Settle and keep Running_
