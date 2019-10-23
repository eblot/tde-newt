#-----------------------------------------------------------------------------
# TinyCrypt
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/src)

build_component_from (
  src/endian.c
  src/os.c
  src/os_callout.c
  src/os_cputime.c
  src/os_cputime_1mhz.c
  src/os_cputime_high.c
  src/os_cputime_pwr2.c
  src/os_dev.c
  src/os_error.c
  src/os_eventq.c
  src/os_heap.c
  src/os_mbuf.c
  src/os_mempool.c
  src/os_msys.c
  src/os_mutex.c
  src/os_priv.h
  src/os_sanity.c
  src/os_sched.c
  src/os_sem.c
  src/os_stacktrace.c
  src/os_task.c
  src/os_time.c
  src/arch/cortex_m4/os_arch_arm.c
  src/arch/cortex_m4/os_fault.c
  src/arch/cortex_m4/m4/HAL_CM4.S
  src/arch/cortex_m4/m4/SVC_Table.S
)
