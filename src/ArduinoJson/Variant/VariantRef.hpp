// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "../Memory/MemoryPool.hpp"
#include "../Misc/Visitable.hpp"
#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "../Operators/VariantOperators.hpp"
#include "../Polyfills/type_traits.hpp"
#include "VariantAs.hpp"
#include "VariantFunctions.hpp"
#include "VariantRef.hpp"

namespace ARDUINOJSON_NAMESPACE {

// Forward declarations.
class ArrayRef;
class ObjectRef;

template <typename, typename>
class MemberProxy;

// Contains the methods shared by VariantRef and VariantConstRef
template <typename TData>
class VariantRefBase {
 public:
  // Tells wether the variant has the specified type.
  // Returns true if the variant has type type T, false otherwise.
  //
  // bool is<char>() const;
  // bool is<signed char>() const;
  // bool is<signed short>() const;
  // bool is<signed int>() const;
  // bool is<signed long>() const;
  // bool is<unsigned char>() const;
  // bool is<unsigned short>() const;
  // bool is<unsigned int>() const;
  // bool is<unsigned long>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_integral<T>::value, bool>::type is()
      const {
    return variantIsInteger(_data);
  }
  //
  // bool is<double>() const;
  // bool is<float>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_floating_point<T>::value, bool>::type is()
      const {
    return variantIsFloat(_data);
  }
  //
  // bool is<bool>() const
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, bool>::value, bool>::type is()
      const {
    return variantIsBoolean(_data);
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  // bool is<std::string>() const;
  // bool is<String>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, const char *>::value ||
                                      is_same<T, char *>::value ||
                                      IsWriteableString<T>::value,
                                  bool>::type
  is() const {
    return variantIsString(_data);
  }
  //
  // bool is<ArrayRef> const;
  // bool is<const ArrayRef> const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, ArrayRef>::value, bool>::type
  is() const {
    return variantIsArray(_data);
  }
  //
  // bool is<ObjectRef> const;
  // bool is<const ObjectRef> const;
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, ObjectRef>::value, bool>::type
  is() const {
    return variantIsObject(_data);
  }

  FORCE_INLINE bool isNull() const {
    return variantIsNull(_data);
  }

  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }

  FORCE_INLINE size_t nesting() const {
    return _data ? _data->nesting() : 0;
  }

  size_t size() const {
    return variantSize(_data);
  }

 protected:
  VariantRefBase(TData *data) : _data(data) {}
  TData *_data;
};

// A variant that can be a any value serializable to a JSON value.
//
// It can be set to:
// - a boolean
// - a char, short, int or a long (signed or unsigned)
// - a string (const char*)
// - a reference to a ArrayRef or ObjectRef
class VariantRef : public VariantRefBase<VariantData>,
                   public VariantOperators<VariantRef>,
                   public Visitable {
  typedef VariantRefBase<VariantData> base_type;
  friend class VariantConstRef;

 public:
  // Intenal use only
  FORCE_INLINE VariantRef(MemoryPool *pool, VariantData *data)
      : base_type(data), _pool(pool) {}

  // Creates an uninitialized VariantRef
  FORCE_INLINE VariantRef() : base_type(0), _pool(0) {}

  // set(bool value)
  FORCE_INLINE bool set(bool value) const {
    return variantSetBoolean(_data, value);
  }

  // set(double value);
  // set(float value);
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_floating_point<T>::value>::type * = 0) const {
    return variantSetFloat(_data, static_cast<Float>(value));
  }

  // set(char)
  // set(signed short)
  // set(signed int)
  // set(signed long)
  // set(signed char)
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_integral<T>::value && is_signed<T>::value>::type * =
          0) const {
    return variantSetSignedInteger(_data, value);
  }

  // set(unsigned short)
  // set(unsigned int)
  // set(unsigned long)
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_integral<T>::value &&
                                  is_unsigned<T>::value>::type * = 0) const {
    return variantSetUnsignedInteger(_data, static_cast<UInt>(value));
  }

  // set(SerializedValue<const char *>)
  FORCE_INLINE bool set(SerializedValue<const char *> value) const {
    return variantSetLinkedRaw(_data, value);
  }

  // set(SerializedValue<std::string>)
  // set(SerializedValue<String>)
  // set(SerializedValue<const __FlashStringHelper*>)
  template <typename T>
  FORCE_INLINE bool set(
      SerializedValue<T> value,
      typename enable_if<!is_same<const char *, T>::value>::type * = 0) const {
    return variantSetOwnedRaw(_data, value, _pool);
  }

  // set(const std::string&)
  // set(const String&)
  template <typename T>
  FORCE_INLINE bool set(
      const T &value,
      typename enable_if<IsString<T>::value>::type * = 0) const {
    return variantSetOwnedString(_data, wrapString(value), _pool);
  }

  // set(char*)
  // set(const __FlashStringHelper*)
  template <typename T>
  FORCE_INLINE bool set(
      T *value, typename enable_if<IsString<T *>::value>::type * = 0) const {
    return variantSetOwnedString(_data, wrapString(value), _pool);
  }

  // set(const char*);
  FORCE_INLINE bool set(const char *value) const {
    return variantSetLinkedString(_data, value);
  }

  bool set(VariantConstRef value) const;
  bool set(VariantRef value) const;

  FORCE_INLINE bool set(ArrayRef array) const;
  FORCE_INLINE bool set(ArrayConstRef array) const;
  FORCE_INLINE bool set(const ElementProxy<ArrayRef> &) const;
  FORCE_INLINE bool set(ObjectRef object) const;
  FORCE_INLINE bool set(ObjectConstRef object) const;
  template <typename TString>
  FORCE_INLINE bool set(const MemberProxy<ObjectRef, TString> &) const;

  // Get the variant as the specified type.
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<!is_same<T, ArrayRef>::value &&
                                      !is_same<T, ObjectRef>::value &&
                                      !is_same<T, VariantRef>::value,
                                  typename VariantAs<T>::type>::type
  as() const {
    return variantAs<T>(_data);
  }
  //
  // ArrayRef as<ArrayRef>() const;
  // const ArrayRef as<const ArrayRef>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, ArrayRef>::value, T>::type as()
      const;
  //
  // ObjectRef as<ObjectRef>() const;
  // const ObjectRef as<const ObjectRef>() const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, ObjectRef>::value, T>::type as()
      const;
  //
  // VariantRef as<VariantRef> const;
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, VariantRef>::value, T>::type as()
      const {
    return *this;
  }

  template <typename Visitor>
  void accept(Visitor &visitor) const {
    variantAccept(_data, visitor);
  }

  FORCE_INLINE bool operator==(VariantRef lhs) const {
    return variantEquals(_data, lhs._data);
  }

  FORCE_INLINE bool operator!=(VariantRef lhs) const {
    return !variantEquals(_data, lhs._data);
  }

  // Change the type of the variant
  //
  // ArrayRef to<ArrayRef>()
  template <typename T>
  typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type to() const;
  //
  // ObjectRef to<ObjectRef>()
  template <typename T>
  typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type to() const;
  //
  // ObjectRef to<VariantRef>()
  template <typename T>
  typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type to()
      const;

  VariantRef get(size_t) const;

  template <typename TKey>
  VariantRef get(TKey *) const;

  template <typename TKey>
  VariantRef get(const TKey &) const;

  template <typename TKey>
  VariantRef getOrCreate(TKey *) const;

  template <typename TKey>
  VariantRef getOrCreate(const TKey &) const;

 private:
  MemoryPool *_pool;
};

class VariantConstRef : public VariantRefBase<const VariantData>,
                        public VariantOperators<VariantConstRef>,
                        public Visitable {
  typedef VariantRefBase<const VariantData> base_type;
  friend class VariantRef;

 public:
  VariantConstRef() : base_type(0) {}
  VariantConstRef(const VariantData *data) : base_type(data) {}
  VariantConstRef(VariantRef var) : base_type(var._data) {}

  template <typename Visitor>
  void accept(Visitor &visitor) const {
    variantAccept(_data, visitor);
  }

  // Get the variant as the specified type.
  //
  template <typename T>
  FORCE_INLINE typename VariantConstAs<T>::type as() const {
    return variantAs<typename VariantConstAs<T>::type>(_data);
  }

  FORCE_INLINE VariantConstRef operator[](size_t index) const;

  //
  // const VariantConstRef operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString &key) const {
    return VariantConstRef(objectGet(variantAsObject(_data), wrapString(key)));
  }
  //
  // VariantConstRef operator[](TKey);
  // TKey = const char*, const char[N], const __FlashStringHelper*
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString *>::value, VariantConstRef>::type
      operator[](TString *key) const {
    const CollectionData *obj = variantAsObject(_data);
    return VariantConstRef(obj ? obj->get(wrapString(key)) : 0);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
