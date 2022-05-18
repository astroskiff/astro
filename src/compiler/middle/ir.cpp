
#include "ir.hpp"

namespace compiler {
namespace middle {
namespace ir {

void push_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void goto_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void jump_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void return_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void call_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void pop_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void add_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void sub_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void div_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void mul_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void mod_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void pow_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void lsh_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void rsh_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void lt_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void gt_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void eq_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void neq_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void and_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void or_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void not_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void bw_and_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void bw_or_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void bw_not_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void bw_xor_c::visit(ir_translator_c &translator) { translator.accept(*this); }

void asm_c::visit(ir_translator_c &translator) { translator.accept(*this); }

} // namespace ir
} // namespace middle
} // namespace compiler