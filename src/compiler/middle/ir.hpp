#ifndef COMPILER_MIDDLE_GENERATION_IR_HPP
#define COMPILER_MIDDLE_GENERATION_IR_HPP

#include <string>
#include <vector>

namespace compiler {
namespace middle {
namespace ir {

//! \brief Interface for something that will translate the IR
//!        to a target
class ir_translator_c;

class ir_instruction_c {
public:
  ~ir_instruction_c() = default;
  virtual void visit(ir_translator_c &translator) = 0;
};

class push_c : ir_instruction_c {
public:
  push_c(const std::vector<uint8_t> &value, const uint64_t &bytes)
      : value(value), n_bytes(bytes) {}
  virtual void visit(ir_translator_c &translator) override;
  std::vector<uint8_t> value;
  uint64_t n_bytes{0};
};

class pop_c : ir_instruction_c {
public:
  pop_c(const std::string &dest, const uint64_t &bytes)
      : dest(dest), n_bytes(bytes) {}
  virtual void visit(ir_translator_c &translator) override;
  std::string dest;
  uint64_t n_bytes{0};
};

class goto_c : ir_instruction_c {
public:
  goto_c(const std::string &dest) : dest(dest) {}
  virtual void visit(ir_translator_c &translator) override;
  std::string dest;
};

class jump_c : ir_instruction_c {
public:
  jump_c(const std::string &dest) : dest(dest) {}
  virtual void visit(ir_translator_c &translator) override;
  std::string dest;
};

class return_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class call_c : ir_instruction_c {
public:
  call_c(const std::string &dest) : dest(dest) {}
  virtual void visit(ir_translator_c &translator) override;
  std::string dest;
};

class add_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class sub_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class div_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class mul_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class mod_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class pow_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class lsh_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class rsh_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class lt_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class gt_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class eq_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class neq_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class and_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class or_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class not_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class bw_and_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class bw_or_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class bw_not_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class bw_xor_c : ir_instruction_c {
public:
  virtual void visit(ir_translator_c &translator) override;
};

class asm_c : ir_instruction_c {
public:
  asm_c(const std::string &data) : data(data) {}
  virtual void visit(ir_translator_c &translator) override;
  std::string data;
};

class ir_translator_c {
public:
  virtual void accept(push_c &ins) = 0;
  virtual void accept(pop_c &ins) = 0;
  virtual void accept(goto_c &ins) = 0;
  virtual void accept(jump_c &ins) = 0;
  virtual void accept(return_c &ins) = 0;
  virtual void accept(call_c &ins) = 0;
  virtual void accept(add_c &ins) = 0;
  virtual void accept(sub_c &ins) = 0;
  virtual void accept(div_c &ins) = 0;
  virtual void accept(mul_c &ins) = 0;
  virtual void accept(pow_c &ins) = 0;
  virtual void accept(mod_c &ins) = 0;
  virtual void accept(lsh_c &ins) = 0;
  virtual void accept(rsh_c &ins) = 0;
  virtual void accept(lt_c &ins) = 0;
  virtual void accept(gt_c &ins) = 0;
  virtual void accept(eq_c &ins) = 0;
  virtual void accept(neq_c &ins) = 0;
  virtual void accept(and_c &ins) = 0;
  virtual void accept(or_c &ins) = 0;
  virtual void accept(not_c &ins) = 0;
  virtual void accept(bw_and_c &ins) = 0;
  virtual void accept(bw_or_c &ins) = 0;
  virtual void accept(bw_not_c &ins) = 0;
  virtual void accept(bw_xor_c &ins) = 0;
  virtual void accept(asm_c &ins) = 0;
};

} // namespace ir
} // namespace middle
} // namespace compiler

#endif