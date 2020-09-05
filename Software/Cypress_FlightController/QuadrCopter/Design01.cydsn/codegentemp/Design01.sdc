# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\data\GitHub\QuadroCopter\Software\Cypress_FlightController\QuadrCopter\Design01.cydsn\Design01.cyprj
# Date: Fri, 04 Sep 2020 19:05:27 GMT
#set_units -time ns
create_clock -name {CyXTAL_32kHz} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/clk_32k_xtal}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {DMA_Timer} -source [get_pins {ClockBlock/clk_sync}] -edges {1 65537 131073} [list [get_pins {ClockBlock/dclk_glb_0}]]


# Component constraints for C:\data\GitHub\QuadroCopter\Software\Cypress_FlightController\QuadrCopter\Design01.cydsn\TopDesign\TopDesign.cysch
# Project: C:\data\GitHub\QuadroCopter\Software\Cypress_FlightController\QuadrCopter\Design01.cydsn\Design01.cyprj
# Date: Fri, 04 Sep 2020 19:05:18 GMT
