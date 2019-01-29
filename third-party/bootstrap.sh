#!/bin/sh

checkout_crashpad(){
    local revision="$1"
    mkdir crashpad
    cd crashpad
    PATH=$(pwd)/../depot_tools:$PATH fetch crashpad
    PATH=$(pwd)/../depot_tools:$PATH gclient sync --revision "$revision"
    cd ->/dev/null
}

checkout_breakpad(){
    local revision="$1"
    mkdir breakpad
    cd breakpad
    PATH=$(pwd)/../depot_tools:$PATH fetch breakpad
    PATH=$(pwd)/../depot_tools:$PATH gclient sync --revision "$revision"
    cd ->/dev/null

    cd breakpad/src
    git apply $(pwd)/../../dump_syms.patch
    cd ->/dev/null
}

build_crashpad(){
    local os="$2"
    local arch="$3"
    local dest="build.crashpad.$1.$os.$arch"
    local target="$4"
    local gn_arch
    case "$arch" in
        x86_64)
            gn_arch="x64"
            ;;
        *)
            exit 1
            ;;
    esac

    mkdir -p "$dest"
    cd crashpad/crashpad
    PATH=$(pwd)/../../depot_tools:$PATH gn gen ../../"$dest" --args="mac_deployment_target=\"$target\" target_os=\"$os\" target_cpu=\"$gn_arch\""
    PATH=$(pwd)/../../depot_tools:$PATH ninja -C ../../"$dest"
    cd ->/dev/null

    mkdir -p "build.$os.$arch/lib"
    cd "build.$os.$arch/lib"
    rm -f *.a
    ln -s "$(pwd)/../../$dest/obj/client/libclient.a" .
    ln -s "$(pwd)/../../$dest/obj/util/libutil.a" .
    ln -s "$(pwd)/../../$dest/obj/third_party/mini_chromium/mini_chromium/base/libbase.a" .
    cd - >/dev/null

    mkdir -p "build.$os.$arch/bin"
    cd "build.$os.$arch/bin"
    cp "$(pwd)/../../$dest/crashpad_handler" .
    cd - >/dev/null
}

build_breakpad(){
    local os="$2"
    local arch="$3"
    local dest="build.breakpad.$1.$os.$arch"
    local target="$4"

    cd breakpad/src
    ./configure
    make
    xcodebuild GCC_TREAT_WARNINGS_AS_ERRORS=NO -sdk macosx$(xcrun --show-sdk-version) -project src/tools/mac/dump_syms/dump_syms.xcodeproj -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=YES MACOSX_DEPLOYMENT_TARGET="$target" CLANG_CXX_LIBRARY=libc++ CLANG_CXX_LANGUAGE_STANDARD=c++0x
    cd - >/dev/null

    mkdir -p "build.$os.$arch/bin"
    cd "build.$os.$arch/bin"
    cp "$(pwd)/../../breakpad/src/src/processor/minidump_stackwalk" .
    cp "$(pwd)/../../breakpad/src/src/tools/mac/dump_syms/build/Release/dump_syms" .
    cd ->/dev/null
}

link_crashpad(){
    local os="$1"
    local arch="$2"
    mkdir -p "build.$os.$arch/include"
    cd "build.$os.$arch/include"
    rm -f client
    rm -f build
    rm -f util
    rm -f base
    ln -s "$(pwd)/../../crashpad/crashpad/client" .
    ln -s "$(pwd)/../../crashpad/crashpad/util" .
    ln -s "$(pwd)/../../crashpad/crashpad/third_party/mini_chromium/mini_chromium/build" .
    ln -s "$(pwd)/../../crashpad/crashpad/third_party/mini_chromium/mini_chromium/base" .
    cd - >/dev/null
}

revision="$1"
os="$2"
arch="$3"

mac_deployment_target="$4"

# Clone only if not there
if [ ! -d "crashpad/crashpad" ]; then
    rm -Rf crashpad
    checkout_crashpad "$revision"
fi

if [ ! -d "breakpad/src" ]; then
    rm -Rf breakpad
    checkout_breakpad "chrome_64"
fi

if [ ! -e "build.$os.$arch/include/client" ]; then
    build_crashpad "$revision" "$os" "$arch" "$mac_deployment_target"
    link_crashpad "$os" "$arch"
fi

if [ ! -e "build.$os.$arch/bin/dump_syms" ]; then
    build_breakpad "chrome_64" "$os" "$arch" "$mac_deployment_target"
fi
