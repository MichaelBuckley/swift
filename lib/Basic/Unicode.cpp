//===--- Unicode.cpp - Unicode utilities ----------------------------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#include "swift/Basic/Unicode.h"
#include "llvm/Support/ConvertUTF.h"

using namespace swift;

static bool extractFirstUnicodeScalarImpl(StringRef S, unsigned &Scalar) {
  if (S.empty())
    return false;

  const UTF8 *SourceStart = reinterpret_cast<const UTF8 *>(S.data());

  const UTF8 *SourceNext = SourceStart;
  UTF32 C;
  UTF32 *TargetStart = &C;

  ConvertUTF8toUTF32(&SourceNext, SourceStart + S.size(), &TargetStart,
                     TargetStart + 1, lenientConversion);
  if (TargetStart == &C) {
    // The source string contains an ill-formed subsequence at the end.
    return false;
  }

  Scalar = C;
  return size_t(SourceNext - SourceStart) == S.size();
}

bool swift::unicode::isSingleUnicodeScalar(StringRef S) {
  unsigned Scalar;
  return extractFirstUnicodeScalarImpl(S, Scalar);
}

unsigned swift::unicode::extractFirstUnicodeScalar(StringRef S) {
  unsigned Scalar;
  bool Result = extractFirstUnicodeScalarImpl(S, Scalar);
  assert(Result && "string does not consist of one Unicode scalar");
  (void)Result;
  return Scalar;
}

uint64_t swift::unicode::getUTF16Length(StringRef Str) {
  uint64_t Length;
  // Transcode the string to UTF-16 to get its length.
  SmallVector<UTF16, 128> buffer(Str.size() + 1); // +1 for ending nulls.
  const UTF8 *fromPtr = (const UTF8 *) Str.data();
  UTF16 *toPtr = &buffer[0];
  ConversionResult Result = ConvertUTF8toUTF16(&fromPtr, fromPtr + Str.size(),
                                               &toPtr, toPtr + Str.size(),
                                               strictConversion);
  assert(Result == conversionOK &&
         "UTF-8 encoded string cannot be converted into UTF-16 encoding");
  (void)Result;

  // The length of the transcoded string in UTF-16 code points.
  Length = toPtr - &buffer[0];
  return Length;
}
