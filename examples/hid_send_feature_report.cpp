#include "hidapi.h"
#include <iostream>
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
	// set your vid pid here
    auto dev = find_hid_device(0xffff, 0xffff);

    if (dev.empty()) {
        return 0;
    }
    const int feature_report_lenght = 64;
    char data[feature_report_lenght];
    memset(data, 0, feature_report_lenght);
	
	//set report id here
    data[0] = 0x03;

    for (size_t i = 0; i < dev.size(); i++) {
        auto ret = hid_send_feature_report(dev[i], (const unsigned char *)data,
                                           feature_report_lenght);
        std::cout << "ret: " << ret << std::endl;
    }
    return 0;
}
