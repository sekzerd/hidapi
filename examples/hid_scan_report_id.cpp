#include "hidapi.h"
#include <iostream>
#include <string>
#include <vector>

std::vector<hid_device *> find_hid_device(const int vendor_id,
                                          const int product_id) {
  hid_init();
  hid_device_info *pDevs = NULL, *pCur_Devs = NULL;
  pDevs = hid_enumerate(0, 0);
  pCur_Devs = pDevs;
  std::vector<hid_device *> ret;

  for (; pCur_Devs != nullptr; pCur_Devs = pCur_Devs->next) {
    if (pCur_Devs->vendor_id == vendor_id &&
        pCur_Devs->product_id == product_id) {

      hid_device *hid_device = hid_open_path(pCur_Devs->path);
      //    hid_open()
      if (!hid_device) {
        continue;
      }
      ret.push_back(hid_device);
    }
  }
  hid_free_enumeration(pDevs);
  return ret;
}

int main(int argc, char *argv[]) {
  // set vid pid here
  auto dev = find_hid_device(0xffff, 0xffff);

  if (dev.empty()) {
    return 0;
  }
  std::vector<std::string> result;
  const int feature_report_lenght = 20;
  char data[feature_report_lenght];
  memset(data, 0, feature_report_lenght);
  for (size_t j = 0; j < 255; ++j) {
    data[0] = j;
    for (size_t i = 0; i < dev.size(); i++) {

      auto ret =
          hid_write(dev[i], (const unsigned char *)data, feature_report_lenght);
      if (ret != -1) {
        std::cout << "report id: " << j << std::endl;
        std::cout << "dev id: " << i << std::endl;
        result.push_back(std::string("report id:" + std::to_string(j)) + "\t" +
                         std::string("dev id:" + std::to_string(i)));
      }
    }
  }
  for (size_t i = 0; i < result.size(); i++) {
    std::cout << result[i] << std::endl;
  }

  return 0;
}
