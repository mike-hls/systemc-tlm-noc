SYSTEMC=/Users/abc/tools/sc-2.3.2
SYSTEMC_LIB=$(firstword $(wildcard $(SYSTEMC)/lib*))

OUT_DIR := build
$(shell mkdir -p $(OUT_DIR))


WAVE_DIR := wave
$(shell mkdir -p $(WAVE_DIR))

.PHONY: all clean vcd 

all: tlm_noc vcd

clean:
	rm -fr $(OUT_DIR)/*.*  $(WAVE_DIR)/*.*

vcd:
	python3 ./script/gen_vcd.py $(OUT_DIR)/wavedump.txt $(WAVE_DIR)/trace.vcd

tlm_noc:
	g++ -O0  -g -ggdb -Wno-deprecated  \
		-DDEBUG_SYSTEMC \
		-I./ -I$(SYSTEMC)/include \
		-I./src   \
		./src/host_base.cpp   \
		./src/pe_base.cpp   \
		./src/router.cpp   \
		./src/l2cache.cpp   \
		./src/host.cpp   \
		./src/main.cpp  \
		-o ./$(OUT_DIR)/noc.exe \
		-L$(SYSTEMC_LIB) -lsystemc
	LD_LIBRARY_PATH=$(SYSTEMC_LIB):$(LD_LIBRARY_PATH) ./$(OUT_DIR)/noc.exe

