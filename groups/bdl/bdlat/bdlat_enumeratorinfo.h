// bdlat_enumeratorinfo.h                                             -*-C++-*-
#ifndef INCLUDED_BDLAT_ENUMERATORINFO
#define INCLUDED_BDLAT_ENUMERATORINFO

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a container for enumerator information.
//
//@CLASSES:
//   bdlat_EnumeratorInfo: container for enumerator information
//
//@SEE_ALSO:
//
//@AUTHOR: Clay Wilson (cwilson9)
//
//@DESCRIPTION:
// This component provides the 'bdlat_EnumeratorInfo' 'class' which is a
// container for holding information (properties) about an enumerator.  The
// properties of an enumerator include its name and the length of its name, its
// value, and a brief annotation.  Although each enumerator property is
// publicly accessible, a manipulator and accessor is also supplied for each.
//
///Usage
///-----
// TBD doc

#ifndef INCLUDED_BDLSCM_VERSION
#include <bdlscm_version.h>
#endif

#ifndef INCLUDED_BDLAT_BDEATOVERRIDES
#include <bdlat_bdeatoverrides.h>
#endif

#ifndef INCLUDED_BSLALG_TYPETRAITS
#include <bslalg_typetraits.h>
#endif

#ifndef INCLUDED_BSL_CLIMITS
#include <bsl_climits.h>
#endif

#ifndef INCLUDED_BSL_CSTRING
#include <bsl_cstring.h>
#endif

#ifndef INCLUDED_BSL_IOSFWD
#include <bsl_iosfwd.h>
#endif


namespace BloombergLP {

                        // ===========================
                        // struct bdlat_EnumeratorInfo
                        // ===========================

struct bdlat_EnumeratorInfo {
    // This 'struct' holds information about an enumerator.  Its data members
    // are 'public' by design so that instances may be statically initialized.

    // PUBLIC DATA -- DO *NOT* REORDER
    int         d_value;         // value of enumerator
    const char *d_name_p;        // name of enumerator
    int         d_nameLength;    // length of enumerator name
                                 // (0-terminator not included)
    const char *d_annotation_p;  // enumerator annotation

    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(bdlat_EnumeratorInfo,
                                   bsl::is_trivially_copyable);
    BSLMF_NESTED_TRAIT_DECLARATION(bdlat_EnumeratorInfo,
                                   bsl::is_trivially_default_constructible);

    // CREATORS
    // The following methods are not defined by design:
    //..
    //   bdlat_EnumeratorInfo();
    //   bdlat_EnumeratorInfo(const bdlat_EnumeratorInfo& original);
    //   ~bdlat_EnumeratorInfo();
    //..
    // The corresponding methods supplied by the compiler are sufficient.

    // MANIPULATORS
    // The following method is not defined by design:
    //..
    //   bdlat_EnumeratorInfo& operator=(const bdlat_EnumeratorInfo& rhs);
    //..
    // The assignment operator supplied by the compiler is sufficient.

    const char *& annotation();
        // Return a reference to the modifiable annotation of this enumerator
        // info object.

    const char *& name();
        // Return a reference to the modifiable name of this enumerator info
        // object.

    int& nameLength();
        // Return a reference to the modifiable length of the name of this
        // enumerator info object.  Note that the 0-terminator is not included
        // in the length.

    int& value();
        // Return a reference to the modifiable id of this enumerator info
        // object.

    // ACCESSORS
    const char *annotation() const;
        // Return the non-modifiable annotation of this enumerator info object.

    const char *name() const;
        // Return the non-modifiable name of this enumerator info object.

    int nameLength() const;
        // Return the length of the name of this enumerator info object.  Note
        // that the 0-terminator is not included in the length.

    int value() const;
        // Return the index of this enumerator info object.
};

// FREE OPERATORS
inline
bool operator==(const bdlat_EnumeratorInfo& lhs,
                const bdlat_EnumeratorInfo& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' enumerator info objects
    // have the same value, and 'false' otherwise.  Two enumerator info objects
    // have the same value if each of their respective properties are
    // identical.

inline
bool operator!=(const bdlat_EnumeratorInfo& lhs,
                const bdlat_EnumeratorInfo& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' enumerator info objects
    // do not have the same value, and 'false' otherwise.  Two enumerator info
    // objects do not have the same value if at least one of their respective
    // properties is not identical.

bsl::ostream& operator<<(bsl::ostream&               stream,
                         const bdlat_EnumeratorInfo& enumeratorInfo);
    // Write the value of the specified 'enumeratorInfo' to the specified
    // 'stream'.

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

// MANIPULATORS

inline
const char *& bdlat_EnumeratorInfo::annotation()
{
    return d_annotation_p;
}

inline
const char *& bdlat_EnumeratorInfo::name()
{
    return d_name_p;
}

inline
int& bdlat_EnumeratorInfo::nameLength()
{
    return d_nameLength;
}

inline
int& bdlat_EnumeratorInfo::value()
{
    return d_value;
}

// ACCESSORS

inline
const char *bdlat_EnumeratorInfo::annotation() const
{
    return d_annotation_p;
}

inline
const char *bdlat_EnumeratorInfo::name() const
{
    return d_name_p;
}

inline
int bdlat_EnumeratorInfo::nameLength() const
{
    return d_nameLength;
}

inline
int bdlat_EnumeratorInfo::value() const
{
    return d_value;
}

// FREE OPERATORS

inline
bool operator==(const bdlat_EnumeratorInfo& lhs,
                const bdlat_EnumeratorInfo& rhs)
{
    return lhs.value()      == rhs.value()
        && lhs.nameLength() == rhs.nameLength()
        && 0 == bsl::memcmp(lhs.name(), rhs.name(), lhs.nameLength())
        && 0 == bsl::strcmp(lhs.annotation(), rhs.annotation());
}

inline
bool operator!=(const bdlat_EnumeratorInfo& lhs,
                const bdlat_EnumeratorInfo& rhs)
{
    return !(lhs == rhs);
}

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2005
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ----------------------------------
