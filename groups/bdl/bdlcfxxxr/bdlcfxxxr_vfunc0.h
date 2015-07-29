// bdlcfxxxr_vfunc0.h               -*-C++-*-
#ifndef INCLUDED_BDLCFXXXR_VFUNC0
#define INCLUDED_BDLCFXXXR_VFUNC0

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")


//@PURPOSE: Provide a common reference-counted base class representation.
//
//@CLASSES:
//   bdlcfxxxr::Vfunc0: thread-safe reference-counted base class with an allocator
//
//@AUTHOR: John Lakos (jlakos)
//
//@DEPRECATED: This component should not be used in new code, and will be
// deleted in the near future.  Please see 'bdlf_function', 'bdlf_bind', etc.
// for alternatives that should be used for all future development.
//
//@DESCRIPTION: This component defines the common (partially implemented) base
// class for all internal representations of the 'bdlcfxxx::Vfunc0' family of
// function objects (functors).  This abstract base class declares the pure
// virtual 'execute' method, whose signature characterizes this family of
// functor representations, while exploiting structural inheritance to
// implement efficient (inline) count manipulators.  The count is intended to
// reflect the number of 'bdlcfxxx::Vfunc0' objects (envelopes) or other partial
// owners that are currently using this functor representation (letter) and is
// manipulated by each owner accordingly.  The counter used by this component
// is atomic therefore providing thread-safe increment and decrement
// operations.  The class also provides a static 'deleteObject' method to allow
// clients to destroy the object (when the count reaches '0') without any
// information about the details of the 'bdlcfxxxr::Vfunc0' object memory management
// scheme.  Note that the object must be allocated dynamically using the same
// allocator supplied at construction and that the allocator must remain valid
// through the life of the object.  'deleteObject' method allows concrete
// classes derived from 'bdlcfxxxr::Vfunc0' to declare destructor 'private' and
// limit an object instantiation to the heap.
//
///USAGE
///-----
// This example demonstrates the essential functionality of the common
// base-class representation.  We will need two global counters for this
// demonstration.
//..
//   static int executeUsageCounter = 0;
//   static int dtorUsageCounter = 0;
//..
// Instantiate a 'ConcreteDerivedClass' class derived from 'bdlcfxxxr::Vfunc0'.
//..
//   class ConcreteDerivedClass : public bdlcfxxxr::Vfunc0 {
//     public:
//       ConcreteDerivedClass(bslma::Allocator *basicAllocator)
//       : bdlcfxxxr::Vfunc0(basicAllocator) { }
//
//       virtual void execute() const
//           // Increment global counter 'testCounter'.
//       {
//           ++executeUsageCounter;
//       }
//
//     private:
//       virtual ~ConcreteDerivedClass()
//           // Destroy the class instance.  Increment a global
//           // 'dtorUsageCounter'.
//       {
//           ++dtorUsageCounter;
//       }
//   };
//..
// Create an envelope class 'EnvelopeClass' that is using
// bdlcfxxxr::Vfunc0
//..
//   class EnvelopeClass {
//       // Provide an object that encapsulates a 'bdlcfxxxr::Vfunc0' object.
//
//       bdlcfxxxr::Vfunc0 *d_rep_p;
//           // polymorphic functor representation
//
//     public:
//       // CREATORS
//       EnvelopeClass(bdlcfxxxr::Vfunc0 *rep) : d_rep_p(rep)
//           // Create a functor that assumes shared ownership of the
//           // specified, dynamically allocated, reference-counted
//           // representation.
//       {
//           if (d_rep_p) {
//               d_rep_p->increment();
//           }
//       }
//
//       ~EnvelopeClass()
//           // Decrement the reference count of that internal representation
//           // object, and, if the count is now 0, destroy and deallocate the
//           // representation using 'deleteObject' method of 'bdlcfxxxr::Vfunc0'
//           // class.
//       {
//           if (0 == d_rep_p->decrement()) {
//               bdlcfxxxr::Vfunc0::deleteObject(d_rep_p);
//           }
//       }
//
//
//       // ACCESSORS
//       void operator()() const
//           // Execute this functor.
//       {
//           d_rep_p->execute();
//       }
//   };
//..
// Then in the body of the program:
//..
//   executeUsageCounter = 0;
//   dtorUsageCounter = 0;
//
//   typedef ConcreteDerivedClass DerivedObj;
//   typedef bdlcfxxxr::Vfunc0 Obj;
//   bslma::Allocator *myAllocator = bslma::Default::defaultAllocator();
//
//   Obj *x = new(*myAllocator) DerivedObj(myAllocator);
//   {
//       // The reference counter is 0
//       EnvelopeClass env0(x);
//       // The reference counter is 1
//       env0();        ASSERT(1 == executeUsageCounter);
//       {
//           EnvelopeClass env1(x);
//           // The reference counter is 2
//           env1();    ASSERT(2 == executeUsageCounter);
//       }
//       // The reference counter is 1
//       ASSERT(0 == dtorUsageCounter);
//   }
//   ASSERT(1 == dtorUsageCounter);
//..

#ifndef INCLUDED_BDLSCM_VERSION
#include <bdlscm_version.h>
#endif

#ifndef INCLUDED_BSLMA_ALLOCATOR
#include <bslma_allocator.h>
#endif

#ifndef INCLUDED_BDLQQ_XXXATOMICUTIL
#include <bdlqq_xxxatomicutil.h>
#endif

namespace BloombergLP {

namespace bdlcfxxxr {
                        // ==================
                        // class Vfunc0
                        // ==================

class Vfunc0 {
    // Common (partially-implemented) abstract base class declaring the
    // characteristic pure virtual 'execute' method and exploiting structural
    // inheritance to achieve efficient (inline) count manipulation.
    // This class also implements a 'deleteObject' class method.  This method
    // facilitates the use of 'Vfunc0' as a reference-counted letter
    // class in the envelope-letter pattern.  'deleteObject' is used to to
    // destroy and deallocate the concrete objects derived from 'Vfunc0',
    // which allows the derived classes to declare the destructor 'private',
    // and limit an object instantiation to the heap.

    bdlqq::AtomicUtil::Int d_count;    // dumb data (number of active references)
    bslma::Allocator *d_allocator_p; // holds (but doesn't own) memory
                                     // allocator

  private:
    Vfunc0(const Vfunc0&);                  // not implemented
    Vfunc0& operator=(const Vfunc0&);       // not implemented

  protected:
    virtual ~Vfunc0();
        // The destructor is declared 'protected' to allow derivation from this
        // class and to disallow direct deletion of the derived concrete
        // object.  Clients must use the static ("class") 'deleteObject'
        // method to destroy and deallocate the object from its base class
        // pointer.

  public:
    // CLASS METHODS
    static void deleteObject(Vfunc0 *object);
        // Destroy the specified 'object' and use the memory allocator held by
        // 'object' to deallocate it.  The behaviour is undefined unless the
        // specified 'object' holds a valid memory allocator.

    // CREATORS
    Vfunc0(bslma::Allocator *basicAllocator);
        // Create the base portion of a functor object, with the initial
        // reference count set to 0.  Return the specified 'basicAllocator' to
        // deallocate memory when 'destroyObject' is invoked.

    // MANIPULATORS
    void increment();
        // Increase the reference count of this base representation by 1.

    int decrement();
        // Decrease the reference count of this base representation by 1 and
        // return its current value.

    // ACCESSORS
    virtual void execute() const = 0;
        // Invoke the client-supplied callback function.
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

// PROTECTED CREATORS
inline Vfunc0::~Vfunc0()
{
}

// CLASS METHODS
inline void Vfunc0::deleteObject(Vfunc0 *object)
{
    object->~Vfunc0();
    object->d_allocator_p->deallocate(object);
}

// CREATORS
inline Vfunc0::Vfunc0(bslma::Allocator *basicAllocator)
: d_allocator_p(basicAllocator)
{
    bdlqq::AtomicUtil::initInt(&d_count,0);
}

// MANIPULATORS
inline void Vfunc0::increment()
{
    bdlqq::AtomicUtil::incrementInt(&d_count);
}

inline int Vfunc0::decrement()
{
    return bdlqq::AtomicUtil::decrementIntNv(&d_count);
}
}  // close package namespace

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2002
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
