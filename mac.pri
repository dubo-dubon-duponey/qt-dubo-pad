###############
# Any of the following may be overriden by the environment
###############

# Commit from December 19, 2018 by default
DUBO_INTERNAL_VERSION = ffd666e16c5f67c7ea45813a233f25cfd3a9d81c
# Where to find the include and built lib output
DUBO_INTERNAL_PATH = build.mac.x86_64

# Libraries needed by this
DUBO_LIBS = -framework AppKit -framework Security -lbsm -lclient -lbase -lutil
