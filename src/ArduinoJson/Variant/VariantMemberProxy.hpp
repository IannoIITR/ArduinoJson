// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../Operators/VariantOperators.hpp"
#include "../Polyfills/type_traits.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ARDUINOJSON_NAMESPACE {

template <typename TObject, typename TKey>
class VariantMemberProxy
    : public VariantOperators<VariantMemberProxy<TObject, TKey> >,
      public Visitable {
  typedef VariantMemberProxy<TObject, TKey> this_type;

 public:
  FORCE_INLINE VariantMemberProxy(TObject variant, TKey key)
      : _object(variant), _key(key) {}

  operator VariantConstRef() const {
    return get_impl();
  }

  FORCE_INLINE this_type &operator=(const this_type &src) {
    set_impl().set(src);
    return *this;
  }

  // Set the specified value
  //
  // operator=(const TValue&);
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, this_type &>::type
  operator=(const TValue &src) {
    set_impl().set(src);
    return *this;
  }
  //
  // operator=(TValue);
  // TValue = char*, const char*, const __FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE this_type &operator=(TValue *src) {
    set_impl().set(src);
    return *this;
  }

  FORCE_INLINE bool isNull() const {
    return get_impl().isNull();
  }

  template <typename TValue>
  FORCE_INLINE typename VariantAs<TValue>::type as() const {
    return get_impl().template as<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return get_impl().template is<TValue>();
  }

  template <typename TValue>
  FORCE_INLINE typename VariantTo<TValue>::type to() {
    return set_impl().template to<TValue>();
  }

  // Sets the specified value.
  //
  // bool set(const TValue&);
  // TValue = bool, char, long, int, short, float, double, serialized,
  // VariantRef,
  //          std::string, String, ArrayRef, ObjectRef
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, bool>::type set(
      const TValue &value) {
    return set_impl().set(value);
  }
  //
  // bool set(TValue);
  // TValue = char*, const char, const __FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(const TValue *value) {
    return set_impl().set(value);
  }

  template <typename Visitor>
  void accept(Visitor &visitor) const {
    return get_impl().accept(visitor);
  }

 private:
  FORCE_INLINE VariantRef get_impl() const {
    return _object.as<ObjectRef>().get(_key);
  }

  FORCE_INLINE VariantRef set_impl() const {
    return _object.promoteToObject().set(_key);
  }

  TObject _object;
  TKey _key;
};

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          VariantMemberProxy<TImpl, const TString &> >::type
    VariantSubscripts<TImpl>::operator[](const TString &key) const {
  return VariantMemberProxy<TImpl, const TString &>(*impl(), key);
}

template <typename TImpl>
template <typename TString>
inline typename enable_if<IsString<TString *>::value,
                          VariantMemberProxy<TImpl, TString *> >::type
    VariantSubscripts<TImpl>::operator[](TString *key) const {
  return VariantMemberProxy<TImpl, TString *>(*impl(), key);
}

}  // namespace ARDUINOJSON_NAMESPACE

#ifdef _MSC_VER
#pragma warning(pop)
#endif
