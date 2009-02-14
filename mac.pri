###############
# Any of the following may be overriden by the environment
###############

# Wether to build static or shared library
DUBO_LINK_TYPE = static
# DUBO_LINK_TYPE = dynamic

# Commit from December 19, 2018 by default
DUBO_INTERNAL_VERSION = ffd666e16c5f67c7ea45813a233f25cfd3a9d81c
# Where to find the include and built lib output
DUBO_INTERNAL_PATH = build.mac.x86_64

# If you rather want to link against your own, specify DUBO_EXTERNAL, a directory that must contain include and lib folders with the necessary dependencies
# Note this will be used BEFORE any other manually specified source
# DUBO_EXTERNAL =

# Not specifying either means your third-party are already installed system-wide.

# Where to output the final build (will default to buildd/$$platform/$$compiler-$$qtmajorversion-$$linktype-$$buildtype) if left empty
DUBO_DESTDIR =

# Flags to use in order to link to the third-party (lib only)
# DUBO_LIBS = -framework Breakpad -framework AppKit
# Special include paths (appended to external deps, if any)  (lib only)
# DUBO_INC = Frameworks/Breakpad.framework/Headers
# DUBO_INC = crashpad

# Libraries needed by this
DUBO_LIBS = -framework AppKit -framework Security -lbsm -lclient -lbase -lutil
