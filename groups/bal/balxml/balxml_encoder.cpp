// balxml_encoder.cpp              -*-C++-*-
#include <balxml_encoder.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(balxml_encoder_cpp,"$Id$ $CSID$")

#ifdef TEST
#include <balxml_decoder.h>     // for testing only
#include <balxml_minireader.h>  // for testing only
#endif

#include <bdlat_formattingmode.h>

#include <bsl_string.h>
#include <bsl_vector.h>

namespace BloombergLP {

                   // -------------------------------------
                   // class baexml::BerEncoder::MemOutStream
                   // -------------------------------------

balxml::Encoder::MemOutStream::~MemOutStream()
{
}

namespace balxml {                       // --------------------
                       // class Encoder
                       // --------------------

Encoder::Encoder(
                   const EncoderOptions *options,
                   bslma::Allocator            *basicAllocator)
: d_options       (options)
, d_allocator     (bslma::Default::allocator(basicAllocator))
, d_logStream     (0)
, d_severity      (ErrorInfo::BAEXML_NO_ERROR)
, d_errorStream   (0)
, d_warningStream (0)
{
}

Encoder::Encoder(
                   const EncoderOptions *options,
                   bsl::ostream                *errorStream,
                   bsl::ostream                *warningStream,
                   bslma::Allocator            *basicAllocator)
: d_options       (options)
, d_allocator     (bslma::Default::allocator(basicAllocator))
, d_logStream     (0)
, d_severity      (ErrorInfo::BAEXML_NO_ERROR)
, d_errorStream   (errorStream)
, d_warningStream (warningStream)
{
}

Encoder::~Encoder()
{
    if (d_logStream != 0) {
        d_logStream->~MemOutStream();
    }
}

ErrorInfo::Severity  Encoder::logError(
                         const char             *text,
                         const bslstl::StringRef&  tag,
                         int                     formattingMode,
                         int                     index)
{
    if ((int) d_severity < (int) ErrorInfo::BAEXML_ERROR) {
        d_severity = ErrorInfo::BAEXML_ERROR;
    }

    bsl::ostream& out = logStream();

    out << text << ':';

    if (index >= 0) {
        out << " index=" << index;
    }

    out << " tag=" << tag
        << " formattingMode=" << formattingMode
        << bsl::endl;

    return d_severity;
}

                       // ----------------------------
                       // class Encoder_Context
                       // ----------------------------

Encoder_Context::Encoder_Context(
                                Formatter *formatter,
                                Encoder   *encoder)
: d_formatter(formatter)
, d_encoder(encoder)
{
}

                       // ------------------------------
                       // class Encoder_EncodeObject
                       // ------------------------------

int Encoder_EncodeObject::executeImp(
                                      const bsl::vector<char>&  object,
                                      const bslstl::StringRef&    tag,
                                      int                       formattingMode,
                                      bdeat_TypeCategory::Array)
{
    if (formattingMode & bdeat_FormattingMode::BDEAT_LIST) {
        return executeArrayListImp(object, tag);
    }

    switch (formattingMode & bdeat_FormattingMode::BDEAT_TYPE_MASK) {
      case bdeat_FormattingMode::BDEAT_BASE64:
      case bdeat_FormattingMode::BDEAT_TEXT:
      case bdeat_FormattingMode::BDEAT_HEX: {
        d_context_p->openElement(tag);

        TypesPrintUtil::print(d_context_p->rawOutputStream(),
                                     object,
                                     formattingMode,
                                     &d_context_p->encoderOptions());

        d_context_p->closeElement(tag);

        int ret = d_context_p->status();
        if (ret) {

            d_context_p->logError("Failed to encode",
                                 tag,
                                 formattingMode);
        }

        return ret;
      }
      default: {
        return executeArrayRepetitionImp(object, tag, formattingMode);
      }
    }
}
}  // close package namespace

}  // close namespace BloombergLP

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2007
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
