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
#   File: xos.pri
#
# Author: $author$
#   Date: 5/4/2019
#
# QtCreator .pri file for xos executable xos
########################################################################

########################################################################
# xos

# xos_exe TARGET
#
xos_exe_TARGET = xos

# xos_exe INCLUDEPATH
#
xos_exe_INCLUDEPATH += \
$${xos_INCLUDEPATH} \

# xos_exe DEFINES
#
xos_exe_DEFINES += \
$${xos_DEFINES} \

########################################################################
# xos_exe OBJECTIVE_HEADERS
#
#xos_exe_OBJECTIVE_HEADERS += \
#$${XOS_SRC}/xos/base/Base.hh \

# xos_exe OBJECTIVE_SOURCES
#
#xos_exe_OBJECTIVE_SOURCES += \
#$${XOS_SRC}/xos/base/Base.mm \

########################################################################
# xos_exe HEADERS
#
xos_exe_HEADERS += \
$${XOS_SRC}/xos/app/console/xos/Main.hxx \

# xos_exe SOURCES
#
xos_exe_SOURCES += \
$${XOS_SRC}/xos/app/console/xos/Main.cxx \

_xos_exe_SOURCES += \

########################################################################
# xos_exe FRAMEWORKS
#
xos_exe_FRAMEWORKS += \
$${xos_FRAMEWORKS} \

# xos_exe LIBS
#
xos_exe_LIBS += \
$${xos_LIBS} \


