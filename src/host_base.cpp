/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#include "host_base.h"

// -------------------------------------------------- //
// Constructor
// -------------------------------------------------- //
host_base::host_base(sc_core::sc_module_name name): sc_module(name),
    m_port("m_port")
{
    wdata_ptr= new uint8_t[1024];
    rdata_ptr= new uint8_t[1024];
    m_port.register_nb_transport_bw(this, &host_base::mst_nb_transport_bw);
}




// -------------------------------------------------- //
// m_port Forward
// -------------------------------------------------- //
void host_base::initiator(gp& trans) 
{

    cout << "\033[1;31m"<< name() << endl;

    noc_payload* ext;
    trans.get_extension(ext);
    if (trans.get_command() == TLM_READ_COMMAND)
        cout << "Read ";
    else if (trans.get_command() == TLM_WRITE_COMMAND)
        cout << "Write ";
    cout << "src(" << ext->src_x <<"," << ext->src_y <<")-->" \
             << "dst(" << ext->dst_x <<"," << ext->dst_y << ")" << endl;


    DUMP_FORWARD_PKT_REQ   


    sc_time delay = SC_ZERO_TIME;
    tlm_phase phase = BEGIN_REQ;
    tlm_sync_enum status = m_port->nb_transport_fw(trans, phase, delay);

    // Before sending the next Read/Write operation,
    // wait for the accept delay + 1 clk cycle.
    wait(delay + sc_time(CLK_PRD, SC_NS));
}


// -------------------------------------------------- //
// mst_nb_transport_bw is a callback function used to
//  handle back-transport transactions on the m_port
// -------------------------------------------------- //
tlm_sync_enum host_base::mst_nb_transport_bw(gp& trans, tlm_phase& phase, sc_time& delay)
{
    if (phase == BEGIN_RESP) {
        //DUMP_FORWARD_PKT_RESP
        phase = END_RESP;
    } else {
        cout << "Error  " << __FILE__ ":line " << __LINE__ << endl;
        exit(0);
    }
    
    rdata_ptr =  trans.get_data_ptr();

    cout << sc_time_stamp() << endl;
    cout << "Host Read Data: ";
    for (int i = 0; i < 8; ++i) {
        cout << static_cast<unsigned>(rdata_ptr[i]) << " ";
    }
    cout << endl;

    return TLM_COMPLETED;
}
