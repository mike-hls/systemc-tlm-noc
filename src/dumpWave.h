/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#ifndef __DUMP_PKT_H__
#define __DUMP_PKT_H__

#define DUMP_FORWARD_PKT_REQ   \
        sprintf(signal, "%s%s\n", name(), "_req");  \
        sprintf(sbuff + strlen(sbuff), "%s %s\n", "signal_name", signal);  \
        sprintf(sbuff + strlen(sbuff), "#%llu\n", sc_time_stamp().value()); \
        sprintf(sbuff + strlen(sbuff),  "1%s\n",  signal);
#define DUMP_FORWARD_PKT_ACCEPT  \
            sprintf(sbuff + strlen(sbuff), "#%llu\n", sc_time_stamp().value() + delay.value()); \
            sprintf(sbuff + strlen(sbuff),  "0%s\n",  signal);


#define DUMP_BACKWARD_PKT_RESP   \
        sprintf(signal, "%s%s\n", name(), "_rsp");  \
        sprintf(sbuff + strlen(sbuff), "%s %s\n", "signal_name", signal);  \
        sprintf(sbuff + strlen(sbuff), "#%llu\n", sc_time_stamp().value()); \
        sprintf(sbuff + strlen(sbuff),  "1%s\n",  signal);
#define DUMP_BACKWARD_PKT_END_RESP  \
            sprintf(sbuff + strlen(sbuff), "#%llu\n", sc_time_stamp().value() + delay.value()); \
            sprintf(sbuff + strlen(sbuff),  "0%s\n",  signal);
#endif // __DUMP_PKT_H__
