########################################################################
# Copyright (c) 1988-2019 $organization$
#
# This software is provided by the author and contributors ``as is''
# and any express or implied warranties, including, but not limited to,
# the implied warranties of merchantability and fitness for a particular
# purpose are disclaimed. In no event shall the author or contributors
# be liable for any direct, indirect, incidental, special, exemplary,
# or consequential damages (including, but not limited to, procurement
# of substitute goods or services; loss of use, data, or profits; or
# business interruption) however caused and on any theory of liability,
# whether in contract, strict liability, or tort (including negligence
# or otherwise) arising in any way out of the use of this software,
# even if advised of the possibility of such damage.
#
#   File: libxos.pri
#
# Author: $author$
#   Date: 5/4/2019
#
# QtCreator .pri file for xos library libxos
########################################################################

########################################################################
# libxos

# libxos TARGET
#
libxos_TARGET = xos
libxos_TEMPLATE = lib
libxos_CONFIG += staticlib

# libxos INCLUDEPATH
#
libxos_INCLUDEPATH += \
$${xos_INCLUDEPATH} \

# libxos DEFINES
#
libxos_DEFINES += \
$${xos_DEFINES} \

########################################################################
# libxos OBJECTIVE_HEADERS
#
#libxos_OBJECTIVE_HEADERS += \
#$${XOS_SRC}/xos/base/Base.h \h \

# libxos OBJECTIVE_SOURCES
#
#libxos_OBJECTIVE_SOURCES += \
#$${XOS_SRC}/xos/base/Base.mm \

########################################################################
# libxos HEADERS
#
libxos_HEADERS += \
$${XOS_SRC}/xos/platform/apple/Osx.h \
$${XOS_SRC}/xos/platform/Build.h \
$${XOS_SRC}/xos/platform/Compiler.h \
$${XOS_SRC}/xos/platform/Configure.h \
$${XOS_SRC}/xos/platform/Defines.h \
$${XOS_SRC}/xos/platform/Includes.h \
$${XOS_SRC}/xos/platform/microsoft/Windows.h \
$${XOS_SRC}/xos/platform/Os.h \
$${XOS_SRC}/xos/platform/Posix.h \
$${XOS_SRC}/xos/platform/Types.h \
\
$${XOS_SRC}/xos/platform/apple/Osx.hxx \
$${XOS_SRC}/xos/platform/microsoft/Windows.hxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/io.h \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/io.hxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/process.h \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/process.hxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/thread.h \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/thread.hxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/Mutex.h \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/Mutex.hxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/Semaphore.h \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/Semaphore.hxx \
$${XOS_SRC}/xos/platform/Posix.hxx \
$${XOS_SRC}/xos/platform/posix/mutex.hxx \
$${XOS_SRC}/xos/platform/posix/Mutexes.hxx \
$${XOS_SRC}/xos/platform/posix/semaphore.hxx \
$${XOS_SRC}/xos/platform/posix/Semaphores.hxx \
$${XOS_SRC}/xos/platform/posix/pthread.h \
$${XOS_SRC}/xos/platform/posix/pthread.hxx \
$${XOS_SRC}/xos/platform/posix/clock.h \
$${XOS_SRC}/xos/platform/posix/clock.hxx \
$${XOS_SRC}/xos/platform/Os.hxx \
\
$${XOS_SRC}/xos/mt/apple/osx/Mutex.hxx \
$${XOS_SRC}/xos/mt/linux/Mutex.hxx \
$${XOS_SRC}/xos/mt/microsoft/windows/Mutex.hxx \
$${XOS_SRC}/xos/mt/os/Mutex.hxx \
$${XOS_SRC}/xos/mt/apple/mach/Os.hxx \
$${XOS_SRC}/xos/mt/os/Os.hxx \
$${XOS_SRC}/xos/mt/posix/Mutex.hxx \
$${XOS_SRC}/xos/mt/Semaphore.hxx \
$${XOS_SRC}/xos/mt/apple/mach/Semaphore.hxx \
$${XOS_SRC}/xos/mt/posix/Semaphore.hxx \
$${XOS_SRC}/xos/mt/os/Semaphore.hxx \
$${XOS_SRC}/xos/mt/Thread.hxx \
$${XOS_SRC}/xos/mt/posix/Thread.hxx \
$${XOS_SRC}/xos/mt/microsoft/windows/crt/Thread.hxx \
\
$${XOS_SRC}/xos/logger/Interface.hxx \
\
$${XOS_SRC}/xos/console/getopt/Main.hxx \
$${XOS_SRC}/xos/console/getopt/MainOpt.hxx \
$${XOS_SRC}/xos/console/Io.hxx \
$${XOS_SRC}/xos/console/Main.hxx \
$${XOS_SRC}/xos/console/Main_main.hxx \
$${XOS_SRC}/xos/console/MainArg.hxx \
\
$${XOS_SRC}/xos/lib/xos/Version.hxx \

# libxos SOURCES
#
libxos_SOURCES += \
\
$${XOS_SRC}/xos/platform/Os.cxx \
$${XOS_SRC}/xos/platform/posix/clock.cxx \
$${XOS_SRC}/xos/platform/posix/pthread.cxx \
$${XOS_SRC}/xos/platform/posix/Mutexes.cxx \
$${XOS_SRC}/xos/platform/posix/Semaphores.cxx \
\
$${XOS_SRC}/xos/platform/posix/microsoft/Windows.cxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/io.cxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/process.cxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/thread.cxx \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/Handle.cxx \
\
$${XOS_SRC}/xos/logger/Interface.cxx \
\
$${XOS_SRC}/xos/console/MainArg.cxx \
$${XOS_SRC}/xos/console/getopt/MainOpt.cxx \
\
$${XOS_SRC}/xos/lib/xos/Version.cxx \

_libxos_SOURCES += \
$${XOS_SRC}/xos/console/Io.cxx \
$${XOS_SRC}/xos/mt/os/Mutex.cxx \
$${XOS_SRC}/xos/mt/posix/Semaphore.cxx \
$${XOS_SRC}/xos/mt/posix/Thread.cxx \
$${XOS_SRC}/xos/mt/microsoft/windows/crt/Thread.cxx \

########################################################################
