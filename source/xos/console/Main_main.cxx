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
///   File: Main_main.cxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#include "xos/mt/Locked.hxx"
#include "xos/console/Logger.hxx"
#include "xos/console/Main_main.hxx"

namespace xos {
namespace console {

} /// namespace console
} /// namespace xos

///////////////////////////////////////////////////////////////////////
/// Function: main
///////////////////////////////////////////////////////////////////////
int main(int argc, char** argv, char** env) {
    int err = 1;
    ERR_LOG_DEBUG("try {...");
    try {
        ::xos::mt::Locked locked(false, true);
        ::xos::console::base::Io io(locked);
        ::xos::console::Logger logger(io);
        
        LOG_DEBUG("::xos::console::Main::TheMain(argc, argv, env)...");
        err = ::xos::console::Main::TheMain(argc, argv, env);
        LOG_DEBUG("..." << err << " = ::xos::console::Main::TheMain(argc, argv, env)...");

        ERR_LOG_DEBUG("...} try");
    } catch (...) {
        ERR_LOG_ERROR("...catch (...)");
    }
    return err;
}
