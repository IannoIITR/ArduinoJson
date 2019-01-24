// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "MemberProxy.hpp"
#include "VariantRef.hpp"

#include <string.h>  // for strcmp

namespace ARDUINOJSON_NAMESPACE {

template <typename T>
inline T VariantData::asIntegral() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
      return T(_content.asInteger);
    case VALUE_IS_NEGATIVE_INTEGER:
      return T(~_content.asInteger + 1);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseInteger<T>(_content.asString);
    case VALUE_IS_FLOAT:
      return T(_content.asFloat);
    default:
      return 0;
  }
}

// T = float/double
template <typename T>
inline T VariantData::asFloat() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
      return static_cast<T>(_content.asInteger);
    case VALUE_IS_NEGATIVE_INTEGER:
      return -static_cast<T>(_content.asInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseFloat<T>(_content.asString);
    case VALUE_IS_FLOAT:
      return static_cast<T>(_content.asFloat);
    default:
      return 0;
  }
}

inline const char *VariantData::asString() const {
  switch (type()) {
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return _content.asString;
    default:
      return 0;
  }
}

inline bool VariantRef::set(ArrayRef array) const {
  return to<ArrayRef>().copyFrom(array);
}

inline bool VariantRef::set(ArrayConstRef array) const {
  return to<ArrayRef>().copyFrom(array);
}

inline bool VariantRef::set(const ElementProxy<ArrayRef> &value) const {
  return set(value.as<VariantRef>());
}

inline bool VariantRef::set(ObjectRef object) const {
  return to<ObjectRef>().copyFrom(object);
}

inline bool VariantRef::set(ObjectConstRef object) const {
  return to<ObjectRef>().copyFrom(object);
}

template <typename TString>
inline bool VariantRef::set(
    const MemberProxy<ObjectRef, TString> &value) const {
  return set(value.template as<VariantRef>());
}

inline bool VariantRef::set(VariantConstRef value) const {
  return variantCopyFrom(_data, value._data, _pool);
}

inline bool VariantRef::set(VariantRef value) const {
  return variantCopyFrom(_data, value._data, _pool);
}

template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, T>::type VariantRef::as()
    const {
  return ArrayRef(_pool, _data != 0 ? _data->asArray() : 0);
}

template <typename T>
inline typename enable_if<is_same<T, ObjectRef>::value, T>::type
VariantRef::as() const {
  return ObjectRef(_pool, variantAsObject(_data));
}

template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type
VariantRef::to() const {
  return ArrayRef(_pool, variantToArray(_data));
}

template <typename T>
typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type
VariantRef::to() const {
  return ObjectRef(_pool, variantToObject(_data));
}

template <typename T>
typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type
VariantRef::to() const {
  variantSetNull(_data);
  return *this;
}

inline VariantConstRef VariantConstRef::operator[](size_t index) const {
  return ArrayConstRef(_data != 0 ? _data->asArray() : 0)[index];
}

inline VariantRef VariantRef::get(size_t index) const {
  return VariantRef(_pool, _data != 0 ? _data->get(index) : 0);
}

template <typename TKey>
VariantRef VariantRef::get(TKey *key) const {
  return VariantRef(_pool, _data != 0 ? _data->get(wrapString(key)) : 0);
}

template <typename TKey>
VariantRef VariantRef::get(const TKey &key) const {
  return VariantRef(_pool, _data != 0 ? _data->get(wrapString(key)) : 0);
}

template <typename TKey>
NO_INLINE VariantData *variantGetOrCreate(VariantData *var, TKey *key,
                                          MemoryPool *pool) {
  return var != 0 ? var->getOrCreate(wrapString(key), pool) : 0;
}

template <typename TKey>
NO_INLINE VariantData *variantGetOrCreate(VariantData *var, const TKey &key,
                                          MemoryPool *pool) {
  return var != 0 ? var->getOrCreate(wrapString(key), pool) : 0;
}

template <typename TKey>
VariantRef VariantRef::getOrCreate(TKey *key) const {
  return VariantRef(_pool, variantGetOrCreate(_data, key, _pool));
}

template <typename TKey>
VariantRef VariantRef::getOrCreate(const TKey &key) const {
  return VariantRef(_pool, variantGetOrCreate(_data, key, _pool));
}

}  // namespace ARDUINOJSON_NAMESPACE
