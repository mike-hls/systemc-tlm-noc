/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __HOST_BASE_H__
#define __HOST_BASE_H__

#include <cstdlib>
#include <systemc.h>
#include <queue>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>

#include "tlm_noc.h"
#include "noc_payload.h"
#include "dumpWave.h"

using namespace std;
using namespace tlm;
using namespace tlm_utils;;


class host_base: public sc_core::sc_module {
    SC_HAS_PROCESS(host_base);
public:

    // TLM interface
    tlm_utils::simple_initiator_socket<host_base> m_port;

    // Constructor  
    host_base(sc_core::sc_module_name name);


    // Variables & functions
    char sbuff[10000];
    char signal[100];
    void initiator(gp& trans);
    uint8_t* wdata_ptr;
    uint8_t* rdata_ptr;


private:
    tlm_sync_enum mst_nb_transport_bw(gp& payload, tlm_phase& phase, sc_time& delay);
};

#endif
