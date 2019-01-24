// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/attributes.hpp"
#include "../Polyfills/type_traits.hpp"

namespace ARDUINOJSON_NAMESPACE {
// Forward declarations.
template <typename>
class ElementProxy;

template <typename TArray>
class ArrayShortcuts {
 public:
  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE ElementProxy<TArray> operator[](size_t index) const;

  FORCE_INLINE ObjectRef createNestedObject() const;

  FORCE_INLINE ArrayRef createNestedArray() const;

 private:
  const TArray *impl() const {
    return static_cast<const TArray *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
