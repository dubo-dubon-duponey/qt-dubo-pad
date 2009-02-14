# Third-party dependencies

The provided bootstrap script will:

 * pull down crashpad and build the library (statically) and `crashpad_handler` binary
 * pull down breakpad and build the `minidump_stackwalk` and `dump_syms` binaries

The `depot_tools` repository is provided as a git submodule and contains build tools necessary to compile the above.

# TL;DR

`./bootstrap.sh ffd666e16c5f67c7ea45813a233f25cfd3a9d81c mac $(uname -m) 10.10`

And you need at least XCode command-line tools for the library.

Building `dump_syms` requires a full-blown XCode install (not sure why).

# Build-time dependencies

The crashpad library is required at build time for the dubopad library to compile, and is always compiled in statically.

# Runtime dependencies

The `crashpad_handler` binary must be shipped within your application (typically, inside the `Helpers` directory).

See the provided `demo` application for a fully working integration example.

# Additional dependencies

Once compiled, you need to extract symbols for your final application.

This is typically done using the `dump_syms` binary, which should be integrated in your build toolchain.

`minidump_stackwalk` is only useful if you want to process minidumps.

# Licenses

Breakpad is licensed under the BSD License (https://chromium.googlesource.com/breakpad/breakpad/+/master/LICENSE).

Crashpad is licensed under the APL (https://chromium.googlesource.com/crashpad/crashpad/+/master/LICENSE).

# Background information

 * https://chromium.googlesource.com/crashpad/crashpad/+/master/README.md
 * https://chromium.googlesource.com/breakpad/breakpad/+/master/README.md
