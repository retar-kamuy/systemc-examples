#include <systemc.h>
#include "Vpicorv32_wrapper.h"
#include "verilated_vcd_sc.h"

int sc_main(int argc, char** argv) {
	printf("Built with %s %s.\n", Verilated::productName(), Verilated::productVersion());
	printf("Recommended: Verilator 4.0 or later.\n");

	Verilated::commandArgs(argc, argv);
	sc_clock clk{"clk", 10, SC_NS, 0.5, 3, SC_NS, true};
	sc_signal<bool> resetn;
	sc_signal<bool> trap;
    sc_signal<bool> trace_valid;
    sc_signal<uint64_t> trace_data;

	Vpicorv32_wrapper* top = new Vpicorv32_wrapper{"top"};

    top->clk(clk);
	top->resetn(resetn);
	top->trap(trap);
    top->trace_valid(trace_valid);
    top->trace_data(trace_data);

	sc_start(1, SC_NS);

	// Tracing (vcd)
	VerilatedVcdSc* tfp = NULL;
    const char* flag_vcd = Verilated::commandArgsPlusMatch("vcd");
	if (flag_vcd && 0==strcmp(flag_vcd, "+vcd")) {
		Verilated::traceEverOn(true);
		tfp = new VerilatedVcdSc;
		top->trace (tfp, 99);
		tfp->open("testbench.vcd");
	}

	// Tracing (data bus, see showtrace.py)
	FILE *trace_fd = NULL;
	const char* flag_trace = Verilated::commandArgsPlusMatch("trace");
	if (flag_trace && 0==strcmp(flag_trace, "+trace")) {
		trace_fd = fopen("testbench.trace", "w");
	}

	int t = 0;
    while (!Verilated::gotFinish()) {
        if (t > 200)
			resetn.write(1);
		// top->clk = !top->clk;
        sc_start(1, SC_NS);
		//if (tfp) tfp->dump (t);
		if (trace_fd && top->clk && top->trace_valid) fprintf(trace_fd, "%9.9lx\n", top->trace_data);
		t += 5;
    }

	top->final();

    if (tfp) {
		tfp->flush();
		tfp->close();
		tfp = NULL;
	}
    delete top;
    return 0;
}
