/************************************************/
// Copyright tlm_noc contributors.
// Author Mike
// SPDX-License-Identifier: Apache-2.0
/************************************************/
#include <systemc.h>
#include <tlm.h>

#include <tlm_utils/peq_with_cb_and_phase.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include "pe_base.h"
#include "host_base.h"
#include "host.h"
#include "router.h"
#include "l2cache.h"
#include "dumpWave.h"

using namespace std;
using namespace tlm;

#include <systemc.h>
#include <tlm.h>
int sc_main (int, char **)
{
    ofstream outfile("./build/wavedump.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    char blkName[100];

    sc_set_time_resolution(1, SC_NS);
    int i = 0;
    int j = 0;
    int m = 0;
    int h = 0;


    // ----------------------------------- //
    // create object
    // ----------------------------------- //
    pe_base* pe[N_PE];
    for (int i = 0; i < N_PE; i++) {
        sprintf(blkName, "PE%d", i);
        pe[i] = new pe_base(blkName);
    }

    i = 0;
    Router* R[N_PE];
    for (int r = 0; r < N_PE_ROW; r++) {
        for (int c = 0; c < N_PE_COL; c++) {
            sprintf(blkName, "router(%d,%d)", r, c);
            R[i] = new Router(blkName, r, c);
            i++;
        }
    }

    host_base* HP[N_HOST_BASE +1];
    for (int i = 0; i < N_HOST_BASE +1; i++) {
        if (i < 1) {
            sprintf(blkName, "Host_%d", i);
            HP[i] = new host(blkName, i);
        } else {
            sprintf(blkName, "HostDummy_%d", i);
            HP[i] = new host_base(blkName);
        }
    }

    l2cache* mem[N_SM];
    for (int i = 0; i < N_SM; i++) {
        sprintf(blkName, "MEM%d", i);
        mem[i] = new l2cache(blkName);
    }


    // ----------------------------------- //
    // bind PEs 
    // ----------------------------------- //
    for (int i = 0; i < N_PE; i++) {
       pe[i]->m_port.bind(R[i]->s_port[Local]);
       R[i]->m_port[Local].bind(pe[i]->s_port);
    }


    // ----------------------------------- //
    // East2West
    // ----------------------------------- //
    cout << "Route: Eest2West" << endl;
    for (int r = 0; r < N_PE_ROW; r++) {
        for (int c = 0; c < N_PE_COL-1; c++) {
            i = r*N_PE_ROW+c;
            j = i + 1;
            R[i]->m_port[West].bind(R[j]->s_port[West]);
            cout << "R" << i << "<---->" << "R" << j << "   ";
        }
        cout << endl;
    }

    cout << "bind Host:" << endl;
    for (int r = 0; r < N_PE_ROW; r++) {
        i = r*N_PE_COL;
        HP[h++]->m_port.bind(R[i]->s_port[West]);
        cout << "Host" << h-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }

    cout << "bind Mem" << endl;
    for (int r = 0; r < N_PE_ROW; r++) {
        i = (r+1)*N_PE_COL -1;
        R[i]->m_port[West].bind(mem[m++]->s_port);
        cout << "MEM" << m-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }

    
    // ----------------------------------- //
    // West2East
    // ----------------------------------- //
    cout << "Route: West2East" << endl;
    for (int r = 0; r < N_PE_ROW; r++) {
        for (int c = N_PE_COL-1; c > 0; c--) {
            i = r*N_PE_ROW+c;
            j = i - 1;
            R[i]->m_port[East].bind(R[j]->s_port[East]);
            cout << "R" << i << "<---->" << "R" << j << "   ";
        }
        cout << endl;
    }

    cout << "bind Host:" << endl;
    for (int r = 0; r < N_PE_ROW; r++) {
        i = (r+1)*N_PE_COL-1;
        HP[h++]->m_port.bind(R[i]->s_port[East]);
        cout << "Host" << h-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }

    cout << "bind Mem" << endl;
    for (int r = 0; r < N_PE_ROW; r++) {
        i = r*N_PE_COL;
        R[i]->m_port[East].bind(mem[m++]->s_port);
        cout << "MEM" << m-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }


    // ----------------------------------- //
    // South2North
    // ----------------------------------- //
    cout << "Route: South2North" << endl;
    for (int c = 0; c < N_PE_COL; c++) {
        for (int r = 0; r < N_PE_ROW-1; r++) {
            i = r*N_PE_COL+c;
            j = i + N_PE_COL;
            R[i]->m_port[North].bind(R[j]->s_port[North]);
            cout << "R" << i << "<---->" << "R" << j << "   ";
        }
        cout << endl;
    }

    cout << "bind Host:" << endl;
    for (int c = 0; c < N_PE_COL; c++) {
        i = c;
        HP[h++]->m_port.bind(R[i]->s_port[North]);
        cout << "Host" << h-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }

    cout << "bind Mem" << endl;
    for (int c = 0; c < N_PE_COL; c++) {
        i = (N_PE_ROW -1) * N_PE_COL + c;
        R[i]->m_port[North].bind(mem[m++]->s_port);
        cout << "MEM" << m-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }

    // ----------------------------------- //
    // North2South
    // ----------------------------------- //
    cout << "Route: North2South" << endl;
    for (int c = 0; c < N_PE_COL; c++) {
        for (int r = N_PE_ROW-1; r > 0; r--) {
            i = r*N_PE_COL+c;
            j = i - N_PE_COL;
            R[i]->m_port[South].bind(R[j]->s_port[South]);
            cout << "R" << i << "<---->" << "R" << j << "   ";
        }
        cout << endl;
    }

    cout << "bind Host:" << endl;
    for (int c = 0; c < N_PE_COL; c++) {
        i = (N_PE_ROW-1) * N_PE_COL+ c;
        HP[h++]->m_port.bind(R[i]->s_port[South]);
        cout << "Host" << h-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }

    cout << "bind Mem" << endl;
    for (int c = 0; c < N_PE_COL; c++) {
        i = c;
        R[i]->m_port[South].bind(mem[m++]->s_port);
        cout << "MEM" << m-1 << "<---->" << "R" << i << "   ";
        cout << endl;
    }


    sc_start(100, SC_MS);

    cout << "----dump Wavefrom file----" << endl;
    for (int i=0; i<9; i++)
        outfile << R[i]->sbuff << endl;


    return 0;
}
