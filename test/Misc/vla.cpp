// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wvla-extension"
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvla"
#else
#define VLA_NOT_SUPPORTED
#endif

#ifndef VLA_NOT_SUPPORTED

TEST_CASE("Variable Length Array") {
  SECTION("JsonBuffer_strdup") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    const char* dup = jsonBuffer.strdup(vla);

    REQUIRE(static_cast<const void*>(vla) != static_cast<const void*>(dup));
    REQUIRE(std::string("world") == dup);
  }

  SECTION("parseJson()") {
    SECTION("parseJson(JsonArray&, char[n])") {
      int i = 8;
      char vla[i];
      strcpy(vla, "[42]");

      StaticJsonArray<JSON_ARRAY_SIZE(1)> arr;
      REQUIRE(parseJson(arr, vla));
    }

    SECTION("parseJson(JsonObject&, char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "{\"a\":42}");

      StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
      REQUIRE(parseJson(obj, vla));
    }

    SECTION("parseJson(JsonVariant&)") {
      int i = 16;
      char vla[i];
      strcpy(vla, "42");

      DynamicJsonVariant variant;
      parseJson(variant, vla);

      REQUIRE(42 == variant.as<int>());
    }
  }

  SECTION("JsonVariant") {
    SECTION("JsonVariant(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "42");

      DynamicJsonVariant variant(vla);

      REQUIRE(42 == variant.as<int>());
    }

    SECTION("operator=(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "42");

      DynamicJsonVariant variant(666);
      variant = vla;

      REQUIRE(42 == variant.as<int>());
    }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[](char[n])") {
      DynamicJsonObject object;
      object["hello"] = "world";
      JsonVariant variant = object;

      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      REQUIRE(std::string("world") == variant[vla]);
    }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[](char[n]) const") {
      DynamicJsonObject object;
      object["hello"] = "world";
      JsonVariant variant = object;

      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      REQUIRE(std::string("world") == variant[vla]);
    }
#endif

    SECTION("operator==(char[n]) const") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      const JsonVariant variant = "hello";

      REQUIRE((vla == variant));
      REQUIRE((variant == vla));
      REQUIRE_FALSE((vla != variant));
      REQUIRE_FALSE((variant != vla));
    }

    SECTION("operator!=(char[n]) const") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      const JsonVariant variant = "world";

      REQUIRE((vla != variant));
      REQUIRE((variant != vla));
      REQUIRE_FALSE((vla == variant));
      REQUIRE_FALSE((variant == vla));
    }
  }

  SECTION("JsonObject") {
    DynamicJsonObject obj;

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
    SECTION("operator[](char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj[vla] = "world";

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("operator[](char[n]) const") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj[vla] = "world";
      const JsonObject& cobj = obj;

      REQUIRE(std::string("world") == cobj[vla]);
    }
#endif

    SECTION("get(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj["hello"] = "world";

      REQUIRE(std::string("world") == obj.get<char*>(vla));
    }

    SECTION("set(char[n], const char*)") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj.set(vla, "world");

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(const char*, char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      obj.set("hello", vla);

      REQUIRE(std::string("world") == obj["hello"]);
    }

    SECTION("set(char[n], char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      obj.set(vla, vla);

      REQUIRE(std::string("world") == obj["world"]);
    }

    SECTION("containsKey(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      DynamicJsonBuffer jsonBuffer;
      obj["hello"] = "world";
      const JsonObject& cobj = obj;

      REQUIRE(true == cobj.containsKey(vla));
    }

    SECTION("remove(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj["hello"] = "world";
      obj.remove(vla);

      REQUIRE(0 == obj.size());
    }

    SECTION("is<T>(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj["hello"] = 42;

      REQUIRE(true == obj.is<int>(vla));
    }

    SECTION("createNestedArray(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj.createNestedArray(vla);
    }

    SECTION("createNestedObject(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "hello");

      obj.createNestedObject(vla);
    }
  }

  SECTION("JsonObjectSubscript") {
    DynamicJsonObject obj;

    SECTION("operator=(char[n])") {  // issue #416
      int i = 32;
      char vla[i];
      strcpy(vla, "world");

      obj["hello"] = vla;

      REQUIRE(std::string("world") == obj["hello"].as<char*>());
    }

    SECTION("set(char[n])") {
      int i = 32;
      char vla[i];
      strcpy(vla, "world");

      obj["hello"].set(vla);

      REQUIRE(std::string("world") == obj["hello"].as<char*>());
    }
  }

  SECTION("JsonArray") {
    DynamicJsonArray arr;

    SECTION("add(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      arr.add(vla);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("set(size_t, char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      arr.add("hello");
      arr.set(0, vla);

      REQUIRE(std::string("world") == arr[0]);
    }
  }

  SECTION("JsonArraySubscript") {
    DynamicJsonArray arr;

    SECTION("set(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      arr.add("hello");
      arr[0].set(vla);

      REQUIRE(std::string("world") == arr[0]);
    }

    SECTION("operator=(char[n])") {
      int i = 16;
      char vla[i];
      strcpy(vla, "world");

      arr.add("hello");
      arr[0] = vla;

      REQUIRE(std::string("world") == arr[0]);
    }
  }
}
#endif
