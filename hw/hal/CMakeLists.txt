#-----------------------------------------------------------------------------
# NimBLE HAL
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

ADD_DEFINITIONS (-DHAL_FLASH_VERIFY_WRITES=0)
ADD_DEFINITIONS (-DHAL_FLASH_VERIFY_ERASES=0)
ADD_DEFINITIONS (-DHAL_FLASH_VERIFY_BUF_SZ=16)
ADD_DEFINITIONS (-DHAL_SYSTEM_RESET_CB=0)

build_component_from (
  src/hal_common.c
  src/hal_flash.c
)
