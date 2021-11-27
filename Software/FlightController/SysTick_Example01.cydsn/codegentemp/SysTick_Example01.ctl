-- ======================================================================
-- SysTick_Example01.ctl generated from SysTick_Example01
-- 11/27/2021 at 17:20
-- This file is auto generated. ANY EDITS YOU MAKE MAY BE LOST WHEN THIS FILE IS REGENERATED!!!
-- ======================================================================

-- TopDesign
-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-030(050) kit.
-- =============================================================================

-- === UART ===
attribute port_location of \UART:tx(0)\ : label is "PORT(3,7)";

-- === RGB LED ===
attribute port_location of LED_Min(0) : label is "PORT(6,2)"; -- LED3
attribute port_location of LED_Sec(0) : label is "PORT(6,3)"; -- LED4

-- === USER SWITCH ===
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(6,1)";  --SW2
attribute port_location of REPLACE_WITH_ACTUAL_PIN_NAME(0) : label is "PORT(15,5)"; --SW3

-- PSoC Clock Editor
-- Directives Editor
-- Analog Device Editor
