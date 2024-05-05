/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#include "host.h"

host::host(sc_core::sc_module_name name, int32_t hid): host_base(name),
    host_id(hid)
{
    SC_THREAD(thread);
}

void host::read(uint64_t* src, uint64_t* dst, uint64 id, uint64_t addr)
{
    noc_payload* ext = new noc_payload;
    ext->src_x     = src[0];
    ext->src_y     = src[1];
    ext->dst_x     = dst[0];
    ext->dst_y     = dst[1];
    ext->id        = id;

    gp* trans = new gp;
    trans->set_extension(ext);

    trans->set_command(TLM_READ_COMMAND);
    trans->set_address(addr);
    trans->set_data_ptr(rdata_ptr);
    trans->set_streaming_width(PKT_DATA_LEN);
    //trans->set_data_length(pkt_len);
    //trans->set_byte_enable_ptr(nullptr);
    //trans->set_dmi_allowed(false);
    //trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    initiator(*trans);
}

void host::write(uint64_t* src, uint64_t* dst, uint64 id, uint64_t addr)
{
    noc_payload* ext = new noc_payload;
    ext->src_x = src[0];
    ext->src_y = src[1];
    ext->dst_x = dst[0];
    ext->dst_y = dst[1];
    ext->id    = id;

    gp* trans = new gp;
    trans->set_extension(ext);

    trans->set_command(TLM_WRITE_COMMAND);
    trans->set_address(addr);
    trans->set_data_ptr(wdata_ptr);
    trans->set_streaming_width(PKT_DATA_LEN);

    //trans->set_data_length(length);
    //trans->set_byte_enable_ptr(nullptr);
    //trans->set_dmi_allowed(false);
    //trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    initiator(*trans);
}


void host::thread()
{
    // Init write buffer
    for (uint8_t i=0; i<20; i++)
    {
        wdata_ptr[i] = i+8;
    }


    // read/write test
    uint64_t addr        = 0x1000000a;
    uint64_t id          = 77;
    uint64_t src_node[2] = {host_id,0};
    uint64_t dst_node[2] = {2,2};
 
    write(src_node, dst_node, id, addr);
    addr = 0x0000000a;
    write(src_node, dst_node, id, addr);
    read(src_node, dst_node, id, addr);
    read(src_node, dst_node, id, addr);
    read(src_node, dst_node, id, addr);
}
