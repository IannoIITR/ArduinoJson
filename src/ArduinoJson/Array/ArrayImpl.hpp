// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Object/ObjectRef.hpp"
#include "ArrayRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

inline ArrayRef ArrayRef::createNestedArray() const {
  return add().to<ArrayRef>();
}

template <typename TImpl>
inline ObjectRef ArrayShortcuts<TImpl>::createNestedObject() const {
  return impl()->template add().template to<ObjectRef>();
}
}  // namespace ARDUINOJSON_NAMESPACE
