/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __NOC_PAYLOAD_H__
#define __NOC_PAYLOAD_H__

class noc_payload : public tlm::tlm_extension<noc_payload>
{
public:
  uint64_t id       ;
  uint64_t src_x    ;
  uint64_t src_y    ;
  uint64_t dst_x    ;
  uint64_t dst_y    ;

  tlm::tlm_extension_base* clone() const
     { return new noc_payload(*this); }
  
  void copy_from(tlm::tlm_extension_base const & that )
     { *this = static_cast<const noc_payload&>(that); } // use assignment operator
};
#endif
