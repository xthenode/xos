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
#   File: thread.pri
#
# Author: $author$
#   Date: 6/2/2019
#
# QtCreator .pri file for xos executable thread
########################################################################

########################################################################
# thread

# thread TARGET
#
thread_TARGET = thread

# thread INCLUDEPATH
#
thread_INCLUDEPATH += \
$${xos_INCLUDEPATH} \

# thread DEFINES
#
thread_DEFINES += \
$${xos_DEFINES} \

########################################################################
# thread OBJECTIVE_HEADERS
#
#thread_OBJECTIVE_HEADERS += \
#$${XOS_SRC}/xos/base/Base.hh \

# thread OBJECTIVE_SOURCES
#
#thread_OBJECTIVE_SOURCES += \
#$${XOS_SRC}/xos/base/Base.mm \

########################################################################
# thread HEADERS
#
thread_HEADERS += \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/thread.h \
$${XOS_SRC}/xos/platform/posix/microsoft/windows/thread.hxx \
$${XOS_SRC}/xos/mt/microsoft/windows/crt/Thread.hxx \
$${XOS_SRC}/xos/mt/apple/osx/Thread.hxx \
$${XOS_SRC}/xos/mt/posix/Thread.hxx \
$${XOS_SRC}/xos/mt/os/Thread.hxx \
$${XOS_SRC}/xos/app/console/mt/thread/MainOpt.hxx \
$${XOS_SRC}/xos/app/console/mt/thread/Main.hxx \

# thread SOURCES
#
thread_SOURCES += \
$${XOS_SRC}/xos/mt/posix/Thread.cxx \
$${XOS_SRC}/xos/app/console/mt/thread/MainOpt.cxx \
$${XOS_SRC}/xos/app/console/mt/thread/Main.cxx \

########################################################################
# thread FRAMEWORKS
#
thread_FRAMEWORKS += \
$${xos_FRAMEWORKS} \

# thread LIBS
#
thread_LIBS += \
$${xos_LIBS} \


