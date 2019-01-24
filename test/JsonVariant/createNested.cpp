// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

static const char* null = 0;

TEST_CASE("JsonVariant::createNestedObject()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("promotes to array") {
    JsonObject obj = variant.createNestedObject();

    REQUIRE(variant.is<JsonArray>() == true);
    REQUIRE(obj.isNull() == false);
  }
}

TEST_CASE("JsonVariant::createNestedArray()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("promotes to array") {
    JsonArray arr = variant.createNestedArray();

    REQUIRE(variant.is<JsonArray>() == true);
    REQUIRE(arr.isNull() == false);
  }
}
