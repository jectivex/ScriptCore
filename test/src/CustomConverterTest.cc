
#include <string>
#include "test.h"

using CustomType = void*;

namespace script::converter {

template <>
struct Converter<CustomType, void> {
  // convert custom type T to Local<Value>
  static Local<Value> toScript(CustomType value) {
    return String::newString(std::to_string(reinterpret_cast<int64_t>(value)));
  }

  // convert Local<Value> to custom type T
  static CustomType toCpp(const Local<Value>& value) {
    return reinterpret_cast<void*>(std::stoll(value.asString().toString()));
  }
};

}  // namespace script::converter

namespace script::test {
DEFINE_ENGINE_TEST(CustomConverterTest);

TEST_F(CustomConverterTest, CustomConverter) {
  EngineScope scope(engine);

  bool called = false;

  static_assert(converter::isConvertible<CustomType>);
  static_assert(!converter::isConvertible<int*>);

  auto func = Function::newFunction([](CustomType ptr) -> CustomType {
    *static_cast<bool*>(ptr) = true;
    return ptr;
  });

  auto ret = func.call({}, static_cast<CustomType>(&called));

  EXPECT_TRUE(called);
  ASSERT_TRUE(ret.isString());
  auto pointerString = std::to_string(reinterpret_cast<int64_t>(&called));
  EXPECT_EQ(ret.asString().toString(), pointerString);
}

}  // namespace script::test
