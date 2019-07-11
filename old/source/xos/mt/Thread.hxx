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
///   File: Thread.hxx
///
/// Author: $author$
///   Date: 5/31/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_THREAD_HXX_
#define _XOS_MT_THREAD_HXX_

#include "xos/base/Ran.hxx"
#include "xos/base/Suspended.hxx"
#include "xos/base/Joined.hxx"
#include "xos/base/logged.hxx"
#include "xos/base/Created.hxx"

#if !defined(XOS_MT_THREAD_CREATED)
#define XOS_MT_THREAD_CREATED(initiallySuspended) \
    IS_ERR_LOGGED_DEBUG("this->Created(initiallySuspended)..."); \
    if (!(this->Created(false))) { \
        IS_ERR_LOGGED_ERROR("...failed on this->Created(initiallySuspended) throw (CreateException(CreateFailed)..."); \
        throw (CreateException(CreateFailed)); \
    } else { \
        IS_ERR_LOGGED_DEBUG("...this->Created(initiallySuspended)"); \
    }
#endif /// !defined(XOS_MT_THREAD_CREATED)

#if !defined(XOS_MT_THREAD_DESTROYED)
#define XOS_MT_THREAD_DESTROYED() \
    IS_ERR_LOGGED_DEBUG("this->Destroyed()..."); \
    if (!(this->Destroyed())) {  \
        IS_ERR_LOGGED_ERROR("...failed on this->Destroyed() throw (CreateException(DestroyFailed))..."); \
        throw (CreateException(DestroyFailed)); \
    } else { \
        IS_ERR_LOGGED_DEBUG("...this->Destroyed()"); \
    }
#endif /// !defined(XOS_MT_THREAD_DESTROYED)

#if !defined(XOS_MT_THREAD_UNEXPECTED)
#define XOS_MT_THREAD_UNEXPECTED() \
    IS_ERR_LOGGED_ERROR("...unexpected throw (Exception(ExceptionFailed)..."); \
    throw (Exception(ExceptionFailed));
#endif /// !defined(XOS_MT_THREAD_UNEXPECTED)

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: ThreadJoinedT
///////////////////////////////////////////////////////////////////////
template <class TRan = Ran, class TSuspended = Suspended, class TJoined = Joined>
class _EXPORT_CLASS ThreadJoinedT
: virtual public TRan, virtual public TSuspended, virtual public TJoined {
public:
    typedef TRan Ran;
}; /// class _EXPORT_CLASS ThreadJoinedT

typedef ThreadJoinedT<> ThreadJoined;

///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<class TJoined = ThreadJoined, 
 class TLogged = LoggedT<TJoined>, 
 class TImplements = CreatorT<TLogged> >

class _EXPORT_CLASS ThreadT: virtual public TImplements {
public:
    typedef TImplements Implements;
    typedef typename Implements::Ran Ran;
}; /// class _EXPORT_CLASS ThreadT

typedef ThreadT<> Thread;

namespace implemented {
///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TAttacher = AttacherT<TAttached, TUnattached, VUnattached, mt::Thread>,
 class TImplements = TAttacher>

class _EXPORT_CLASS ThreadT: virtual public TImplements {
public:
    typedef TImplements Implements;

    typedef typename Implements::Ran Ran;
    
    typedef TAttached Attached;
    typedef TUnattached UnattachedT;
    static const TUnattached Unattached = VUnattached;

    using Implements::Created;
    virtual bool Created(bool initiallySuspended) { 
        if ((initiallySuspended) && (!this->IsCreated())) {
            IS_ERR_LOGGED_DEBUG("this->Create(initiallySuspended)...");
            if ((this->Create(initiallySuspended))) {
                return true;
            }
        }
        IS_ERR_LOGGED_DEBUG("this->Created()...");
        if ((this->Created())) {
            return true;
        }
        return false;
    }
    using Implements::Create;
    virtual bool Create(bool initiallySuspended) { 
        return false; 
    }
    virtual Attached Detach() {
        Attached detached = Implements::Detach();
        IS_ERR_LOGGED_DEBUG("this->SetIsForked(false)...");
        this->SetIsForked(false);
        return detached;
    }

    virtual bool Join() { 
        IS_ERR_LOGGED_DEBUG("this->UntimedJoin()...");
        if (JoinSuccess == (this->UntimedJoin())) {
            return true;
        }
        return false; 
    }
    virtual JoinStatus TryJoin() { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            IS_ERR_LOGGED_DEBUG("TryJoinDetached(detached)...");
            return TryJoinDetached(detached);
        }
        return JoinFailed; 
    }
    virtual JoinStatus TimedJoin(mseconds_t milliseconds) { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            IS_ERR_LOGGED_DEBUG("TimedJoinDetached(detached, milliseconds)...");
            return TimedJoinDetached(detached, milliseconds);
        }
        return JoinFailed; 
    }
    virtual JoinStatus UntimedJoin() { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            IS_ERR_LOGGED_DEBUG("UntimedJoinDetached(detached)...");
            return UntimedJoinDetached(detached);
        }
        return JoinFailed; 
    }

    virtual JoinStatus TryJoinDetached(Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
        }
        return JoinFailed; 
    }
    virtual JoinStatus TimedJoinDetached(Attached detached, mseconds_t milliseconds) const { 
        if (((Attached)Unattached) != (detached)) {
        }
        return JoinFailed; 
    }
    virtual JoinStatus UntimedJoinDetached(Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
        }
        return JoinFailed; 
    }
}; /// class _EXPORT_CLASS ThreadT
typedef ThreadT<> Thread;
} /// namespace implemented

namespace extended {
///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0,
 typename TAttached = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::ThreadT<TAttached, TUnattached, VUnattached>,
 class TLogged = xos::extended::LoggedT<TImplement, Base>,
 class TAttachedT = AttachedT<TAttached, TUnattached, VUnattached, TImplement, TLogged>,
 class TCreated = CreatedT<TAttached, TUnattached, VUnattached, TImplement, TAttachedT>,
 class TImplements = TImplement, class TExtends = TCreated>

class _EXPORT_CLASS ThreadT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::Ran Ran;

    typedef TError error_t;
    static const TError ErrorSuccess = VErrorSuccess;

    typedef TAttached Attached;
    typedef TUnattached UnattachedT;
    static const TUnattached Unattached = VUnattached;

    ThreadT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged)
    : Extends(detached, isCreated), _ran(*this), _forked(false) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isErrLogged);
    }
    ThreadT(Attached detached, bool isCreated, bool isLogged)
    : Extends(detached, isCreated), _ran(*this), _forked(false) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isLogged);
    }
    ThreadT(Attached detached, bool isCreated)
    : Extends(detached, isCreated), _ran(*this), _forked(false) {
    }
    ThreadT(Attached detached)
    : Extends(detached), _ran(*this), _forked(false) {
    }
    ThreadT(bool isLogged, bool isErrLogged): _ran(*this), _forked(false) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isErrLogged);
    }
    ThreadT(bool isLogged): _ran(*this), _forked(false) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isLogged);
    }
    ThreadT(Ran& ran): _ran(ran), _forked(false) {
    }
    ThreadT(): _ran(*this), _forked(false) {
    }
    virtual ~ThreadT() {
        XOS_MT_THREAD_DESTROYED();
    }
private:
    ThreadT(const ThreadT& copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }

public:
    virtual bool Create(bool initiallySuspended) {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateAttached(initiallySuspended)...");
        if (((Attached)Unattached) != (detached = CreateAttached(initiallySuspended))) {
            this->SetIsCreated();
            this->SetIsForked();
            return true;
        }
        return false;
    }
    virtual bool Create() {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateAttached()...");
        if (((Attached)Unattached) != (detached = CreateAttached())) {
            IS_ERR_LOGGED_DEBUG("this->SetIsCreated()...");
            this->SetIsCreated();
            IS_ERR_LOGGED_DEBUG("this->SetIsForked()...");
            this->SetIsForked();
            return true;
        }
        return false;
    }
    using Extends::CreateAttached;
    virtual Attached CreateAttached(bool initiallySuspended) {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateDetached(initiallySuspended)...");
        if ((detached = CreateDetached(initiallySuspended))) {
            this->Attach(detached);
        }
        return detached;
    }
    using Extends::CreateDetached;
    virtual Attached CreateDetached(bool initiallySuspended) const {
        Attached detached = ((Attached)Unattached);
        return detached;
    }
    virtual bool Destroy() {
        IS_ERR_LOGGED_DEBUG("this->Joined()...");
        if ((this->Joined())) {
            IS_ERR_LOGGED_DEBUG("this->Detach()...");
            this->Detach();
            return true;
        }
        return false;
    }
    virtual Attached DetachCreated(bool& isCreated) {
        Attached detached = Extends::DetachCreated(isCreated);
        IS_ERR_LOGGED_DEBUG("this->SetIsForked(false)...");
        this->SetIsForked(false);
        return detached;
    }

    virtual bool SetIsForked(bool to = true) {
        IS_ERR_LOGGED_DEBUG("..._forked = " << to);
        _forked = to;
        return IsForked();
    }
    virtual bool IsForked() const {
        return _forked;
    }

    virtual JoinStatus TryJoinDetached(Attached detached) const { 
        return TryJoinDetached((bool&)_forked, detached); 
    }
    virtual JoinStatus TimedJoinDetached(Attached detached, mseconds_t milliseconds) const { 
        return TimedJoinDetached((bool&)_forked, detached, milliseconds); 
    }
    virtual JoinStatus UntimedJoinDetached(Attached detached) const { 
        IS_ERR_LOGGED_DEBUG("UntimedJoinDetached((bool&)_forked, detached)...");
        return UntimedJoinDetached((bool&)_forked, detached); 
    }
    virtual JoinStatus TryJoinDetached(bool& forked, Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
        }
        return JoinFailed; 
    }
    virtual JoinStatus TimedJoinDetached(bool& forked, Attached detached, mseconds_t milliseconds) const { 
        if (((Attached)Unattached) != (detached)) {
        }
        return JoinFailed; 
    }
    virtual JoinStatus UntimedJoinDetached(bool& forked, Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
        }
        return JoinFailed; 
    }

    virtual void Run() {
        if (this != (&_ran)) {
            _ran.Run();
        }
    }

protected:
    Ran& _ran;
    bool _forked;
}; /// class _EXPORT_CLASS ThreadT
typedef ThreadT<> Thread;
} /// namespace extended

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_THREAD_HXX_
