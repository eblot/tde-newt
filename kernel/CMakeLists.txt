#-----------------------------------------------------------------------------
# Newt kernel
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

# define subprojects
find_subprojects (subprojects)

#-----------------------------------------------------------------------------
# Build configuration
#-----------------------------------------------------------------------------

newt_no_warnings ()

# build all projects
FOREACH (project ${subprojects})
  ADD_SUBDIRECTORY (${project} ${project})
ENDFOREACH ()
