///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2019 $organization$
///
/// This software is provided by the author and contributors ``as is'' 
/// and any express or implied warranties, including, but not limited to, 
/// the implied warranties of merchantability and fitness for a particular 
/// purpose are disclaimed. In no event shall the author or contributors 
/// be liable for any direct, indirect, incidental, special, exemplary, 
/// or consequential damages (including, but not limited to, procurement 
/// of substitute goods or services; loss of use, data, or profits; or 
/// business interruption) however caused and on any theory of liability, 
/// whether in contract, strict liability, or tort (including negligence 
/// or otherwise) arising in any way out of the use of this software, 
/// even if advised of the possibility of such damage.
///
///   File: Level.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_LOGGER_LEVEL_HXX_
#define _XOS_LOGGER_LEVEL_HXX_

#include "xos/base/Base.hxx"

namespace xos {
namespace logger {

typedef unsigned Level;
enum {
    LevelNone  = 0,

    LevelFatal = 1,
    LevelError = (LevelFatal << 1),
    LevelWarn  = (LevelError << 1),
    LevelInfo  = (LevelWarn << 1),
    LevelDebug = (LevelInfo << 1),
    LevelTrace = (LevelDebug << 1),

    LevelAll   = ((LevelTrace << 1) - 1),

    MessageLevelNone  = 0,

    MessageLevelFatal = (LevelTrace << 1),
    MessageLevelError = (MessageLevelFatal << 1),
    MessageLevelWarn  = (MessageLevelError << 1),
    MessageLevelInfo  = (MessageLevelWarn << 1),
    MessageLevelDebug = (MessageLevelInfo << 1),
    MessageLevelTrace = (MessageLevelDebug << 1),

    MessageLevelAll   = ((LevelAll << LevelTrace))
};

typedef Level Levels;
enum {
    LevelsNone  = 0,

    LevelsFatal = 1,
    LevelsError = (((LevelsFatal + 1) << 1) - 1),
    LevelsWarn  = (((LevelsError + 1) << 1) - 1),
    LevelsInfo  = (((LevelsWarn + 1) << 1) - 1),
    LevelsDebug = (((LevelsInfo + 1) << 1) - 1),
    LevelsTrace = (((LevelsDebug + 1) << 1) - 1),

    LevelsAll   = LevelsTrace,

    MessageLevelsNone  = 0,

    MessageLevelsFatal = (((LevelsFatal) << LevelTrace)),
    MessageLevelsError = (((LevelsError) << LevelTrace)),
    MessageLevelsWarn  = (((LevelsWarn) << LevelTrace)),
    MessageLevelsInfo  = (((LevelsInfo) << LevelTrace)),
    MessageLevelsDebug = (((LevelsDebug) << LevelTrace)),
    MessageLevelsTrace = (((LevelsTrace) << LevelTrace)),

    MessageLevelsAll   = MessageLevelsTrace
};

} /// namespace logger
} /// namespace xos

#define XOS_LOGGING_LEVEL ::xos::logger::Level
#define XOS_LOGGING_LEVEL_ALL ::xos::logger::LevelAll
#define XOS_LOGGING_LEVEL_NONE ::xos::logger::LevelNone
#define XOS_LOGGING_LEVEL_FATAL ::xos::logger::LevelFatal
#define XOS_LOGGING_LEVEL_ERROR ::xos::logger::LevelError
#define XOS_LOGGING_LEVEL_WARN ::xos::logger::LevelWarn
#define XOS_LOGGING_LEVEL_INFO ::xos::logger::LevelInfo
#define XOS_LOGGING_LEVEL_DEBUG ::xos::logger::LevelDebug
#define XOS_LOGGING_LEVEL_TRACE ::xos::logger::LevelTrace

#define XOS_LOGGING_MESSAGE_LEVEL ::xos::logger::MessageLevel
#define XOS_LOGGING_MESSAGE_LEVEL_ALL ::xos::logger::MessageLevelAll
#define XOS_LOGGING_MESSAGE_LEVEL_NONE ::xos::logger::MessageLevelNone
#define XOS_LOGGING_MESSAGE_LEVEL_FATAL ::xos::logger::MessageLevelFatal
#define XOS_LOGGING_MESSAGE_LEVEL_ERROR ::xos::logger::MessageLevelError
#define XOS_LOGGING_MESSAGE_LEVEL_WARN ::xos::logger::MessageLevelWarn
#define XOS_LOGGING_MESSAGE_LEVEL_INFO ::xos::logger::MessageLevelInfo
#define XOS_LOGGING_MESSAGE_LEVEL_DEBUG ::xos::logger::MessageLevelDebug
#define XOS_LOGGING_MESSAGE_LEVEL_TRACE ::xos::logger::MessageLevelTrace

#define XOS_LOGGING_LEVELS ::xos::logger::Level
#define XOS_LOGGING_LEVELS_ALL ::xos::logger::LevelsAll
#define XOS_LOGGING_LEVELS_NONE ::xos::logger::LevelsNone
#define XOS_LOGGING_LEVELS_FATAL ::xos::logger::LevelsFatal
#define XOS_LOGGING_LEVELS_ERROR ::xos::logger::LevelsError
#define XOS_LOGGING_LEVELS_WARN ::xos::logger::LevelsWarn
#define XOS_LOGGING_LEVELS_INFO ::xos::logger::LevelsInfo
#define XOS_LOGGING_LEVELS_DEBUG ::xos::logger::LevelsDebug
#define XOS_LOGGING_LEVELS_TRACE ::xos::logger::LevelsTrace

#define XOS_LOGGING_MESSAGE_LEVELS ::xos::logger::MessageLevel
#define XOS_LOGGING_MESSAGE_LEVELS_ALL ::xos::logger::MessageLevelsAll
#define XOS_LOGGING_MESSAGE_LEVELS_NONE ::xos::logger::MessageLevelsNone
#define XOS_LOGGING_MESSAGE_LEVELS_FATAL ::xos::logger::MessageLevelsFatal
#define XOS_LOGGING_MESSAGE_LEVELS_ERROR ::xos::logger::MessageLevelsError
#define XOS_LOGGING_MESSAGE_LEVELS_WARN ::xos::logger::MessageLevelsWarn
#define XOS_LOGGING_MESSAGE_LEVELS_INFO ::xos::logger::MessageLevelsInfo
#define XOS_LOGGING_MESSAGE_LEVELS_DEBUG ::xos::logger::MessageLevelsDebug
#define XOS_LOGGING_MESSAGE_LEVELS_TRACE ::xos::logger::MessageLevelsTrace

#if !defined(XOS_LOGGING_LEVELS_DEFAULT)
#if defined(TRACE_BUILD)
#define XOS_LOGGING_LEVELS_DEFAULT XOS_LOGGING_LEVELS_TRACE
#else // defined(TRACE_BUILD)
#if defined(DEBUG_BUILD)
#define XOS_LOGGING_LEVELS_DEFAULT XOS_LOGGING_LEVELS_DEBUG
#else // defined(DEBUG_BUILD)
#define XOS_LOGGING_LEVELS_DEFAULT XOS_LOGGING_LEVELS_ERROR
#endif // defined(DEBUG_BUILD)
#endif // defined(TRACE_BUILD)
#endif // !defined(XOS_LOGGING_LEVELS_DEFAULT)

#endif /// _XOS_LOGGER_LEVEL_HXX_
