#include "library.h"

#include <iostream>

// Taken from https://stackoverflow.com/questions/215963/
// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring& wstr) {
  if (wstr.empty())
    return std::string();
  int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(),
                                        NULL, 0, NULL, NULL);
  std::string strTo(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0],
                      size_needed, NULL, NULL);
  return strTo;
}

Wmi::Wmi() : pLoc(nullptr), pSvc(nullptr) {};

Wmi::~Wmi() {
  pSvc->Release();
  pLoc->Release();
  CoUninitialize();
}

HRESULT Wmi::init() {
  HRESULT hres;

  // Initialize COM. ------------------------------------------
  hres = CoInitializeEx(0, COINIT_MULTITHREADED);
  if (FAILED(hres)) {
    return hres;
  }

  // Set general COM security levels --------------------------
  hres = CoInitializeSecurity(
      NULL,
      -1,                          // COM authentication
      NULL,                        // Authentication services
      NULL,                        // Reserved
      RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
      RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
      NULL,                        // Authentication info
      EOAC_NONE,                   // Additional capabilities
      NULL                         // Reserved
  );

  if (FAILED(hres)) {
    CoUninitialize();
    return hres;
  }

  // Obtain the initial locator to WMI -------------------------
  hres = CoCreateInstance(
      CLSID_WbemLocator,
      nullptr,
      CLSCTX_INPROC_SERVER,
      IID_IWbemLocator, (LPVOID*) &pLoc);

  if (FAILED(hres)) {
    CoUninitialize();
    return hres;
  }

  hres = pLoc->ConnectServer(
      _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
      NULL,                    // User name. NULL = current user
      NULL,                    // User password. NULL = current
      nullptr,                 // Locale. NULL indicates current
      NULL,                    // Security flags.
      nullptr,                 // Authority (for example, Kerberos)
      nullptr,                 // Context object
      &pSvc                    // pointer to IWbemServices proxy
  );

  if (FAILED(hres)) {
    pLoc->Release();
    CoUninitialize();
    return hres;
  }

  // Set security levels on the proxy -------------------------
  hres = CoSetProxyBlanket(
      pSvc,                        // Indicates the proxy to set
      RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
      RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
      NULL,                        // Server principal name
      RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
      RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
      NULL,                        // client identity
      EOAC_NONE                    // proxy capabilities
  );

  if (FAILED(hres)) {
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
    return hres;
  }

  // Initialised WMI successfully
  return S_OK;
}

HRESULT Wmi::query(std::string queryStr, std::vector<QueryObj>& queryVectorOut) {
  HRESULT hres;
  IEnumWbemClassObject* pEnumerator = nullptr;
  // Make the WMI query
  hres = pSvc->ExecQuery(
      bstr_t("WQL"),
      bstr_t(queryStr.c_str()),
      WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
      nullptr,
      &pEnumerator);

  if (FAILED(hres))
    return hres;

  IWbemClassObject* pclsObj = nullptr;
  ULONG uReturn = 0;

  while (pEnumerator) {
    hres = pEnumerator->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn);
    if (0==uReturn) {
      break;
    }

    SAFEARRAY* sfArray;
    LONG lstart, lend;
    VARIANT vtProp;

    //Get Wmi objects names
    pclsObj->GetNames(0, WBEM_FLAG_ALWAYS, 0, &sfArray);
    // Find safe array boundaries
    SafeArrayGetLBound(sfArray, 1, &lstart);
    SafeArrayGetUBound(sfArray, 1, &lend);

    BSTR* pbstr;
    SafeArrayAccessData(sfArray, (void HUGEP**) &pbstr);
    int nIdx = 0;

    if (SUCCEEDED(hres)) {
      CIMTYPE pType;
      QueryObj item;
      for (nIdx = lstart; nIdx < lend; nIdx++) {
        pclsObj->Get(pbstr[nIdx], 0, &vtProp, &pType, 0);

        if (vtProp.vt==VT_NULL)  continue;

        if (pType==CIM_STRING && pType!=CIM_EMPTY && pType!=CIM_ILLEGAL)
          item.emplace(utf8_encode(pbstr[nIdx]), utf8_encode(vtProp.bstrVal));

        VariantClear(&vtProp);
      }
      // Push item to vector
      queryVectorOut.emplace_back(item);

      SafeArrayUnaccessData(sfArray);
    }
    VariantClear(&vtProp);
    pclsObj->Release();
  }

  pEnumerator->Release();

  return S_OK;
}
