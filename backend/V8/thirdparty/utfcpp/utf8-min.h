// Copyright 2006-2016 Nemanja Trifunovic

// this file is adapted from https://github.com/nemtrif/utfcpp
// keep only logic related with `utf8::utf16to8`

/*
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <cstdint>
#include <exception>
#include <iterator>

namespace utf8 {

// Helper code - not intended to be directly called by the library users. May be changed at any time
namespace internal {

// Unicode constants
// Leading (high) surrogates: 0xd800 - 0xdbff
// Trailing (low) surrogates: 0xdc00 - 0xdfff
const uint16_t LEAD_SURROGATE_MIN = 0xd800u;
const uint16_t LEAD_SURROGATE_MAX = 0xdbffu;
const uint16_t TRAIL_SURROGATE_MIN = 0xdc00u;
const uint16_t TRAIL_SURROGATE_MAX = 0xdfffu;
const uint32_t SURROGATE_OFFSET =
    0xfca02400u;  // 0x10000u - (LEAD_SURROGATE_MIN << 10) - TRAIL_SURROGATE_MIN

// Maximum valid value for a Unicode code point
const uint32_t CODE_POINT_MAX = 0x0010ffffu;

template <typename u16_type>
inline uint16_t mask16(u16_type oc) {
  return static_cast<uint16_t>(0xffff & oc);
}

template <typename u16>
inline bool is_lead_surrogate(u16 cp) {
  return (cp >= LEAD_SURROGATE_MIN && cp <= LEAD_SURROGATE_MAX);
}

template <typename u16>
inline bool is_trail_surrogate(u16 cp) {
  return (cp >= TRAIL_SURROGATE_MIN && cp <= TRAIL_SURROGATE_MAX);
}

template <typename u16>
inline bool is_surrogate(u16 cp) {
  return (cp >= LEAD_SURROGATE_MIN && cp <= TRAIL_SURROGATE_MAX);
}

template <typename u32>
inline bool is_code_point_valid(u32 cp) {
  return (cp <= CODE_POINT_MAX && !utf8::internal::is_surrogate(cp));
}

}  // namespace internal

/// The library API - functions intended to be called by the users

template <typename octet_iterator>
octet_iterator append(uint32_t cp, octet_iterator result) {
  if (!utf8::internal::is_code_point_valid(cp)) throw std::exception();

  if (cp < 0x80)  // one octet
    *(result++) = static_cast<uint8_t>(cp);
  else if (cp < 0x800) {  // two octets
    *(result++) = static_cast<uint8_t>((cp >> 6) | 0xc0);
    *(result++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
  } else if (cp < 0x10000) {  // three octets
    *(result++) = static_cast<uint8_t>((cp >> 12) | 0xe0);
    *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f) | 0x80);
    *(result++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
  } else {  // four octets
    *(result++) = static_cast<uint8_t>((cp >> 18) | 0xf0);
    *(result++) = static_cast<uint8_t>(((cp >> 12) & 0x3f) | 0x80);
    *(result++) = static_cast<uint8_t>(((cp >> 6) & 0x3f) | 0x80);
    *(result++) = static_cast<uint8_t>((cp & 0x3f) | 0x80);
  }
  return result;
}

template <typename u16bit_iterator, typename octet_iterator>
octet_iterator utf16to8(u16bit_iterator start, u16bit_iterator end, octet_iterator result) {
  while (start != end) {
    uint32_t cp = utf8::internal::mask16(*start++);
    // Take care of surrogate pairs first
    if (utf8::internal::is_lead_surrogate(cp)) {
      if (start != end) {
        uint32_t trail_surrogate = utf8::internal::mask16(*start++);
        if (utf8::internal::is_trail_surrogate(trail_surrogate)) {
          cp = (cp << 10) + trail_surrogate + internal::SURROGATE_OFFSET;
        } else {
          throw std::exception();
        }
      } else {
        throw std::exception();
      }
    }
    // Lone trail surrogate
    else if (utf8::internal::is_trail_surrogate(cp)) {
      throw std::exception();
    }

    result = utf8::append(cp, result);
  }
  return result;
}

}  // namespace utf8
