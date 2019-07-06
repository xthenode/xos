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
#   File: xos.pro
#
# Author: $author$
#   Date: 5/4/2019
#
# QtCreator .pro file for xos executable xos
########################################################################
include(../../../../../build/QtCreator/xos.pri)
include(../../../../QtCreator/xos.pri)
include(../../xos.pri)
include(../../../../QtCreator/app/xos/xos.pri)

TARGET = $${xos_exe_TARGET}

########################################################################
# INCLUDEPATH
#
INCLUDEPATH += \
$${xos_exe_INCLUDEPATH} \

# DEFINES
# 
DEFINES += \
$${xos_exe_DEFINES} \

########################################################################
# OBJECTIVE_HEADERS
#
OBJECTIVE_HEADERS += \
$${xos_exe_OBJECTIVE_HEADERS} \

# OBJECTIVE_SOURCES
#
OBJECTIVE_SOURCES += \
$${xos_exe_OBJECTIVE_SOURCES} \

########################################################################
# HEADERS
#
HEADERS += \
$${xos_exe_HEADERS} \
$${OBJECTIVE_HEADERS} \

# SOURCES
#
SOURCES += \
$${xos_exe_SOURCES} \

########################################################################
# FRAMEWORKS
#
FRAMEWORKS += \
$${xos_exe_FRAMEWORKS} \

# LIBS
#
LIBS += \
$${xos_exe_LIBS} \
$${FRAMEWORKS} \


