.init __skiff__entry__method

.u64 _mc 0
.u64 _fc 0
.u64 _stack_address 0

.code 

;   Actual VM entry - sets up machine for execution
;   and calls into user supplied entry method
;
__skiff__entry__method:

  ; Allocate 1 Mib and ensure that it was stored in 
  ; the expected memory slot (1)
  ;
  mov i0 @1048576
  alloc i0 i0
  aseq x1 i0

  call hll_fn_program_entry
  exit

;
;   Generated user code below this
;

hll_fn_program_entry:

  ret