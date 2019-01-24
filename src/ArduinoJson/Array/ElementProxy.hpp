// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Operators/VariantOperators.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename TArray>
class ElementProxy : public VariantOperators<ElementProxy<TArray> >,
                     public Visitable {
  typedef ElementProxy<TArray> this_type;

 public:
  FORCE_INLINE ElementProxy(TArray array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE this_type& operator=(const this_type& src) {
    get_impl().set(src.as<VariantConstRef>());
    return *this;
  }

  // Replaces the value
  //
  // operator=(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename T>
  FORCE_INLINE this_type& operator=(const T& src) {
    get_impl().set(src);
    return *this;
  }
  //
  // operator=(TValue)
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename T>
  FORCE_INLINE this_type& operator=(T* src) {
    get_impl().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return get_impl().isNull();
  }

  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    return get_impl().as<T>();
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return get_impl().is<T>();
  }

  template <typename T>
  FORCE_INLINE typename VariantTo<T>::type to() const {
    return get_impl().to<T>();
  }

  // Replaces the value
  //
  // bool set(const TValue&)
  // TValue = bool, long, int, short, float, double, serialized, VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) const {
    return get_impl().set(value);
  }
  //
  // bool set(TValue)
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(TValue* value) const {
    return get_impl().set(value);
  }

  template <typename Visitor>
  void accept(Visitor& visitor) const {
    return get_impl().accept(visitor);
  }

  FORCE_INLINE size_t size() const {
    return get_impl().size();
  }

  template <typename TNestedKey>
  VariantRef get(TNestedKey* key) const {
    return get_impl().get(key);
  }

  template <typename TNestedKey>
  VariantRef get(const TNestedKey& key) const {
    return get_impl().get(key);
  }

  template <typename TNestedKey>
  VariantRef getOrCreate(TNestedKey* key) const {
    return get_impl().getOrCreate(key);
  }

  template <typename TNestedKey>
  VariantRef getOrCreate(const TNestedKey& key) const {
    return get_impl().getOrCreate(key);
  }

 private:
  FORCE_INLINE VariantRef get_impl() const {
    return _array.get(_index);
  }

  TArray _array;
  const size_t _index;
};

template <typename TImpl>
inline ElementProxy<TImpl> ArrayShortcuts<TImpl>::operator[](
    size_t index) const {
  return ElementProxy<TImpl>(*impl(), index);
}

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
