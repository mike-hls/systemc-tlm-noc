/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#include "router.h"

// ------------------------------------------- //
// Constructor
// ------------------------------------------- //
Router::Router(sc_core::sc_module_name name, int row, int col): sc_module(name),
    row_index(row),
    col_index(col),
    peq_route_fw(this, &Router::route_fw_callback),
    peq_route_bw(this, &Router::route_bw_callback)
{
    for (int i = 0; i < N_RTR_PORT; i++) {
        m_port[i].register_nb_transport_bw(this, &Router::mst_nb_transport_bw);
        s_port[i].register_nb_transport_fw(this, &Router::slv_nb_transport_fw);
    }
}



// ------------------------------------------- //
// Forward path (at_1_phase)
// ------------------------------------------- //
tlm_sync_enum Router::slv_nb_transport_fw(gp& trans, tlm_phase& phase, sc_time& delay)
{
    cout << "\033[1;32m" << name() << "___fw" << endl;

    if (phase == BEGIN_REQ) { 
        DUMP_FORWARD_PKT_REQ
    } else {
        cout << "Error  " << __FILE__ ":line " << __LINE__ << endl;
        exit(0);
    }


    // slavePort: nb_transport_fw
    sc_time trans_delay;
    trans_delay = sc_time(RTR_TRANS_DLY, SC_NS);
    peq_route_fw.notify(trans, phase, trans_delay);


    // slavePort: TLM_ACCEPTED(GP, X, T)*
    uint64_t gp_data_length = trans.get_data_length();
    
    double pkt_beat_len;;
    if (trans.get_command() == TLM_READ_COMMAND)
        pkt_beat_len = PKT_HEAD_LEN;
    else
        pkt_beat_len = PKT_HEAD_LEN + PKT_DATA_LEN;
 
    delay = sc_time(pkt_beat_len, SC_NS);

    DUMP_FORWARD_PKT_ACCEPT; 

    return TLM_ACCEPTED;
}

void Router::route_fw_callback(gp& trans, const tlm_phase& phase)
{
    noc_payload* ext;
    trans.get_extension(ext);
    uint32_t m_port_index = FW_DISABLE;

    if (ext->dst_x == row_index && ext->dst_y == col_index) {
        if (trans.get_address() & L2_BASE_ADDR) {
            if (row_index == 0)
                m_port_index = South;
            else if (row_index == N_PE_ROW-1)
                m_port_index = North;
            else if (col_index == 0)
                m_port_index = East;
            else if (col_index == N_PE_COL-1)
                m_port_index = West;
        } else {
            m_port_index = Local;
        }
    }
    else if (ext->dst_x > row_index)
        m_port_index = North;
    else if (ext->dst_y > col_index)
        m_port_index = West;
    else if (ext->dst_y < col_index)
        m_port_index = East;
    else if (ext->dst_x < row_index)
        m_port_index = South;
    else {
        m_port_index = FW_DISABLE;
        cout << "Error  " << __FILE__ "  line" << __LINE__ << endl;
    }

    if (m_port_index != FW_DISABLE) {
        sc_time delay;
        delay = SC_ZERO_TIME;
        tlm_phase beginPhase = BEGIN_REQ;
        tlm_sync_enum resp = m_port[m_port_index]->nb_transport_fw(trans, beginPhase, delay);
    }
}




// ------------------------------------------- //
// Backward path (at_1_phase)
// ------------------------------------------- //

tlm_sync_enum Router::mst_nb_transport_bw(gp &trans, tlm_phase &phase, sc_time &delay)
{
    cout << "\033[1;32m"<< name() << "___bw" << endl;

    if (phase == BEGIN_RESP) { 
        DUMP_BACKWARD_PKT_RESP
    } else {
        cout << "Error  " << __FILE__ ":line " << __LINE__ << endl;
        exit(0);
    }


    // masterPort: nb_transport_fw
    sc_time delay1;
    delay1 = sc_time(RTR_PIPELINE, SC_NS);
    phase = BEGIN_RESP;
    peq_route_bw.notify(trans, phase, delay1);


    // masterPort: TLM_COMPLETED (GP, END_RESP, T2)*
    double pkt_beat_len = PKT_DATA_LEN;
    delay = sc_time(pkt_beat_len, SC_NS);
    phase = END_RESP;

    DUMP_BACKWARD_PKT_END_RESP
    return TLM_COMPLETED;
}

void Router::route_bw_callback(gp& trans, const tlm_phase& phase)
{
    uint32_t s_port_index = BW_DISABLE;
    noc_payload* ext;
    trans.get_extension(ext);
    if (ext->src_x == row_index && ext->src_y == col_index) {
        if (ext->id == 77) {
            if (row_index == 0)
                s_port_index = North;
            else if (row_index == N_PE_ROW-1)
                s_port_index = South;
            else if (col_index == 0)
                s_port_index = West;
            else if (col_index == N_PE_COL-1)
                s_port_index = East;
        } else {
            s_port_index = Local;
        }
    }
    else if (ext->src_y < col_index)
        s_port_index = West;
    else if (ext->src_x < row_index)
        s_port_index = North;
    else if (ext->src_x > row_index)
        s_port_index = South;
    else if (ext->src_y > col_index)
        s_port_index = East;
    else {
        s_port_index = BW_DISABLE;
        cout << "Error  " << __FILE__ "  line" << __LINE__ << endl;
    }

    if (s_port_index != BW_DISABLE) {
        sc_time delay = SC_ZERO_TIME;
        tlm_phase endPhase = phase; // END_RESP
        tlm_sync_enum resp = s_port[s_port_index]->nb_transport_bw(trans, endPhase, delay);
    }
}
