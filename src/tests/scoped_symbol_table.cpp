

#include "compiler/middle/analysis/scoped_symbol_table.hpp"

#include <CppUTest/TestHarness.h>

TEST_GROUP(scoped_symbol_table_tests){};

TEST(scoped_symbol_table_tests, all) {
  compiler::middle::analysis::scoped_symbol_table_c table;

  // Ensure we can't find anything
  {
    auto global_scope = table.get_current_scope();
    CHECK_TRUE_TEXT(global_scope, "Scope not created on table init");
    for (auto item : {"add", "sub", "div", "mul", "josh"}) {
      CHECK_TRUE_TEXT(global_scope->find(item) == nullptr,
                      "Found item in empty scope");
    }
  }

  auto function_names = {"add",   "sub",    "div",         "mul",
                         "print", "reboot", "do_something"};
  auto variable_names = {"x",   "y",   "i",   "idx",  "t",
                         "foo", "bar", "baz", "moot", "to"};

  table.new_scope();

  {
    auto current_scope = table.get_current_scope();
    for (auto fn : function_names) {
      current_scope->add(fn,
                         new compiler::middle::analysis::function_c(fn, {}));
    }

    for (auto fn : function_names) {
      auto locate = current_scope->find(fn);
      CHECK_TRUE_TEXT(locate, "Unable to locate added item");
      CHECK_EQUAL_TEXT(
          (int)locate->type,
          (int)compiler::middle::analysis::entry_c::entry_type_e::FUNCTION,
          "Unable to locate added item");
    }

    for (auto vn : variable_names) {
      current_scope->add(
          vn, new compiler::middle::analysis::type_c(
                  compiler::middle::analysis::base_forward_types_e::BOOL));
    }

    for (auto fn : function_names) {
      auto locate = current_scope->find(fn);
      CHECK_TRUE_TEXT(locate, "Unable to locate added item");
      CHECK_EQUAL_TEXT(
          (int)locate->type,
          (int)compiler::middle::analysis::entry_c::entry_type_e::FUNCTION,
          "Unable to locate added item");
    }

    for (auto vn : variable_names) {
      auto locate = current_scope->find(vn);
      CHECK_TRUE_TEXT(locate, "Unable to locate added item");
      CHECK_EQUAL_TEXT(
          (int)locate->type,
          (int)compiler::middle::analysis::entry_c::entry_type_e::MEMBER,
          "Unable to locate added item");
    }
  }

  table.pop_scope();

  {
    auto current_scope = table.get_current_scope();
    for (auto fn : function_names) {
      auto locate = current_scope->find(fn);
      CHECK_FALSE_TEXT(locate, "Able to locate delete item");
    }
    for (auto vn : variable_names) {
      auto locate = current_scope->find(vn);
      CHECK_FALSE_TEXT(locate, "Able to locate delete item");
    }
  }

  table.new_scope();
  {
    {
      auto current_scope = table.get_current_scope();
      for (auto vn : variable_names) {
        current_scope->add(
            vn, new compiler::middle::analysis::type_c(
                    compiler::middle::analysis::base_forward_types_e::BOOL));
      }
    }
    table.new_scope();
    table.new_scope();
    {
      auto current_scope = table.get_current_scope();
      for (auto vn : variable_names) {
        auto locate = current_scope->find(vn);
        CHECK_TRUE_TEXT(locate, "Unable to locate added item");
        CHECK_EQUAL_TEXT(
            (int)locate->type,
            (int)compiler::middle::analysis::entry_c::entry_type_e::MEMBER,
            "Unable to locate added item");
      }

      for (auto fn : function_names) {
        auto locate = current_scope->find(fn);
        CHECK_FALSE_TEXT(locate, "Able to locate delete item");
      }
    }

    table.pop_scope();
    {
      auto current_scope = table.get_current_scope();
      for (auto vn : variable_names) {
        auto locate = current_scope->find(vn);
        CHECK_TRUE_TEXT(locate, "Unable to locate added item");
        CHECK_EQUAL_TEXT(
            (int)locate->type,
            (int)compiler::middle::analysis::entry_c::entry_type_e::MEMBER,
            "Unable to locate added item");
      }

      for (auto fn : function_names) {
        auto locate = current_scope->find(fn);
        CHECK_FALSE_TEXT(locate, "Able to locate delete item");
      }
    }

    table.pop_scope();
    {
      auto current_scope = table.get_current_scope();
      for (auto vn : variable_names) {
        auto locate = current_scope->find(vn);
        CHECK_TRUE_TEXT(locate, "Unable to locate added item");
        CHECK_EQUAL_TEXT(
            (int)locate->type,
            (int)compiler::middle::analysis::entry_c::entry_type_e::MEMBER,
            "Unable to locate added item");
      }

      for (auto fn : function_names) {
        auto locate = current_scope->find(fn);
        CHECK_FALSE_TEXT(locate, "Able to locate delete item");
      }
    }

    table.pop_scope();
    {
      auto current_scope = table.get_current_scope();
      for (auto fn : function_names) {
        auto locate = current_scope->find(fn);
        CHECK_FALSE_TEXT(locate, "Able to locate delete item");
      }
      for (auto vn : variable_names) {
        auto locate = current_scope->find(vn);
        CHECK_FALSE_TEXT(locate, "Able to locate delete item");
      }
    }
  }
}