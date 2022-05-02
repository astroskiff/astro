#include "compiler/lang/types.hpp"

#include <string>
#include <vector>

#include <CppUTest/TestHarness.h>

TEST_GROUP(types_test){};

//  Check the type conversion logic for base types
//
TEST(types_test, conversion_logic) {
  struct tc {
    compiler::lang::types::base_type_c *from;
    compiler::lang::types::base_type_c *to;
    compiler::lang::types::conversion_e expected;
  };

  std::vector<tc> test_cases = {
      {new compiler::lang::types::u8_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u8_c(),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::U8},
           compiler::lang::types::container_variant_e::FIXED),
       compiler::lang::types::conversion_e::INVALID},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u16_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u32_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::u64_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i8_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i16_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i32_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::i64_c(), new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::PROMOTION},
      {new compiler::lang::types::float_c(), new compiler::lang::types::u8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::u16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::u32_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::u64_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::i8_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::i16_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::i32_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(), new compiler::lang::types::i64_c(),
       compiler::lang::types::conversion_e::TRUNCATE},
      {new compiler::lang::types::float_c(),
       new compiler::lang::types::float_c(),
       compiler::lang::types::conversion_e::DIRECT},
      {new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::I64},
           compiler::lang::types::container_variant_e::FIXED),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::I32},
           compiler::lang::types::container_variant_e::FIXED),
       compiler::lang::types::conversion_e::INDIRECT},
      {new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::FLOAT},
           compiler::lang::types::container_variant_e::FIXED),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::U32},
           compiler::lang::types::container_variant_e::FIXED),
       compiler::lang::types::conversion_e::INDIRECT},
      {new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::I64},
           compiler::lang::types::container_variant_e::FIXED),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::CONTAINER},
           compiler::lang::types::container_variant_e::FIXED),
       compiler::lang::types::conversion_e::INVALID},
      {new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::FLOAT},
           compiler::lang::types::container_variant_e::VARIABLE),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::U32},
           compiler::lang::types::container_variant_e::VARIABLE),
       compiler::lang::types::conversion_e::INDIRECT},
      {new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::FLOAT,
            compiler::lang::types::type_e::I16},
           compiler::lang::types::container_variant_e::VARIABLE),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::U32},
           compiler::lang::types::container_variant_e::VARIABLE),
       compiler::lang::types::conversion_e::INVALID},
      {new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::FLOAT},
           compiler::lang::types::container_variant_e::FIXED),
       new compiler::lang::types::container_c(
           {compiler::lang::types::type_e::U32},
           compiler::lang::types::container_variant_e::VARIABLE),
       compiler::lang::types::conversion_e::INVALID},
  };

  uint64_t i{0};
  for (auto &c : test_cases) {
    auto result = compiler::lang::types::check_conversion(c.from, c.to);
    CHECK_EQUAL_TEXT(
        static_cast<int>(c.expected), static_cast<int>(result),
        std::string("Unexpected conversion result : " + std::to_string(i))
            .c_str());
    i++;
  }

  // Cleanup
  for (size_t i = 0; i < test_cases.size(); i++) {
    delete test_cases[i].to;
    delete test_cases[i].from;
  }
}