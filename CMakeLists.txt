#-----------------------------------------------------------------------------
# Newt core
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

# define subprojects
find_subprojects (subprojects)

#-----------------------------------------------------------------------------
# Build configuration
#-----------------------------------------------------------------------------

newt_no_warnings ()

use_sdk_headers (
  core/sys/shell
  core/time/datetime
  core/util/crc
  core/util/mem
  core/util/streamer
)

# build all projects
FOREACH (project ${subprojects})
  ADD_SUBDIRECTORY (${project} ${project})
ENDFOREACH ()
