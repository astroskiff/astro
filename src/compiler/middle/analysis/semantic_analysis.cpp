#include "semantic_analysis.hpp"
#include "compiler/shared/reporter.hpp"

#include <iostream>

namespace compiler {
namespace middle {

semantic_analysis_c::semantic_analysis_c(
    std::unordered_map<std::string, shared::page_c> &pages)
    : _pages(pages) {}

std::tuple<bool, std::vector<ir::ir_instruction_c *>>
semantic_analysis_c::analyze(const std::vector<node_c *> &instructions) {

  for(auto &ins : instructions) {
    analyze_node(ins);
  }
 
  if(!_sa_okay) {
    for(auto &ins : _resulting_instructions) {
      delete ins;
    }
    return {false, {}};
  }

  return {true, _resulting_instructions};
}

void semantic_analysis_c::die(node_c *node, uint64_t error_no, std::string error, bool basic_error)
{
  _sa_okay = false;
  compiler::shared::reporter_c reporter(_pages);
  if (!node || basic_error) {
    reporter.standard_report(compiler::shared::base_report_c(
        compiler::shared::report_origin_e::PARSER,
        compiler::shared::level_e::ERROR, error));
  } else {
    reporter.marked_report(compiler::shared::marked_source_report_c(
        compiler::shared::report_origin_e::PARSER,
        compiler::shared::level_e::ERROR, error,
        node->location, error_no));
  }
}

void semantic_analysis_c::analyze_node(node_c *node)
{
  // Within the SA here we should only hit "top level" nodes
  // So.. full statement constructs not operations

  if(!node) {
    return;
  }

  switch(node->type) {
    case node_type_e::FN: {
      analyze_function(reinterpret_cast<compiler::function_node_c*>(node));
    }
    case node_type_e::CALL: { break; }
    case node_type_e::ASM: { break; }
    case node_type_e::LET: { break; }
    case node_type_e::REASSIGN: { break; }
    case node_type_e::LABEL: { break; }
    case node_type_e::FOR: { break; }
    case node_type_e::GOTO: { break; }
    case node_type_e::GOSUB: { break; }
    case node_type_e::RETURN: { break; }
    default: {

      //  TODO : Update this to use die() once file / node information can be
      //         tied together
      //
      std::cerr << "Internal Error: SA hit non-statement node" << std::endl;
      _sa_okay = false;
      return;
    }
  }
}

void semantic_analysis_c::analyze_function(function_node_c * fn)
{
  // Prepare data for table representation of the function
  //
  std::vector<analysis::function_c::param_t> params;
  for(auto &param : fn->params) {
    auto p = reinterpret_cast<compiler::variable_c*>(param);
    params.push_back({
      p->data,
      {p->data_type}
    });
  }

  //  Add the function to the table
  //
  if(auto scope = _symbol_table.get_current_scope()) {
    scope->add(fn->data, new analysis::function_c(fn->data, params, fn->data_type));
  }

  //  Enter a new scope for the function
  //
  _symbol_table.new_scope();

  //  Add the parameters to the scope
  //
  if(auto scope = _symbol_table.get_current_scope()) {
    for(auto &param : params) {
      if(scope->find(param.name)) {
        die(fn, 
            0, 
            "Function contains multiple parameters with same name : " + param.name);
        return;
      }
      scope->add(param.name, new analysis::type_c(param.type));
    }
  }

  //  Generate IR for function start
  //
  build_function_ir(fn);

  //  Iterate over the function body and build it
  //
  for(auto &el : fn->body) {
    analyze_node(el);
  }

  //  Leave the function scope
  //
  _symbol_table.pop_scope();
}

void semantic_analysis_c::build_function_ir(function_node_c * fn)
{
  // -- Create label for function
  // -- Create 'assign' and 'pop' statements for parameters
  // > See middle::README.md
}

} // namespace middle
} // namespace compiler