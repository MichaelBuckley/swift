//===--- Unicode.h - Unicode utilities --------------------------*- C++ -*-===//
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

#ifndef SWIFT_BASIC_UNICODE_H
#define SWIFT_BASIC_UNICODE_H

#include "swift/Basic/LLVM.h"
#include "llvm/ADT/StringRef.h"

#include "unicode/brkiter.h"

namespace swift {
namespace unicode {

static inline bool isSingleExtendedGraphemeCluster(StringRef S) {
  UErrorCode status = U_ZERO_ERROR;
  BreakIterator *bi = BreakIterator::createCharacterInstance(Locale("root"), status);
  UnicodeString us = UnicodeString(S.data(), S.size());

  bi->setText(us);

  return bi->first() == BreakIterator::DONE;
}

enum class GraphemeClusterBreakProperty : uint8_t {
  Other,
  CR,
  LF,
  Control,
  Extend,
  Regional_Indicator,
  Prepend,
  SpacingMark,
  L,
  V,
  T,
  LV,
  LVT,
};

/// Extended grapheme cluster boundary rules, represented as a matrix.  Indexed
/// by first code point, then by second code point in least-significant-bit
/// order.  A set bit means that a boundary is prohibited between two code
/// points.
extern const uint16_t ExtendedGraphemeClusterNoBoundaryRulesMatrix[];

/// Returns the value of the Grapheme_Cluster_Break property for a given code
/// point.
GraphemeClusterBreakProperty getGraphemeClusterBreakProperty(uint32_t C);

bool isSingleUnicodeScalar(StringRef S);

unsigned extractFirstUnicodeScalar(StringRef S);

/// Get the length of the UTF8 string transcoded into UTF16.
/// Returns the number of code units in UTF16 representation
uint64_t getUTF16Length(StringRef Str);

} // namespace unicode
} // namespace swift

#endif

