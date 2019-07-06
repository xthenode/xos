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
#   File: semaphore.pri
#
# Author: $author$
#   Date: 5/30/2019
#
# QtCreator .pri file for xos executable semaphore
########################################################################

########################################################################
# semaphore

# semaphore TARGET
#
semaphore_TARGET = semaphore

# semaphore INCLUDEPATH
#
semaphore_INCLUDEPATH += \
$${xos_INCLUDEPATH} \

# semaphore DEFINES
#
semaphore_DEFINES += \
$${xos_DEFINES} \

########################################################################
# semaphore OBJECTIVE_HEADERS
#
#semaphore_OBJECTIVE_HEADERS += \
#$${XOS_SRC}/xos/base/Base.hh \

# semaphore OBJECTIVE_SOURCES
#
#semaphore_OBJECTIVE_SOURCES += \
#$${XOS_SRC}/xos/base/Base.mm \

########################################################################
# semaphore HEADERS
#
semaphore_HEADERS += \
$${XOS_SRC}/xos/app/console/mt/semaphore/Main.hxx \

# semaphore SOURCES
#
semaphore_SOURCES += \
$${XOS_SRC}/xos/app/console/mt/semaphore/Main.cxx \

########################################################################
# semaphore FRAMEWORKS
#
semaphore_FRAMEWORKS += \
$${xos_FRAMEWORKS} \

# semaphore LIBS
#
semaphore_LIBS += \
$${xos_LIBS} \


