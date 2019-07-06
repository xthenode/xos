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
#   File: mutex.pri
#
# Author: $author$
#   Date: 5/30/2019
#
# QtCreator .pri file for xos executable mutex
########################################################################

########################################################################
# mutex

# mutex TARGET
#
mutex_TARGET = mutex

# mutex INCLUDEPATH
#
mutex_INCLUDEPATH += \
$${xos_INCLUDEPATH} \

# mutex DEFINES
#
mutex_DEFINES += \
$${xos_DEFINES} \

########################################################################
# mutex OBJECTIVE_HEADERS
#
#mutex_OBJECTIVE_HEADERS += \
#$${XOS_SRC}/xos/base/Base.hh \

# mutex OBJECTIVE_SOURCES
#
#mutex_OBJECTIVE_SOURCES += \
#$${XOS_SRC}/xos/base/Base.mm \

########################################################################
# mutex HEADERS
#
mutex_HEADERS += \
$${XENE_SRC}/xos/mt/Mutex.hxx \
$${XOS_SRC}/xos/mt/posix/Mutex.hxx \
$${XOS_SRC}/xos/mt/apple/osx/Mutex.hxx \
$${XOS_SRC}/xos/mt/microsoft/windows/Mutex.hxx \
$${XOS_SRC}/xos/mt/os/Mutex.hxx \
$${XOS_SRC}/xos/platform/Posix.hxx \
\
$${XOS_SRC}/xos/app/console/mt/MainOpt.hxx \
$${XOS_SRC}/xos/app/console/mt/Main.hxx \
$${XOS_SRC}/xos/app/console/mt/mutex/MainOpt.hxx \
$${XOS_SRC}/xos/app/console/mt/mutex/Main.hxx \

# mutex SOURCES
#
mutex_SOURCES += \
$${XOS_SRC}/xos/app/console/mt/MainOpt.cxx \
$${XOS_SRC}/xos/app/console/mt/mutex/Main.cxx \

########################################################################
# mutex FRAMEWORKS
#
mutex_FRAMEWORKS += \
$${xos_FRAMEWORKS} \

# mutex LIBS
#
mutex_LIBS += \
$${xos_LIBS} \


