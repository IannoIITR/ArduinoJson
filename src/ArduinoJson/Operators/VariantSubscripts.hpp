// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Array/ArrayShortcuts.hpp"
#include "../Object/ObjectShortcuts.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TImpl>
class VariantSubscripts : public ObjectShortcuts<TImpl>,
                          public ArrayShortcuts<TImpl> {
 public:
  using ObjectShortcuts<TImpl>::operator[];
  using ObjectShortcuts<TImpl>::createNestedObject;
  using ArrayShortcuts<TImpl>::operator[];
  using ArrayShortcuts<TImpl>::createNestedObject;
};
}  // namespace ARDUINOJSON_NAMESPACE
