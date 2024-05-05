/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef _TLM_ROUTER_H__
#define _TLM_ROUTER_H__

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include "noc_payload.h"
#include "tlm_noc.h"
#include "dumpWave.h"

using namespace std;
using namespace tlm;

class Router: public sc_core::sc_module {
    SC_HAS_PROCESS(Router);
public:
    char signal[100];
    char sbuff[10000];
    int  row_index;
    int  col_index;

    // TLM interface
    tlm_utils::simple_initiator_socket<Router> m_port[N_RTR_PORT];
    tlm_utils::simple_target_socket<Router>    s_port[N_RTR_PORT];

    // Constructor  
    Router(sc_core::sc_module_name name, int row, int col);

private:
    tlm_sync_enum slv_nb_transport_fw(gp& payload, tlm_phase& phase, sc_time& delay);
    tlm_sync_enum mst_nb_transport_bw(gp& payload, tlm_phase& phase, sc_time& delay);
    void route_fw_callback(gp& trans, const tlm_phase& phase);
    void route_bw_callback(gp& trans, const tlm_phase& phase);

    // Declare the peq with callback function
    tlm_utils::peq_with_cb_and_phase<Router> peq_route_fw;
    tlm_utils::peq_with_cb_and_phase<Router> peq_route_bw;
};

#endif
