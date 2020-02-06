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
///   File: Array.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_ARRAY_HXX_
#define _XOS_BASE_ARRAY_HXX_

#include "xos/base/Base.hxx"

#define XOS_ARRAY_DEFAULT_SIZE 128

namespace xos {

typedef ImplementBase ArrayTImplements;
typedef Base ArrayTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: ArrayT
///////////////////////////////////////////////////////////////////////
template
<typename TWhat = char,
 typename TSize = size_t, TSize VSize = XOS_ARRAY_DEFAULT_SIZE,
 class TImplements = ArrayTImplements, class TExtends = ArrayTExtends>

class _EXPORTED_ ArrayT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TWhat what_t;
    typedef TSize size_t;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ArrayT(ssize_t length)
    : m_elements(m_sized), m_size(VSize), m_length(0) {
        SetLength(length);
    }
    ArrayT(const ArrayT& copy)
    : m_elements(m_sized), m_size(VSize), m_length(0) {
        Append(copy.Elements(), copy.Length());
    }
    ArrayT()
    : m_elements(m_sized), m_size(VSize), m_length(0) {
    }
    virtual ~ArrayT() {
        Clear();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual size_t Assign(const TWhat* elements, size_t elementsLength) {
        size_t count = 0;
        size_t newelementsLength;

        if ((!elements) || (!m_elements) || (elementsLength < 1)) {
            return 0;
        }
        if (m_size < (newelementsLength = (elementsLength))) {
            if (0 >= (count = AdjustToSize(NewSize(newelementsLength)))) {
                return count;
            }
        }
        CopyElements(m_elements, elements, elementsLength);
        m_length = (count = elementsLength);
        return count;
    }
    virtual size_t Append(const TWhat* elements, size_t elementsLength) {
        size_t count = 0;
        size_t newelementsLength;

        if ((!elements) || (!m_elements) || (elementsLength < 1)) {
            return 0;
        }
        if (m_size < (newelementsLength = (m_length + elementsLength))) {
            if (0 >= (count = AdjustToSize(NewSize(newelementsLength)))) {
                return count;
            }
        }
        CopyElements(m_elements+m_length, elements, elementsLength);
        m_length += (count = elementsLength);
        return count;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual size_t Set(const TWhat& element){
        return Set(element, m_length);
    }
    virtual size_t Set(const TWhat& element, size_t elementsLength){
        size_t count = 0;
        size_t newelementsLength;

        if ((!m_elements) || (elementsLength < 1)) {
            return 0;
        }
        if (m_size < (newelementsLength = (elementsLength))) {
            if (0 >= (count = AdjustToSize(NewSize(newelementsLength)))) {
                return count;
            }
        }
        SetElements(m_elements, element, elementsLength);
        m_length = (count = elementsLength);
        return count;
    }
    virtual size_t Clear(){
        size_t count = m_length;
        if (m_elements) {
            if (m_elements != m_sized) {
                delete[] m_elements;
            }
        }
        m_elements = m_sized;
        m_size = VSize;
        m_length = 0;
        return count;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual size_t SetLength(ssize_t toLength){
        size_t toSize = 0;

        if (!m_elements) {
            return 0;
        }
        if (0 > toLength) {
            toLength = VSize;
        }
        if ((toSize = (size_t)(toLength)) > m_size) {
            if (toLength > ((ssize_t)(SetSize(toSize)))) {
                return 0;
            }
        }
        m_length = toLength;
        return m_length;
    }
    virtual size_t Length() const {
        return m_length;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual size_t SetSize(size_t toSize){
        if (toSize != m_size){
            AdjustToSize(NewSize(toSize));
        }
        return m_size;
    }
    virtual size_t Size() const {
        return m_size;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual TWhat* Elements() const {
        return m_elements;
    }
    virtual TWhat& operator[](size_t index) const {
        return (TWhat&)(m_elements[index]);
    }
#if !defined(__MSC__)
    virtual operator TWhat*() const {
        return (TWhat*)(m_elements);
    }
#endif // !defined(__MSC__)

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual size_t AdjustToSize(size_t size) {
        size_t count = 0;
        TWhat* elements;

        if (!m_elements) {
            return 0;
        }
        if (size <= m_size) {
            // elements is already big enough
            //
            return m_size;
        }
        if (!(elements = new TWhat[size])) {
            return 0;
        }
        if (m_elements) {
            if (m_length > 0) {
                CopyElements(elements, m_elements, m_length);
            }
            if (m_elements != m_sized) {
                delete m_elements;
            }
        }
        m_elements = elements;
        m_size = size;
        count = m_size;
        return count;
    }
    virtual size_t NewSize(size_t size) const {
        size_t count = 0;
        // Lets increase the buffer size by default buffer
        // sized chunks. Note the desired new size is always
        // needed size + 1. The size in chunks is calculated
        // as (new size + (chunk size - 1)) / chunk size.
        // since new size = needed size + 1 then we have
        // chunks = (needed size + chunk size) / chunk size.
        // Finally we need bytes which is chunks * chunk size
        // which can be reduced to
        // ((needed size / chunk size) + 1) * chunk size
        //
        count = (size = ((size/VSize)+1)*VSize);
        return count;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual size_t SetElements(TWhat* to, const TWhat& from, size_t size) const {
        size_t count = 0;
        if ((to)) {
            for (count = 0; size > 0; --size, count++) {
                (*to++) = from;
            }
        }
        return count;
    }
    virtual size_t CopyElements(TWhat* to, const TWhat* from, size_t size) const {
        size_t count = 0;
        if ((to) && (from)) {
            for (count = 0; size > 0; --size, count++) {
                (*to++) = (*from++);
            }
        }
        return count;
    }
    virtual size_t CountElements(TWhat* what, const TWhat& end) const {
        size_t count = 0;
        if (what) {
            while (end != *(what++)) {
                ++count;
            }
        }
        return count;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    TWhat m_sized[VSize];
    TWhat* m_elements;
    size_t m_size;
    size_t m_length;
};

} // namespace xos 

#endif /// _XOS_BASE_ARRAY_HXX_
