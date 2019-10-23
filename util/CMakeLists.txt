#-----------------------------------------------------------------------------
# Newt Util
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

ADD_DEFINITIONS (-DSTREAMER_MBUF_PRINTF_MAX=128)

build_component_from (
  crc/src/crc8.c
  crc/src/crc16.c
  mem/src/mem.c
  streamer/src/streamer.c
  streamer/src/streamer_console.c
  streamer/src/streamer_mbuf.c
)
