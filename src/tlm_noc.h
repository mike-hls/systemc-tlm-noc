/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __TLM_NOC_H__
#define __TLM_NOC_H__

typedef tlm::tlm_generic_payload gp;

//clock period 1ns
#define CLK_PRD          1

#define FW_DISABLE       0xff
#define BW_DISABLE       0xff
#define East             0
#define West             1
#define South            2
#define North            3
#define Local            4
#define N_PE_ROW         3
#define N_PE_COL         3
#define N_PE             N_PE_ROW * N_PE_COL
#define N_SM             N_PE_ROW*2 + N_PE_COL*2 
#define N_HOST_BASE      N_SM -1
#define N_PE_DUMMY       N_SM

// PKT: PACKET
#define PKT_DATA_LEN     8
#define PKT_HEAD_LEN     1


// RTR:ROUTER
#define N_RTR_PORT       5
#define RTR_PIPELINE     5
#define RTR_TRANS_DLY    RTR_PIPELINE * CLK_PRD
#define RTR_BIT_WIDTH    512

#define PE_BASE_ADDR     0x00000000
#define L2_BASE_ADDR     0x10000000
#define L2_MASK_ADDR     0x0fffffff

#define L2_SRAM_BYTE     0x1024*512
#define PE_SRAM_BYTE     0x1024*128

#endif
