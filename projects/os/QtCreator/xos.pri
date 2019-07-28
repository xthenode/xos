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
# Os QtCreator .pri file for xos
########################################################################
UNAME = $$system(uname)

contains(UNAME,Darwin) {
XOS_OS = macosx
} else {
contains(UNAME,Linux) {
XOS_OS = linux
} else {
XOS_OS = windows
} # contains(UNAME,Linux)
} # contains(UNAME,Darwin)

contains(XOS_OS_BUILD,XOS_OS) {
XOS_OS_BUILD = $${XOS_OS}
} else {
XOS_OS_BUILD = os
}

#CONFIG += c++11
#CONFIG += c++0x

########################################################################
# xos

# xos INCLUDEPATH
#
xos_INCLUDEPATH += \

# xos DEFINES
#
xos_DEFINES += \

# xos LIBS
#
xos_LIBS += \
$${build_xos_LIBS} \

contains(XOS_OS,macosx|linux) {
xos_LIBS += \
-lpthread \
-ldl
} else {
} # contains(XOS_OS,macosx|linux)

contains(XOS_OS,linux) {
xos_LIBS += \
-lrt
} else {
} # contains(XOS_OS,linux)
