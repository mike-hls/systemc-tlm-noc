#***********************************************/
# Copyright tlm_noc contributors.
# Author Mike
# SPDX-License-Identifier: Apache-2.0
#***********************************************/
import sys

def dumpFile2VCD(filename):
    result = []
    signal = []
    value  = []
    dumpvars = []
    
    result.append("$timescale")
    result.append("      1 ns")
    result.append("$end      ")
    result.append("          ")
    result.append("$scope module SystemC $end")


#$dumpvars
    with open(filename, 'r') as file:
        #extract signals from input file
        for line in file:
            items = line.strip().split(' ')
            if items[0] == "signal_name":
                str = "$var wire 1" + " " + items[1] + " " + items[1] + " " + "$end"
                if str not in signal:
                    signal.append(str)
                    dumpvars.append("0"+items[1])
            else:
                value.extend(items)
            #result.extend(items)
    
        signal.append("$upscope $end")
        signal.append("$enddefinitions  $end")


    signal.append("$dumpvars")
    signal.extend(dumpvars)
    signal.append("$end")
        
    result.extend(signal)
    result.extend(value)
    #print(signal)
    #print(result)
    return result

def write_list_to_file(result_list, output_filename):
    with open(output_filename, 'w') as file:
        for item in result_list:
            file.write("%s\n" % item)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py inFileName outFileName")
        sys.exit(1)

    input_filename = sys.argv[1]
    output_filename = sys.argv[2]
    separator = ' '

    result_list = dumpFile2VCD(input_filename)

    write_list_to_file(result_list, output_filename)
