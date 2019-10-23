#-----------------------------------------------------------------------------
# Newt BSP
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

use_sdk_headers (core/sys/flash_map)

build_component_from (
  ${NEWT_BSP}/src/hal_bsp.c
  ${NEWT_BSP}/src/sbrk.c
  ${NEWT_BSP}/src/arch/cortex_m4/gcc_startup_nrf52.S
  ${NEWT_BSP}/src/arch/cortex_m4/gcc_startup_nrf52_split.S
)
