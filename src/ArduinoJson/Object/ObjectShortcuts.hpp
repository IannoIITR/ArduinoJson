// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/attributes.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Strings/StringWrappers.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename TParent, typename TKey>
class MemberProxy;

template <typename TImpl>
class ObjectShortcuts {
 public:
  // MemberProxy operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey>::value,
                                  MemberProxy<TImpl, const TKey &> >::type
  operator[](const TKey &key) const;
  //
  // MemberProxy operator[](TKey) const;
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TKey>
  FORCE_INLINE typename enable_if<IsString<TKey *>::value,
                                  MemberProxy<TImpl, TKey *> >::type
  operator[](TKey *key) const;

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
