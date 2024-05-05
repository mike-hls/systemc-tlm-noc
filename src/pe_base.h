/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __PE_BASE_H__
#define __PE_BASE_H__

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "tlm_noc.h"
#include "dumpWave.h"
#include "noc_payload.h"

using namespace std;
using namespace sc_core;
using namespace tlm;

class pe_base: public sc_core::sc_module {
    SC_HAS_PROCESS(pe_base);
public:

    // TLM interface
    tlm_utils::simple_initiator_socket<pe_base> m_port;
    tlm_utils::simple_target_socket<pe_base>    s_port;

    // Constructor  
    pe_base(sc_core::sc_module_name name);


    // Variables & functions
    char sbuff[10000];
    char signal[100];

private:
    void peq_callback(gp &trans, const tlm_phase &phase);
    tlm_sync_enum mst_nb_transport_bw(gp& payload, tlm_phase& phase, sc_time& delay);
    tlm_sync_enum slv_nb_transport_fw(gp& payload, tlm_phase& phase, sc_time& delay);
    tlm_utils::peq_with_cb_and_phase<pe_base> peq;
    uint8_t pe_sram[PE_SRAM_BYTE];
    double pkt_counter;
};

#endif
