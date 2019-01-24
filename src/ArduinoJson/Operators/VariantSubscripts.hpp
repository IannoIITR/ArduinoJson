// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/attributes.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../Strings/StringWrappers.hpp"
#include "../Variant/VariantAs.hpp"

#include "../Object/ObjectShortcuts.hpp"

namespace ARDUINOJSON_NAMESPACE {
// Forward declarations.
class ArraySubscript;

template <typename TImpl>
class VariantSubscripts : public ObjectShortcuts<TImpl> {
 public:
  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  FORCE_INLINE ArraySubscript operator[](size_t index) const;

  using ObjectShortcuts<TImpl>::operator[];

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
