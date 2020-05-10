#define OCAML_STACK_WOSIZE              2048
#define OCAML_STATIC_HEAP_WOSIZE           4
#define OCAML_DYNAMIC_HEAP_WOSIZE        252
#define OCAML_FLASH_HEAP_WOSIZE           81
#define OCAML_STACK_INITIAL_WOSIZE         1
#define OCAML_RAM_GLOBDATA_NUMBER          0
#define OCAML_FLASH_GLOBDATA_NUMBER       15
#define OCAML_BYTECODE_BSIZE             264
#define OCAML_PRIMITIVE_NUMBER            12
#define OCAML_VIRTUAL_ARCH                32
#define OCAML_STARTING_OOID                1
#define OCAML_NO_FLASH_HEAP                0
#define OCAML_NO_FLASH_GLOBALS             0

#define OCAML_GC_MARK_AND_COMPACT

#include </Users/ruiwenwang/Desktop/OMicroB/src/byterun/vm/values.h>

#define OCAML_ACC0                        0
#define OCAML_ACC1                        1
#define OCAML_ACC2                        2
#define OCAML_PUSH                        3
#define OCAML_PUSHACC1                    4
#define OCAML_PUSHACC2                    5
#define OCAML_PUSHACC3                    6
#define OCAML_PUSHACC4                    7
#define OCAML_PUSHACC5                    8
#define OCAML_PUSHACC7                    9
#define OCAML_PUSHACC                    10
#define OCAML_ASSIGN                     11
#define OCAML_APPLY1                     12
#define OCAML_APPLY2                     13
#define OCAML_APPLY3                     14
#define OCAML_APPTERM1                   15
#define OCAML_APPTERM2                   16
#define OCAML_APPTERM3                   17
#define OCAML_RETURN                     18
#define OCAML_RESTART                    19
#define OCAML_GRAB                       20
#define OCAML_CLOSURE_1B                 21
#define OCAML_GETFLASHGLOBAL_1B          22
#define OCAML_PUSHGETFLASHGLOBAL_1B      23
#define OCAML_MAKEBLOCK2                 24
#define OCAML_GETFIELD1                  25
#define OCAML_GETBYTESCHAR               26
#define OCAML_BRANCH_1B                  27
#define OCAML_BRANCH_2B                  28
#define OCAML_BRANCHIF_1B                29
#define OCAML_BRANCHIFNOT_1B             30
#define OCAML_RAISE                      31
#define OCAML_CHECK_SIGNALS              32
#define OCAML_C_CALL1                    33
#define OCAML_C_CALL4                    34
#define OCAML_C_CALL5                    35
#define OCAML_CONST0                     36
#define OCAML_CONST1                     37
#define OCAML_CONSTINT_2B                38
#define OCAML_PUSHCONST0                 39
#define OCAML_PUSHCONST1                 40
#define OCAML_ADDINT                     41
#define OCAML_SUBINT                     42
#define OCAML_NEQ                        43
#define OCAML_GTINT                      44
#define OCAML_OFFSETINT_1B               45
#define OCAML_BEQ_1B                     46
#define OCAML_BNEQ_1B                    47
#define OCAML_BGTINT_1B                  48
#define OCAML_STOP                       49

value ocaml_stack[OCAML_STACK_WOSIZE];
value ocaml_ram_heap[OCAML_STATIC_HEAP_WOSIZE + OCAML_DYNAMIC_HEAP_WOSIZE];
value ocaml_ram_global_data[OCAML_RAM_GLOBDATA_NUMBER];

PROGMEM value const ocaml_flash_heap[OCAML_FLASH_HEAP_WOSIZE] = {
  /*  0 */  Make_header(0, 0, Color_white),
  /*  1 */  Make_header(2, Object_tag, Color_white),
  /*  2 */  Val_flash_block(&ocaml_flash_heap[5]),
  /*  3 */  Val_int(-1),
  /*  4 */  Make_header(4, String_tag, Color_white),
  /*  5 */  Make_string_data('O', 'u', 't', '_'),
  /*  6 */  Make_string_data('o', 'f', '_', 'm'),
  /*  7 */  Make_string_data('e', 'm', 'o', 'r'),
  /*  8 */  Make_string_data('y', '\0', '\0', '\2'),
  /*  9 */  Make_header(2, Object_tag, Color_white),
  /* 10 */  Val_flash_block(&ocaml_flash_heap[13]),
  /* 11 */  Val_int(-3),
  /* 12 */  Make_header(2, String_tag, Color_white),
  /* 13 */  Make_string_data('F', 'a', 'i', 'l'),
  /* 14 */  Make_string_data('u', 'r', 'e', '\0'),
  /* 15 */  Make_header(2, Object_tag, Color_white),
  /* 16 */  Val_flash_block(&ocaml_flash_heap[19]),
  /* 17 */  Val_int(-4),
  /* 18 */  Make_header(5, String_tag, Color_white),
  /* 19 */  Make_string_data('I', 'n', 'v', 'a'),
  /* 20 */  Make_string_data('l', 'i', 'd', '_'),
  /* 21 */  Make_string_data('a', 'r', 'g', 'u'),
  /* 22 */  Make_string_data('m', 'e', 'n', 't'),
  /* 23 */  Make_string_data('\0', '\0', '\0', '\3'),
  /* 24 */  Make_header(2, Object_tag, Color_white),
  /* 25 */  Val_flash_block(&ocaml_flash_heap[28]),
  /* 26 */  Val_int(-6),
  /* 27 */  Make_header(5, String_tag, Color_white),
  /* 28 */  Make_string_data('D', 'i', 'v', 'i'),
  /* 29 */  Make_string_data('s', 'i', 'o', 'n'),
  /* 30 */  Make_string_data('_', 'b', 'y', '_'),
  /* 31 */  Make_string_data('z', 'e', 'r', 'o'),
  /* 32 */  Make_string_data('\0', '\0', '\0', '\3'),
  /* 33 */  Make_header(2, Object_tag, Color_white),
  /* 34 */  Val_flash_block(&ocaml_flash_heap[37]),
  /* 35 */  Val_int(-9),
  /* 36 */  Make_header(4, String_tag, Color_white),
  /* 37 */  Make_string_data('S', 't', 'a', 'c'),
  /* 38 */  Make_string_data('k', '_', 'o', 'v'),
  /* 39 */  Make_string_data('e', 'r', 'f', 'l'),
  /* 40 */  Make_string_data('o', 'w', '\0', '\1'),
  /* 41 */  Make_header(1, Closure_tag, Color_white),
  /* 42 */  Val_codeptr(235),
  /* 43 */  Make_header(4, String_tag, Color_white),
  /* 44 */  Make_string_data('H', 'e', 'l', 'l'),
  /* 45 */  Make_string_data('o', ' ', 'w', 'o'),
  /* 46 */  Make_string_data('r', 'l', 'd', 0x0A),
  /* 47 */  Make_string_data('\0', '\0', '\0', '\3'),
  /* 48 */  Make_header(6, String_tag, Color_white),
  /* 49 */  Make_string_data('S', 't', 'r', 'i'),
  /* 50 */  Make_string_data('n', 'g', '.', 's'),
  /* 51 */  Make_string_data('u', 'b', ' ', '/'),
  /* 52 */  Make_string_data(' ', 'B', 'y', 't'),
  /* 53 */  Make_string_data('e', 's', '.', 's'),
  /* 54 */  Make_string_data('u', 'b', '\0', '\1'),
  /* 55 */  Make_header(1, Closure_tag, Color_white),
  /* 56 */  Val_codeptr(37),
  /* 57 */  Make_header(1, Closure_tag, Color_white),
  /* 58 */  Val_codeptr(48),
  /* 59 */  Make_header(1, Closure_tag, Color_white),
  /* 60 */  Val_codeptr(54),
  /* 61 */  Make_header(1, Closure_tag, Color_white),
  /* 62 */  Val_codeptr(43),
  /* 63 */  Make_header(1, Closure_tag, Color_white),
  /* 64 */  Val_codeptr(93),
  /* 65 */  Make_header(1, String_tag, Color_white),
  /* 66 */  Make_string_data('\0', '\0', '\0', '\3'),
  /* 67 */  Make_header(1, Closure_tag, Color_white),
  /* 68 */  Val_codeptr(157),
  /* 69 */  Make_header(1, Closure_tag, Color_white),
  /* 70 */  Val_codeptr(4),
  /* 71 */  Make_header(1, Closure_tag, Color_white),
  /* 72 */  Val_codeptr(141),
  /* 73 */  Make_header(1, Closure_tag, Color_white),
  /* 74 */  Val_codeptr(108),
  /* 75 */  Make_header(1, Closure_tag, Color_white),
  /* 76 */  Val_codeptr(244),
  /* 77 */  Make_header(1, Closure_tag, Color_white),
  /* 78 */  Val_codeptr(173),
  /* 79 */  Make_header(1, Closure_tag, Color_white),
  /* 80 */  Val_codeptr(168)
};

PROGMEM value const ocaml_initial_static_heap[OCAML_STATIC_HEAP_WOSIZE] = {
  /* 0 */  Make_header(3, 0, Color_white),
  /* 1 */  Val_flash_block(&ocaml_flash_heap[76]),
  /* 2 */  Val_flash_block(&ocaml_flash_heap[42]),
  /* 3 */  Val_flash_block(&ocaml_flash_heap[78])
};

PROGMEM value const ocaml_initial_stack[OCAML_STACK_INITIAL_WOSIZE] = {
  /* 0 */  Val_flash_block(&ocaml_flash_heap[44])
};

PROGMEM value const ocaml_flash_global_data[OCAML_FLASH_GLOBDATA_NUMBER] = {
  /*  0 */  Val_flash_block(&ocaml_flash_heap[16]),
  /*  1 */  Val_flash_block(&ocaml_flash_heap[49]),
  /*  2 */  Val_flash_block(&ocaml_flash_heap[56]),
  /*  3 */  Val_flash_block(&ocaml_flash_heap[58]),
  /*  4 */  Val_flash_block(&ocaml_flash_heap[60]),
  /*  5 */  Val_flash_block(&ocaml_flash_heap[62]),
  /*  6 */  Val_flash_block(&ocaml_flash_heap[64]),
  /*  7 */  Val_flash_block(&ocaml_flash_heap[66]),
  /*  8 */  Val_flash_block(&ocaml_flash_heap[68]),
  /*  9 */  Val_flash_block(&ocaml_flash_heap[70]),
  /* 10 */  Val_flash_block(&ocaml_flash_heap[72]),
  /* 11 */  Val_flash_block(&ocaml_flash_heap[74]),
  /* 12 */  Val_static_block(&ocaml_ram_heap[1]),
  /* 13 */  Val_flash_block(&ocaml_flash_heap[44]),
  /* 14 */  Val_flash_block(&ocaml_flash_heap[80])
};

value acc = Val_flash_block(&ocaml_flash_heap[42]);
value env = Val_unit;

#define OCAML_Out_of_memory        Val_flash_block(&ocaml_flash_heap[2])
#define OCAML_Failure              Val_flash_block(&ocaml_flash_heap[10])
#define OCAML_Invalid_argument     Val_flash_block(&ocaml_flash_heap[16])
#define OCAML_Division_by_zero     Val_flash_block(&ocaml_flash_heap[25])
#define OCAML_Stack_overflow       Val_flash_block(&ocaml_flash_heap[34])

#define OCAML_atom0                Val_flash_block(&ocaml_flash_heap[1])

PROGMEM opcode_t const ocaml_bytecode[OCAML_BYTECODE_BSIZE] = {
  /*   0 */  OCAML_BRANCH_2B, 0, 253,
  /*   3 */  OCAML_RESTART,
  /*   4 */  OCAML_GRAB, 1,
  /*   6 */  OCAML_ACC0,
  /*   7 */  OCAML_C_CALL1, 0,
  /*   9 */  OCAML_PUSHACC2,
  /*  10 */  OCAML_C_CALL1, 0,
  /*  12 */  OCAML_PUSH,
  /*  13 */  OCAML_PUSHACC2,
  /*  14 */  OCAML_ADDINT,
  /*  15 */  OCAML_C_CALL1, 1,
  /*  17 */  OCAML_PUSHACC2,
  /*  18 */  OCAML_PUSHCONST0,
  /*  19 */  OCAML_PUSHACC2,
  /*  20 */  OCAML_PUSHCONST0,
  /*  21 */  OCAML_PUSHACC7,
  /*  22 */  OCAML_C_CALL5, 2,
  /*  24 */  OCAML_ACC1,
  /*  25 */  OCAML_PUSHACC3,
  /*  26 */  OCAML_PUSHACC2,
  /*  27 */  OCAML_PUSHCONST0,
  /*  28 */  OCAML_PUSHACC, 8,
  /*  30 */  OCAML_C_CALL5, 2,
  /*  32 */  OCAML_ACC0,
  /*  33 */  OCAML_C_CALL1, 3,
  /*  35 */  OCAML_RETURN, 5,
  /*  37 */  OCAML_ACC0,
  /*  38 */  OCAML_PUSHGETFLASHGLOBAL_1B, 0,
  /*  40 */  OCAML_MAKEBLOCK2, 0,
  /*  42 */  OCAML_RAISE,
  /*  43 */  OCAML_ACC0,
  /*  44 */  OCAML_C_CALL1, 4,
  /*  46 */  OCAML_RETURN, 1,
  /*  48 */  OCAML_ACC0,
  /*  49 */  OCAML_C_CALL1, 5,
  /*  51 */  OCAML_RETURN, 1,
  /*  53 */  OCAML_RESTART,
  /*  54 */  OCAML_GRAB, 2,
  /*  56 */  OCAML_ACC1,
  /*  57 */  OCAML_BGTINT_1B, 0, 16,
  /*  60 */  OCAML_ACC2,
  /*  61 */  OCAML_BGTINT_1B, 0, 12,
  /*  64 */  OCAML_ACC2,
  /*  65 */  OCAML_PUSHACC1,
  /*  66 */  OCAML_C_CALL1, 6,
  /*  68 */  OCAML_SUBINT,
  /*  69 */  OCAML_PUSHACC2,
  /*  70 */  OCAML_GTINT,
  /*  71 */  OCAML_BRANCHIFNOT_1B, 8,
  /*  73 */  OCAML_GETFLASHGLOBAL_1B, 1,
  /*  75 */  OCAML_PUSHGETFLASHGLOBAL_1B, 2,
  /*  77 */  OCAML_APPTERM1, 4,
  /*  79 */  OCAML_ACC2,
  /*  80 */  OCAML_C_CALL1, 1,
  /*  82 */  OCAML_PUSHACC3,
  /*  83 */  OCAML_PUSHCONST0,
  /*  84 */  OCAML_PUSHACC2,
  /*  85 */  OCAML_PUSHACC5,
  /*  86 */  OCAML_PUSHACC5,
  /*  87 */  OCAML_C_CALL5, 7,
  /*  89 */  OCAML_ACC0,
  /*  90 */  OCAML_RETURN, 4,
  /*  92 */  OCAML_RESTART,
  /*  93 */  OCAML_GRAB, 1,
  /*  95 */  OCAML_ACC0,
  /*  96 */  OCAML_C_CALL1, 1,
  /*  98 */  OCAML_PUSHACC2,
  /*  99 */  OCAML_PUSHACC2,
  /* 100 */  OCAML_PUSHCONST0,
  /* 101 */  OCAML_PUSHACC3,
  /* 102 */  OCAML_C_CALL4, 8,
  /* 104 */  OCAML_ACC0,
  /* 105 */  OCAML_RETURN, 3,
  /* 107 */  OCAML_RESTART,
  /* 108 */  OCAML_GRAB, 1,
  /* 110 */  OCAML_CONST0,
  /* 111 */  OCAML_PUSHACC2,
  /* 112 */  OCAML_C_CALL1, 0,
  /* 114 */  OCAML_OFFSETINT_1B, (opcode_t) -1,
  /* 116 */  OCAML_PUSH,
  /* 117 */  OCAML_PUSHACC2,
  /* 118 */  OCAML_GTINT,
  /* 119 */  OCAML_BRANCHIF_1B, 18,
  /* 121 */  OCAML_CHECK_SIGNALS,
  /* 122 */  OCAML_ACC1,
  /* 123 */  OCAML_PUSHACC4,
  /* 124 */  OCAML_GETBYTESCHAR,
  /* 125 */  OCAML_PUSHACC3,
  /* 126 */  OCAML_APPLY1,
  /* 127 */  OCAML_ACC1,
  /* 128 */  OCAML_PUSH,
  /* 129 */  OCAML_OFFSETINT_1B, 1,
  /* 131 */  OCAML_ASSIGN, 2,
  /* 133 */  OCAML_ACC1,
  /* 134 */  OCAML_NEQ,
  /* 135 */  OCAML_BRANCHIF_1B, (opcode_t) -14,
  /* 137 */  OCAML_CONST0,
  /* 138 */  OCAML_RETURN, 4,
  /* 140 */  OCAML_RESTART,
  /* 141 */  OCAML_GRAB, 2,
  /* 143 */  OCAML_ACC2,
  /* 144 */  OCAML_PUSHACC2,
  /* 145 */  OCAML_PUSHACC2,
  /* 146 */  OCAML_PUSHGETFLASHGLOBAL_1B, 3,
  /* 148 */  OCAML_APPLY1,
  /* 149 */  OCAML_PUSHGETFLASHGLOBAL_1B, 4,
  /* 151 */  OCAML_APPLY3,
  /* 152 */  OCAML_PUSHGETFLASHGLOBAL_1B, 5,
  /* 154 */  OCAML_APPTERM1, 4,
  /* 156 */  OCAML_RESTART,
  /* 157 */  OCAML_GRAB, 1,
  /* 159 */  OCAML_ACC1,
  /* 160 */  OCAML_PUSHACC1,
  /* 161 */  OCAML_PUSHGETFLASHGLOBAL_1B, 6,
  /* 163 */  OCAML_APPLY2,
  /* 164 */  OCAML_PUSHGETFLASHGLOBAL_1B, 5,
  /* 166 */  OCAML_APPTERM1, 3,
  /* 168 */  OCAML_ACC0,
  /* 169 */  OCAML_C_CALL1, 9,
  /* 171 */  OCAML_RETURN, 1,
  /* 173 */  OCAML_GETFLASHGLOBAL_1B, 7,
  /* 175 */  OCAML_PUSHCONST0,
  /* 176 */  OCAML_C_CALL1, 10,
  /* 178 */  OCAML_PUSH,
  /* 179 */  OCAML_PUSHCONST1,
  /* 180 */  OCAML_PUSHGETFLASHGLOBAL_1B, 8,
  /* 182 */  OCAML_APPLY2,
  /* 183 */  OCAML_PUSHACC2,
  /* 184 */  OCAML_PUSHGETFLASHGLOBAL_1B, 9,
  /* 186 */  OCAML_APPLY2,
  /* 187 */  OCAML_ASSIGN, 1,
  /* 189 */  OCAML_BRANCH_1B, 19,
  /* 191 */  OCAML_CHECK_SIGNALS,
  /* 192 */  OCAML_CONST0,
  /* 193 */  OCAML_C_CALL1, 10,
  /* 195 */  OCAML_ASSIGN, 0,
  /* 197 */  OCAML_ACC0,
  /* 198 */  OCAML_PUSHCONST1,
  /* 199 */  OCAML_PUSHGETFLASHGLOBAL_1B, 8,
  /* 201 */  OCAML_APPLY2,
  /* 202 */  OCAML_PUSHACC2,
  /* 203 */  OCAML_PUSHGETFLASHGLOBAL_1B, 9,
  /* 205 */  OCAML_APPLY2,
  /* 206 */  OCAML_ASSIGN, 1,
  /* 208 */  OCAML_ACC0,
  /* 209 */  OCAML_BRANCHIFNOT_1B, 10,
  /* 211 */  OCAML_ACC0,
  /* 212 */  OCAML_BEQ_1B, 10, 7,
  /* 215 */  OCAML_ACC0,
  /* 216 */  OCAML_BNEQ_1B, 13, (opcode_t) -25,
  /* 219 */  OCAML_ACC1,
  /* 220 */  OCAML_C_CALL1, 0,
  /* 222 */  OCAML_OFFSETINT_1B, (opcode_t) -1,
  /* 224 */  OCAML_PUSHCONST0,
  /* 225 */  OCAML_PUSHACC3,
  /* 226 */  OCAML_PUSHGETFLASHGLOBAL_1B, 10,
  /* 228 */  OCAML_APPTERM3, 6,
  /* 230 */  OCAML_ACC0,
  /* 231 */  OCAML_C_CALL1, 11,
  /* 233 */  OCAML_RETURN, 1,
  /* 235 */  OCAML_ACC0,
  /* 236 */  OCAML_PUSH,
  /* 237 */  OCAML_CLOSURE_1B, 0, (opcode_t) -7,
  /* 240 */  OCAML_PUSHGETFLASHGLOBAL_1B, 11,
  /* 242 */  OCAML_APPTERM2, 3,
  /* 244 */  OCAML_CONST0,
  /* 245 */  OCAML_RETURN, 1,
  /* 247 */  OCAML_CHECK_SIGNALS,
  /* 248 */  OCAML_GETFLASHGLOBAL_1B, 13,
  /* 250 */  OCAML_PUSHGETFLASHGLOBAL_1B, 12,
  /* 252 */  OCAML_GETFIELD1,
  /* 253 */  OCAML_APPLY1,
  /* 254 */  OCAML_CONSTINT_2B, 7, 208,
  /* 257 */  OCAML_PUSHGETFLASHGLOBAL_1B, 14,
  /* 259 */  OCAML_APPLY1,
  /* 260 */  OCAML_CONST1,
  /* 261 */  OCAML_BRANCHIF_1B, (opcode_t) -14,
  /* 263 */  OCAML_STOP
};

#include </Users/ruiwenwang/Desktop/OMicroB/src/byterun/vm/runtime.c>

PROGMEM void * const ocaml_primitives[OCAML_PRIMITIVE_NUMBER] = {
  /*  0 */  (void *) &caml_ml_string_length,
  /*  1 */  (void *) &caml_create_bytes,
  /*  2 */  (void *) &caml_blit_string,
  /*  3 */  (void *) &caml_string_of_bytes,
  /*  4 */  (void *) &caml_unsafe_string_of_bytes,
  /*  5 */  (void *) &caml_unsafe_bytes_of_string,
  /*  6 */  (void *) &caml_ml_bytes_length,
  /*  7 */  (void *) &caml_blit_bytes,
  /*  8 */  (void *) &caml_fill_bytes,
  /*  9 */  (void *) &caml_delay,
  /* 10 */  (void *) &caml_microbit_serial_read,
  /* 11 */  (void *) &caml_microbit_serial_write,
};
