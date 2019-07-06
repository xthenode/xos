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
#   File: libxos.pro
#
# Author: $author$
#   Date: 5/4/2019
#
# QtCreator .pro file for xos library libxos
########################################################################
include(../../../../../build/QtCreator/xos.pri)
include(../../../../QtCreator/xos.pri)
include(../../xos.pri)
include(../../../../QtCreator/lib/libxos/libxos.pri)

TARGET = $${libxos_TARGET}
TEMPLATE = $${libxos_TEMPLATE}
CONFIG += $${libxos_CONFIG}

########################################################################
# INCLUDEPATH
#
INCLUDEPATH += \
$${libxos_INCLUDEPATH} \

# DEFINES
# 
DEFINES += \
$${libxos_DEFINES} \

########################################################################
# OBJECTIVE_HEADERS
#
OBJECTIVE_HEADERS += \
$${libxos_OBJECTIVE_HEADERS} \

# OBJECTIVE_SOURCES
#
OBJECTIVE_SOURCES += \
$${libxos_OBJECTIVE_SOURCES} \

########################################################################
# HEADERS
#
HEADERS += \
$${libxos_HEADERS} \
$${OBJECTIVE_HEADERS} \

# SOURCES
#
SOURCES += \
$${libxos_SOURCES} \

########################################################################


