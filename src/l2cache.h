/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __L2CACHE_H__
#define __L2CACHE_H__

#include <systemc.h>
#include <tlm.h>

// Convenience Sockets:
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_target_socket.h>
#include "tlm_noc.h"
#include "noc_payload.h"
using namespace tlm;

class l2cache: public sc_core::sc_module {
    SC_HAS_PROCESS(l2cache);
public:
    // TLM interface
    tlm_utils::simple_target_socket<l2cache>    s_port;

    // Constructor  
    l2cache(sc_core::sc_module_name name);

private:
    void peq_callback(gp &trans, const tlm_phase &phase);
    tlm_sync_enum slv_nb_transport_fw(gp& payload, tlm_phase& phase, sc_time& delay);
    tlm_utils::peq_with_cb_and_phase<l2cache> peq;
    uint8_t l2sram[L2_SRAM_BYTE];
    double pkt_counter;
};

#endif
