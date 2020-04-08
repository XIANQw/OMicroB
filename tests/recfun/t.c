#define OCAML_STACK_WOSIZE              2048
#define OCAML_STATIC_HEAP_WOSIZE           0
#define OCAML_DYNAMIC_HEAP_WOSIZE        256
#define OCAML_FLASH_HEAP_WOSIZE           48
#define OCAML_STACK_INITIAL_WOSIZE         3
#define OCAML_RAM_GLOBDATA_NUMBER          0
#define OCAML_FLASH_GLOBDATA_NUMBER        7
#define OCAML_BYTECODE_BSIZE             443
#define OCAML_PRIMITIVE_NUMBER             4
#define OCAML_VIRTUAL_ARCH                64
#define OCAML_STARTING_OOID                1
#define OCAML_NO_FLASH_HEAP                0
#define OCAML_NO_FLASH_GLOBALS             0

#define OCAML_GC_MARK_AND_COMPACT

#include </usr/local/include/omicrob/vm/values.h>

#define OCAML_ACC0                        0
#define OCAML_ACC1                        1
#define OCAML_ACC2                        2
#define OCAML_PUSH                        3
#define OCAML_PUSHACC1                    4
#define OCAML_PUSHACC2                    5
#define OCAML_PUSHACC3                    6
#define OCAML_PUSHACC4                    7
#define OCAML_PUSHACC5                    8
#define OCAML_POP                         9
#define OCAML_APPLY1                     10
#define OCAML_APPLY2                     11
#define OCAML_APPTERM1                   12
#define OCAML_APPTERM2                   13
#define OCAML_RETURN                     14
#define OCAML_RESTART                    15
#define OCAML_GRAB                       16
#define OCAML_CLOSURE_1B                 17
#define OCAML_CLOSUREREC_1B              18
#define OCAML_PUSHOFFSETCLOSUREM2        19
#define OCAML_PUSHOFFSETCLOSURE2         20
#define OCAML_PUSHOFFSETCLOSURE          21
#define OCAML_PUSHGETFLASHGLOBAL_1B      22
#define OCAML_MAKEBLOCK2                 23
#define OCAML_GETFIELD0                  24
#define OCAML_GETFIELD1                  25
#define OCAML_BRANCH_2B                  26
#define OCAML_BRANCHIF_1B                27
#define OCAML_BRANCHIFNOT_1B             28
#define OCAML_SWITCH_1B                  29
#define OCAML_C_CALL1                    30
#define OCAML_C_CALL2                    31
#define OCAML_CONST0                     32
#define OCAML_CONST1                     33
#define OCAML_CONST2                     34
#define OCAML_CONST3                     35
#define OCAML_CONSTINT_1B                36
#define OCAML_CONSTINT_2B                37
#define OCAML_PUSHCONST1                 38
#define OCAML_PUSHCONSTINT_1B            39
#define OCAML_OFFSETINT_1B               40
#define OCAML_STOP                       41

value ocaml_stack[OCAML_STACK_WOSIZE];
value ocaml_ram_heap[OCAML_STATIC_HEAP_WOSIZE + OCAML_DYNAMIC_HEAP_WOSIZE];
value ocaml_ram_global_data[OCAML_RAM_GLOBDATA_NUMBER];

PROGMEM value const ocaml_flash_heap[OCAML_FLASH_HEAP_WOSIZE] = {
  /*  0 */  Make_header(0, 0, Color_white),
  /*  1 */  Make_header(2, Object_tag, Color_white),
  /*  2 */  Val_flash_block(&ocaml_flash_heap[5]),
  /*  3 */  Val_int(-1),
  /*  4 */  Make_header(2, String_tag, Color_white),
  /*  5 */  Make_string_data('O', 'u', 't', '_', 'o', 'f', '_', 'm'),
  /*  6 */  Make_string_data('e', 'm', 'o', 'r', 'y', '\0', '\0', '\2'),
  /*  7 */  Make_header(2, Object_tag, Color_white),
  /*  8 */  Val_flash_block(&ocaml_flash_heap[11]),
  /*  9 */  Val_int(-3),
  /* 10 */  Make_header(1, String_tag, Color_white),
  /* 11 */  Make_string_data('F', 'a', 'i', 'l', 'u', 'r', 'e', '\0'),
  /* 12 */  Make_header(2, Object_tag, Color_white),
  /* 13 */  Val_flash_block(&ocaml_flash_heap[16]),
  /* 14 */  Val_int(-4),
  /* 15 */  Make_header(3, String_tag, Color_white),
  /* 16 */  Make_string_data('I', 'n', 'v', 'a', 'l', 'i', 'd', '_'),
  /* 17 */  Make_string_data('a', 'r', 'g', 'u', 'm', 'e', 'n', 't'),
  /* 18 */  Make_string_data('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\7'),
  /* 19 */  Make_header(2, Object_tag, Color_white),
  /* 20 */  Val_flash_block(&ocaml_flash_heap[23]),
  /* 21 */  Val_int(-6),
  /* 22 */  Make_header(3, String_tag, Color_white),
  /* 23 */  Make_string_data('D', 'i', 'v', 'i', 's', 'i', 'o', 'n'),
  /* 24 */  Make_string_data('_', 'b', 'y', '_', 'z', 'e', 'r', 'o'),
  /* 25 */  Make_string_data('\0', '\0', '\0', '\0', '\0', '\0', '\0', '\7'),
  /* 26 */  Make_header(2, Object_tag, Color_white),
  /* 27 */  Val_flash_block(&ocaml_flash_heap[30]),
  /* 28 */  Val_int(-9),
  /* 29 */  Make_header(2, String_tag, Color_white),
  /* 30 */  Make_string_data('S', 't', 'a', 'c', 'k', '_', 'o', 'v'),
  /* 31 */  Make_string_data('e', 'r', 'f', 'l', 'o', 'w', '\0', '\1'),
  /* 32 */  Make_header(1, Closure_tag, Color_white),
  /* 33 */  Val_codeptr(35),
  /* 34 */  Make_header(1, Closure_tag, Color_white),
  /* 35 */  Val_codeptr(383),
  /* 36 */  Make_header(1, Closure_tag, Color_white),
  /* 37 */  Val_codeptr(246),
  /* 38 */  Make_header(1, Closure_tag, Color_white),
  /* 39 */  Val_codeptr(142),
  /* 40 */  Make_header(1, Closure_tag, Color_white),
  /* 41 */  Val_codeptr(86),
  /* 42 */  Make_header(1, Closure_tag, Color_white),
  /* 43 */  Val_codeptr(198),
  /* 44 */  Make_header(1, Closure_tag, Color_white),
  /* 45 */  Val_codeptr(9),
  /* 46 */  Make_header(1, Closure_tag, Color_white),
  /* 47 */  Val_codeptr(3)
};

PROGMEM value const ocaml_initial_static_heap[OCAML_STATIC_HEAP_WOSIZE] = {
};

PROGMEM value const ocaml_initial_stack[OCAML_STACK_INITIAL_WOSIZE] = {
  /* 0 */  Val_int(10),
  /* 1 */  Val_int(1),
  /* 2 */  Val_flash_block(&ocaml_flash_heap[35])
};

PROGMEM value const ocaml_flash_global_data[OCAML_FLASH_GLOBDATA_NUMBER] = {
  /* 0 */  Val_flash_block(&ocaml_flash_heap[37]),
  /* 1 */  Val_flash_block(&ocaml_flash_heap[39]),
  /* 2 */  Val_flash_block(&ocaml_flash_heap[41]),
  /* 3 */  Val_flash_block(&ocaml_flash_heap[43]),
  /* 4 */  Val_flash_block(&ocaml_flash_heap[45]),
  /* 5 */  Val_flash_block(&ocaml_flash_heap[47]),
  /* 6 */  Val_flash_block(&ocaml_flash_heap[33])
};

value acc = Val_flash_block(&ocaml_flash_heap[33]);
value env = Val_unit;

#define OCAML_Out_of_memory        Val_flash_block(&ocaml_flash_heap[2])
#define OCAML_Failure              Val_flash_block(&ocaml_flash_heap[8])
#define OCAML_Invalid_argument     Val_flash_block(&ocaml_flash_heap[13])
#define OCAML_Division_by_zero     Val_flash_block(&ocaml_flash_heap[20])
#define OCAML_Stack_overflow       Val_flash_block(&ocaml_flash_heap[27])

#define OCAML_atom0                Val_flash_block(&ocaml_flash_heap[1])

PROGMEM opcode_t const ocaml_bytecode[OCAML_BYTECODE_BSIZE] = {
  /*   0 */  OCAML_BRANCH_2B, 1, 138,
  /*   3 */  OCAML_ACC0,
  /*   4 */  OCAML_C_CALL1, 0,
  /*   6 */  OCAML_RETURN, 1,
  /*   8 */  OCAML_RESTART,
  /*   9 */  OCAML_GRAB, 1,
  /*  11 */  OCAML_ACC0,
  /*  12 */  OCAML_PUSHGETFLASHGLOBAL_1B, 0,
  /*  14 */  OCAML_APPLY1,
  /*  15 */  OCAML_PUSHACC1,
  /*  16 */  OCAML_PUSHGETFLASHGLOBAL_1B, 1,
  /*  18 */  OCAML_APPLY1,
  /*  19 */  OCAML_PUSHACC3,
  /*  20 */  OCAML_BRANCHIFNOT_1B, 8,
  /*  22 */  OCAML_ACC0,
  /*  23 */  OCAML_PUSHACC2,
  /*  24 */  OCAML_C_CALL2, 1,
  /*  26 */  OCAML_RETURN, 4,
  /*  28 */  OCAML_ACC0,
  /*  29 */  OCAML_PUSHACC2,
  /*  30 */  OCAML_C_CALL2, 2,
  /*  32 */  OCAML_RETURN, 4,
  /*  34 */  OCAML_RESTART,
  /*  35 */  OCAML_GRAB, 1,
  /*  37 */  OCAML_ACC0,
  /*  38 */  OCAML_PUSHGETFLASHGLOBAL_1B, 0,
  /*  40 */  OCAML_APPLY1,
  /*  41 */  OCAML_PUSHACC1,
  /*  42 */  OCAML_PUSHGETFLASHGLOBAL_1B, 1,
  /*  44 */  OCAML_APPLY1,
  /*  45 */  OCAML_PUSHACC2,
  /*  46 */  OCAML_PUSHGETFLASHGLOBAL_1B, 2,
  /*  48 */  OCAML_APPLY1,
  /*  49 */  OCAML_PUSHACC3,
  /*  50 */  OCAML_PUSHGETFLASHGLOBAL_1B, 3,
  /*  52 */  OCAML_APPLY1,
  /*  53 */  OCAML_PUSHACC5,
  /*  54 */  OCAML_SWITCH_1B, 3, 0, 6, 16, 22,
  /*  60 */  OCAML_ACC1,
  /*  61 */  OCAML_PUSHACC1,
  /*  62 */  OCAML_C_CALL2, 1,
  /*  64 */  OCAML_ACC2,
  /*  65 */  OCAML_PUSHACC4,
  /*  66 */  OCAML_C_CALL2, 1,
  /*  68 */  OCAML_RETURN, 6,
  /*  70 */  OCAML_ACC1,
  /*  71 */  OCAML_PUSHACC1,
  /*  72 */  OCAML_C_CALL2, 2,
  /*  74 */  OCAML_RETURN, 6,
  /*  76 */  OCAML_ACC1,
  /*  77 */  OCAML_PUSHACC1,
  /*  78 */  OCAML_C_CALL2, 1,
  /*  80 */  OCAML_ACC2,
  /*  81 */  OCAML_PUSHACC4,
  /*  82 */  OCAML_C_CALL2, 2,
  /*  84 */  OCAML_RETURN, 6,
  /*  86 */  OCAML_ACC0,
  /*  87 */  OCAML_SWITCH_1B, 24, 0, 27, 30, 33, 36, 39, 43, 47, 43, 39, 51, 43, 47, 43, 47, 30, 33, 27, 36, 47, 43, 51, 39, 33, 36,
  /* 114 */  OCAML_CONST2,
  /* 115 */  OCAML_RETURN, 1,
  /* 117 */  OCAML_CONST3,
  /* 118 */  OCAML_RETURN, 1,
  /* 120 */  OCAML_CONST1,
  /* 121 */  OCAML_RETURN, 1,
  /* 123 */  OCAML_CONST0,
  /* 124 */  OCAML_RETURN, 1,
  /* 126 */  OCAML_CONSTINT_1B, 4,
  /* 128 */  OCAML_RETURN, 1,
  /* 130 */  OCAML_CONSTINT_1B, 6,
  /* 132 */  OCAML_RETURN, 1,
  /* 134 */  OCAML_CONSTINT_1B, 7,
  /* 136 */  OCAML_RETURN, 1,
  /* 138 */  OCAML_CONSTINT_1B, 5,
  /* 140 */  OCAML_RETURN, 1,
  /* 142 */  OCAML_ACC0,
  /* 143 */  OCAML_SWITCH_1B, 24, 0, 27, 30, 33, 36, 39, 43, 47, 43, 39, 51, 43, 47, 43, 47, 30, 33, 27, 36, 47, 43, 51, 39, 33, 36,
  /* 170 */  OCAML_CONST2,
  /* 171 */  OCAML_RETURN, 1,
  /* 173 */  OCAML_CONST3,
  /* 174 */  OCAML_RETURN, 1,
  /* 176 */  OCAML_CONST1,
  /* 177 */  OCAML_RETURN, 1,
  /* 179 */  OCAML_CONST0,
  /* 180 */  OCAML_RETURN, 1,
  /* 182 */  OCAML_CONSTINT_1B, 4,
  /* 184 */  OCAML_RETURN, 1,
  /* 186 */  OCAML_CONSTINT_1B, 6,
  /* 188 */  OCAML_RETURN, 1,
  /* 190 */  OCAML_CONSTINT_1B, 7,
  /* 192 */  OCAML_RETURN, 1,
  /* 194 */  OCAML_CONSTINT_1B, 5,
  /* 196 */  OCAML_RETURN, 1,
  /* 198 */  OCAML_ACC0,
  /* 199 */  OCAML_SWITCH_1B, 24, 0, 31, 31, 31, 31, 31, 35, 31, 27, 39, 39, 39, 39, 31, 35, 39, 39, 39, 39, 43, 43, 43, 43, 43, 43,
  /* 226 */  OCAML_CONSTINT_1B, 8,
  /* 228 */  OCAML_RETURN, 1,
  /* 230 */  OCAML_CONSTINT_1B, 7,
  /* 232 */  OCAML_RETURN, 1,
  /* 234 */  OCAML_CONSTINT_1B, 6,
  /* 236 */  OCAML_RETURN, 1,
  /* 238 */  OCAML_CONSTINT_1B, 5,
  /* 240 */  OCAML_RETURN, 1,
  /* 242 */  OCAML_CONSTINT_1B, 9,
  /* 244 */  OCAML_RETURN, 1,
  /* 246 */  OCAML_ACC0,
  /* 247 */  OCAML_SWITCH_1B, 24, 0, 30, 30, 30, 30, 30, 33, 30, 27, 36, 36, 36, 36, 30, 33, 36, 36, 36, 36, 39, 39, 39, 39, 39, 39,
  /* 274 */  OCAML_CONST3,
  /* 275 */  OCAML_RETURN, 1,
  /* 277 */  OCAML_CONST2,
  /* 278 */  OCAML_RETURN, 1,
  /* 280 */  OCAML_CONST1,
  /* 281 */  OCAML_RETURN, 1,
  /* 283 */  OCAML_CONST0,
  /* 284 */  OCAML_RETURN, 1,
  /* 286 */  OCAML_CONSTINT_1B, 4,
  /* 288 */  OCAML_RETURN, 1,
  /* 290 */  OCAML_RESTART,
  /* 291 */  OCAML_GRAB, 1,
  /* 293 */  OCAML_CONST0,
  /* 294 */  OCAML_PUSHACC1,
  /* 295 */  OCAML_PUSHGETFLASHGLOBAL_1B, 4,
  /* 297 */  OCAML_APPLY2,
  /* 298 */  OCAML_CONSTINT_2B, 0, 250,
  /* 301 */  OCAML_PUSHGETFLASHGLOBAL_1B, 5,
  /* 303 */  OCAML_APPLY1,
  /* 304 */  OCAML_CONST1,
  /* 305 */  OCAML_PUSHACC1,
  /* 306 */  OCAML_PUSHGETFLASHGLOBAL_1B, 4,
  /* 308 */  OCAML_APPLY2,
  /* 309 */  OCAML_ACC1,
  /* 310 */  OCAML_PUSHOFFSETCLOSURE2,
  /* 311 */  OCAML_APPTERM1, 3,
  /* 313 */  OCAML_ACC0,
  /* 314 */  OCAML_BRANCHIFNOT_1B, 11,
  /* 316 */  OCAML_ACC0,
  /* 317 */  OCAML_GETFIELD1,
  /* 318 */  OCAML_PUSHACC1,
  /* 319 */  OCAML_GETFIELD0,
  /* 320 */  OCAML_PUSHACC1,
  /* 321 */  OCAML_PUSHACC1,
  /* 322 */  OCAML_PUSHOFFSETCLOSUREM2,
  /* 323 */  OCAML_APPTERM2, 5,
  /* 325 */  OCAML_CONST0,
  /* 326 */  OCAML_RETURN, 1,
  /* 328 */  OCAML_CLOSUREREC_1B, 2, 0, (opcode_t) -37, (opcode_t) -15,
  /* 333 */  OCAML_ACC2,
  /* 334 */  OCAML_PUSHACC1,
  /* 335 */  OCAML_APPTERM1, 4,
  /* 337 */  OCAML_ACC0,
  /* 338 */  OCAML_BRANCHIF_1B, 5,
  /* 340 */  OCAML_CONST0,
  /* 341 */  OCAML_RETURN, 1,
  /* 343 */  OCAML_ACC0,
  /* 344 */  OCAML_OFFSETINT_1B, (opcode_t) -1,
  /* 346 */  OCAML_PUSHOFFSETCLOSURE2,
  /* 347 */  OCAML_APPLY1,
  /* 348 */  OCAML_OFFSETINT_1B, 1,
  /* 350 */  OCAML_RETURN, 1,
  /* 352 */  OCAML_ACC0,
  /* 353 */  OCAML_BRANCHIF_1B, 5,
  /* 355 */  OCAML_CONST0,
  /* 356 */  OCAML_RETURN, 1,
  /* 358 */  OCAML_ACC0,
  /* 359 */  OCAML_OFFSETINT_1B, (opcode_t) -1,
  /* 361 */  OCAML_PUSHOFFSETCLOSURE2,
  /* 362 */  OCAML_APPLY1,
  /* 363 */  OCAML_OFFSETINT_1B, 10,
  /* 365 */  OCAML_RETURN, 1,
  /* 367 */  OCAML_ACC0,
  /* 368 */  OCAML_BRANCHIF_1B, 5,
  /* 370 */  OCAML_CONST0,
  /* 371 */  OCAML_RETURN, 1,
  /* 373 */  OCAML_ACC0,
  /* 374 */  OCAML_OFFSETINT_1B, (opcode_t) -1,
  /* 376 */  OCAML_PUSHOFFSETCLOSURE, (opcode_t) -4,
  /* 378 */  OCAML_APPLY1,
  /* 379 */  OCAML_OFFSETINT_1B, 100,
  /* 381 */  OCAML_RETURN, 1,
  /* 383 */  OCAML_CLOSUREREC_1B, 3, 0, (opcode_t) -46, (opcode_t) -31, (opcode_t) -16,
  /* 389 */  OCAML_CONSTINT_1B, 10,
  /* 391 */  OCAML_PUSHACC3,
  /* 392 */  OCAML_APPTERM1, 5,
  /* 394 */  OCAML_APPLY2,
  /* 395 */  OCAML_CONST0,
  /* 396 */  OCAML_PUSHACC1,
  /* 397 */  OCAML_APPLY1,
  /* 398 */  OCAML_C_CALL1, 3,
  /* 400 */  OCAML_CLOSURE_1B, 0, (opcode_t) -72,
  /* 403 */  OCAML_PUSHCONSTINT_1B, 9,
  /* 405 */  OCAML_PUSHCONSTINT_1B, 10,
  /* 407 */  OCAML_PUSHCONSTINT_1B, 11,
  /* 409 */  OCAML_PUSHCONST1,
  /* 410 */  OCAML_PUSHACC3,
  /* 411 */  OCAML_PUSHGETFLASHGLOBAL_1B, 6,
  /* 413 */  OCAML_APPLY2,
  /* 414 */  OCAML_CONST1,
  /* 415 */  OCAML_PUSHACC2,
  /* 416 */  OCAML_PUSHGETFLASHGLOBAL_1B, 6,
  /* 418 */  OCAML_APPLY2,
  /* 419 */  OCAML_CONST1,
  /* 420 */  OCAML_PUSHACC1,
  /* 421 */  OCAML_PUSHGETFLASHGLOBAL_1B, 6,
  /* 423 */  OCAML_APPLY2,
  /* 424 */  OCAML_CONST0,
  /* 425 */  OCAML_PUSHACC3,
  /* 426 */  OCAML_MAKEBLOCK2, 0,
  /* 428 */  OCAML_PUSHACC1,
  /* 429 */  OCAML_MAKEBLOCK2, 0,
  /* 431 */  OCAML_PUSHACC2,
  /* 432 */  OCAML_MAKEBLOCK2, 0,
  /* 434 */  OCAML_PUSHACC3,
  /* 435 */  OCAML_MAKEBLOCK2, 0,
  /* 437 */  OCAML_PUSH,
  /* 438 */  OCAML_PUSHACC5,
  /* 439 */  OCAML_APPLY1,
  /* 440 */  OCAML_POP, 6,
  /* 442 */  OCAML_STOP
};

#include </usr/local/include/omicrob/vm/runtime.c>

PROGMEM void * const ocaml_primitives[OCAML_PRIMITIVE_NUMBER] = {
  /*  0 */  (void *) &caml_delay,
  /*  1 */  (void *) &caml_clear_bit,
  /*  2 */  (void *) &caml_set_bit,
  /*  3 */  (void *) &caml_debug_tracei,
};
