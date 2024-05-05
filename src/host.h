/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __HOST_H__
#define __HOST_H__

#include <sstream>
#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "host_base.h"



using namespace std;
using namespace sc_core;
using namespace tlm_utils;
using namespace tlm;

class host: public host_base {
    SC_HAS_PROCESS(host);
public:
    int32_t host_id;
    host(sc_core::sc_module_name name, int32_t hid);
private:
    void thread(void);
    void read(uint64_t* src, uint64_t* dst, uint64 id, uint64_t addr);
    void write(uint64_t* src, uint64_t* dst, uint64 id, uint64_t addr);
};
#endif
