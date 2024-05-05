/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#include "l2cache.h"

l2cache::l2cache(sc_core::sc_module_name name): sc_module(name),
    s_port("s_port"),
    peq(this, &l2cache::peq_callback)
{
    s_port.register_nb_transport_fw(this, &l2cache::slv_nb_transport_fw);
    pkt_counter = 0;
}


void l2cache::peq_callback(gp& trans, const tlm_phase& phase)
{
    if (phase == BEGIN_RESP) {
        sc_time delay = SC_ZERO_TIME;
        tlm_phase bw_phase = BEGIN_RESP;
        s_port->nb_transport_bw(trans, bw_phase, delay);
    }
}

tlm_sync_enum l2cache::slv_nb_transport_fw(gp& trans, tlm_phase& phase, sc_time& delay)
{
    uint64_t burst_byte_len = trans.get_streaming_width()*RTR_BIT_WIDTH/8;    
    uint64_t sram_addr = trans.get_address() & L2_MASK_ADDR;
    noc_payload* ext1;
    trans.get_extension(ext1);

    if (phase == BEGIN_REQ) {
        if (trans.is_read()) {
            cout << "L2 Read: (sx=" << ext1->src_x << ", sy=" << ext1->src_y << ")" << endl;
            memcpy(trans.get_data_ptr(), &l2sram[sram_addr], burst_byte_len);
            uint8_t* rdata_ptr =  trans.get_data_ptr();

            // Set the response status to TLM_OK_RESPONSE 
            trans.set_response_status(TLM_OK_RESPONSE);

            // Initiate the BEGIN_RESP phase using PEQ.
            sc_time delay = sc_time((pkt_counter++) * PKT_DATA_LEN, SC_NS);
            peq.notify(trans, BEGIN_RESP, delay);
            return TLM_ACCEPTED;
        } else if (trans.is_write()) {
            cout << "L2 Write: (sx=" << ext1->src_x << ", sy=" << ext1->src_y << ")" << endl;
            memcpy(&l2sram[sram_addr], trans.get_data_ptr(), burst_byte_len);

            // Set the response status to TLM_OK_RESPONSE 
            trans.set_response_status(TLM_OK_RESPONSE);

            // disable the BEGIN_RESP phase in Write operation
            return TLM_ACCEPTED;
        }
    }
    return TLM_COMPLETED;
}
