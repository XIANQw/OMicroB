#define OCAML_STACK_WOSIZE               800
#define OCAML_STATIC_HEAP_WOSIZE         117
#define OCAML_DYNAMIC_HEAP_WOSIZE        683
#define OCAML_FLASH_HEAP_WOSIZE            0
#define OCAML_STACK_INITIAL_WOSIZE         2
#define OCAML_RAM_GLOBDATA_NUMBER          0
#define OCAML_FLASH_GLOBDATA_NUMBER       16
#define OCAML_BYTECODE_BSIZE             361
#define OCAML_PRIMITIVE_NUMBER             6
#define OCAML_VIRTUAL_ARCH                16
#define OCAML_STARTING_OOID                1
#define OCAML_NO_FLASH_HEAP                1
#define OCAML_NO_FLASH_GLOBALS             0

#define OCAML_GC_MARK_AND_COMPACT

#include </home/xian/Projets/M1-S2/PSTL/OMicroB/src/byterun/vm/values.h>

#define OCAML_ACC0                        0
#define OCAML_ACC1                        1
#define OCAML_ACC2                        2
#define OCAML_PUSH                        3
#define OCAML_PUSHACC1                    4
#define OCAML_PUSHACC2                    5
#define OCAML_PUSHACC3                    6
#define OCAML_PUSHACC4                    7
#define OCAML_PUSHACC7                    8
#define OCAML_POP                         9
#define OCAML_ASSIGN                     10
#define OCAML_ENVACC1                    11
#define OCAML_PUSHENVACC1                12
#define OCAML_PUSHENVACC2                13
#define OCAML_APPLY1                     14
#define OCAML_APPLY2                     15
#define OCAML_APPTERM1                   16
#define OCAML_APPTERM2                   17
#define OCAML_RETURN                     18
#define OCAML_RESTART                    19
#define OCAML_GRAB                       20
#define OCAML_CLOSURE_1B                 21
#define OCAML_CLOSUREREC_1B              22
#define OCAML_PUSHOFFSETCLOSURE0         23
#define OCAML_GETFLASHGLOBAL_1B          24
#define OCAML_PUSHGETFLASHGLOBAL_1B      25
#define OCAML_MAKEBLOCK2                 26
#define OCAML_GETFIELD0                  27
#define OCAML_GETFIELD1                  28
#define OCAML_GETFIELD2                  29
#define OCAML_GETFIELD3                  30
#define OCAML_GETFIELD                   31
#define OCAML_GETBYTESCHAR               32
#define OCAML_BRANCH_1B                  33
#define OCAML_BRANCH_2B                  34
#define OCAML_BRANCHIF_1B                35
#define OCAML_BRANCHIFNOT_1B             36
#define OCAML_BOOLNOT                    37
#define OCAML_PUSHTRAP_1B                38
#define OCAML_POPTRAP                    39
#define OCAML_CHECK_SIGNALS              40
#define OCAML_C_CALL1                    41
#define OCAML_CONST0                     42
#define OCAML_CONSTINT_1B                43
#define OCAML_PUSHCONST0                 44
#define OCAML_PUSHCONST2                 45
#define OCAML_PUSHCONSTINT_1B            46
#define OCAML_MULINT                     47
#define OCAML_MODINT                     48
#define OCAML_EQ                         49
#define OCAML_NEQ                        50
#define OCAML_GTINT                      51
#define OCAML_OFFSETINT_1B               52
#define OCAML_STOP                       53

value ocaml_stack[OCAML_STACK_WOSIZE];
value ocaml_ram_heap[OCAML_STATIC_HEAP_WOSIZE + OCAML_DYNAMIC_HEAP_WOSIZE];
value ocaml_ram_global_data[OCAML_RAM_GLOBDATA_NUMBER];

PROGMEM value const ocaml_flash_heap[OCAML_FLASH_HEAP_WOSIZE] = {
};

PROGMEM value const ocaml_initial_static_heap[OCAML_STATIC_HEAP_WOSIZE] = {
  /*   0 */  Make_header(0, 0, Color_white),
  /*   1 */  Make_header(2, Object_tag, Color_white),
  /*   2 */  Val_static_block(&ocaml_ram_heap[5]),
  /*   3 */  Val_int(-1),
  /*   4 */  Make_header(7, String_tag, Color_white),
  /*   5 */  Make_string_data('O', 'u'),
  /*   6 */  Make_string_data('t', '_'),
  /*   7 */  Make_string_data('o', 'f'),
  /*   8 */  Make_string_data('_', 'm'),
  /*   9 */  Make_string_data('e', 'm'),
  /*  10 */  Make_string_data('o', 'r'),
  /*  11 */  Make_string_data('y', '\0'),
  /*  12 */  Make_header(2, Object_tag, Color_white),
  /*  13 */  Val_static_block(&ocaml_ram_heap[16]),
  /*  14 */  Val_int(-3),
  /*  15 */  Make_header(4, String_tag, Color_white),
  /*  16 */  Make_string_data('F', 'a'),
  /*  17 */  Make_string_data('i', 'l'),
  /*  18 */  Make_string_data('u', 'r'),
  /*  19 */  Make_string_data('e', '\0'),
  /*  20 */  Make_header(2, Object_tag, Color_white),
  /*  21 */  Val_static_block(&ocaml_ram_heap[24]),
  /*  22 */  Val_int(-4),
  /*  23 */  Make_header(9, String_tag, Color_white),
  /*  24 */  Make_string_data('I', 'n'),
  /*  25 */  Make_string_data('v', 'a'),
  /*  26 */  Make_string_data('l', 'i'),
  /*  27 */  Make_string_data('d', '_'),
  /*  28 */  Make_string_data('a', 'r'),
  /*  29 */  Make_string_data('g', 'u'),
  /*  30 */  Make_string_data('m', 'e'),
  /*  31 */  Make_string_data('n', 't'),
  /*  32 */  Make_string_data('\0', '\1'),
  /*  33 */  Make_header(2, Object_tag, Color_white),
  /*  34 */  Val_static_block(&ocaml_ram_heap[37]),
  /*  35 */  Val_int(-6),
  /*  36 */  Make_header(9, String_tag, Color_white),
  /*  37 */  Make_string_data('D', 'i'),
  /*  38 */  Make_string_data('v', 'i'),
  /*  39 */  Make_string_data('s', 'i'),
  /*  40 */  Make_string_data('o', 'n'),
  /*  41 */  Make_string_data('_', 'b'),
  /*  42 */  Make_string_data('y', '_'),
  /*  43 */  Make_string_data('z', 'e'),
  /*  44 */  Make_string_data('r', 'o'),
  /*  45 */  Make_string_data('\0', '\1'),
  /*  46 */  Make_header(2, Object_tag, Color_white),
  /*  47 */  Val_static_block(&ocaml_ram_heap[50]),
  /*  48 */  Val_int(-9),
  /*  49 */  Make_header(8, String_tag, Color_white),
  /*  50 */  Make_string_data('S', 't'),
  /*  51 */  Make_string_data('a', 'c'),
  /*  52 */  Make_string_data('k', '_'),
  /*  53 */  Make_string_data('o', 'v'),
  /*  54 */  Make_string_data('e', 'r'),
  /*  55 */  Make_string_data('f', 'l'),
  /*  56 */  Make_string_data('o', 'w'),
  /*  57 */  Make_string_data('\0', '\1'),
  /*  58 */  Make_header(1, Closure_tag, Color_white),
  /*  59 */  Val_codeptr(127),
  /*  60 */  Make_header(1, Closure_tag, Color_white),
  /*  61 */  Val_codeptr(230),
  /*  62 */  Make_header(1, Closure_tag, Color_white),
  /*  63 */  Val_codeptr(121),
  /*  64 */  Make_header(1, Closure_tag, Color_white),
  /*  65 */  Val_codeptr(42),
  /*  66 */  Make_header(1, Closure_tag, Color_white),
  /*  67 */  Val_codeptr(3),
  /*  68 */  Make_header(1, Closure_tag, Color_white),
  /*  69 */  Val_codeptr(155),
  /*  70 */  Make_header(1, Closure_tag, Color_white),
  /*  71 */  Val_codeptr(9),
  /*  72 */  Make_header(5, 0, Color_white),
  /*  73 */  Val_static_block(&ocaml_ram_heap[59]),
  /*  74 */  Val_static_block(&ocaml_ram_heap[79]),
  /*  75 */  Val_static_block(&ocaml_ram_heap[81]),
  /*  76 */  Val_static_block(&ocaml_ram_heap[69]),
  /*  77 */  Val_static_block(&ocaml_ram_heap[83]),
  /*  78 */  Make_header(1, Closure_tag, Color_white),
  /*  79 */  Val_codeptr(132),
  /*  80 */  Make_header(1, Closure_tag, Color_white),
  /*  81 */  Val_codeptr(137),
  /*  82 */  Make_header(1, Closure_tag, Color_white),
  /*  83 */  Val_codeptr(142),
  /*  84 */  Make_header(1, Closure_tag, Color_white),
  /*  85 */  Val_codeptr(253),
  /*  86 */  Make_header(1, Closure_tag, Color_white),
  /*  87 */  Val_codeptr(165),
  /*  88 */  Make_header(1, Closure_tag, Color_white),
  /*  89 */  Val_codeptr(110),
  /*  90 */  Make_header(3, String_tag, Color_white),
  /*  91 */  Make_string_data('S', 'T'),
  /*  92 */  Make_string_data('A', 'R'),
  /*  93 */  Make_string_data('T', '\0'),
  /*  94 */  Make_header(1, Closure_tag, Color_white),
  /*  95 */  Val_codeptr(61),
  /*  96 */  Make_header(8, String_tag, Color_white),
  /*  97 */  Make_string_data('S', 'T'),
  /*  98 */  Make_string_data('A', 'C'),
  /*  99 */  Make_string_data('K', 'O'),
  /* 100 */  Make_string_data('V', 'E'),
  /* 101 */  Make_string_data('R', 'F'),
  /* 102 */  Make_string_data('L', 'O'),
  /* 103 */  Make_string_data('W', 0x0A),
  /* 104 */  Make_string_data('\0', '\1'),
  /* 105 */  Make_header(7, String_tag, Color_white),
  /* 106 */  Make_string_data('O', 'U'),
  /* 107 */  Make_string_data('T', 'O'),
  /* 108 */  Make_string_data('F', 'M'),
  /* 109 */  Make_string_data('E', 'M'),
  /* 110 */  Make_string_data('O', 'R'),
  /* 111 */  Make_string_data('Y', 0x0A),
  /* 112 */  Make_string_data('\0', '\1'),
  /* 113 */  Make_header(3, String_tag, Color_white),
  /* 114 */  Make_string_data('S', 'T'),
  /* 115 */  Make_string_data('O', 'P'),
  /* 116 */  Make_string_data('\0', '\1')
};

PROGMEM value const ocaml_initial_stack[OCAML_STACK_INITIAL_WOSIZE] = {
  /* 0 */  Val_unit,
  /* 1 */  Val_static_block(&ocaml_ram_heap[61])
};

PROGMEM value const ocaml_flash_global_data[OCAML_FLASH_GLOBDATA_NUMBER] = {
  /*  0 */  Val_static_block(&ocaml_ram_heap[2]),
  /*  1 */  Val_static_block(&ocaml_ram_heap[47]),
  /*  2 */  Val_static_block(&ocaml_ram_heap[63]),
  /*  3 */  Val_static_block(&ocaml_ram_heap[65]),
  /*  4 */  Val_static_block(&ocaml_ram_heap[67]),
  /*  5 */  Val_static_block(&ocaml_ram_heap[69]),
  /*  6 */  Val_static_block(&ocaml_ram_heap[71]),
  /*  7 */  Val_static_block(&ocaml_ram_heap[73]),
  /*  8 */  Val_static_block(&ocaml_ram_heap[85]),
  /*  9 */  Val_static_block(&ocaml_ram_heap[87]),
  /* 10 */  Val_static_block(&ocaml_ram_heap[89]),
  /* 11 */  Val_static_block(&ocaml_ram_heap[91]),
  /* 12 */  Val_static_block(&ocaml_ram_heap[95]),
  /* 13 */  Val_static_block(&ocaml_ram_heap[97]),
  /* 14 */  Val_static_block(&ocaml_ram_heap[106]),
  /* 15 */  Val_static_block(&ocaml_ram_heap[114])
};

value acc = Val_static_block(&ocaml_ram_heap[59]);
value env = Val_unit;

#define OCAML_Out_of_memory        Val_static_block(&ocaml_ram_heap[2])
#define OCAML_Failure              Val_static_block(&ocaml_ram_heap[13])
#define OCAML_Invalid_argument     Val_static_block(&ocaml_ram_heap[21])
#define OCAML_Division_by_zero     Val_static_block(&ocaml_ram_heap[34])
#define OCAML_Stack_overflow       Val_static_block(&ocaml_ram_heap[47])

#define OCAML_atom0                Val_static_block(&ocaml_ram_heap[1])

PROGMEM opcode_t const ocaml_bytecode[OCAML_BYTECODE_BSIZE] = {
  /*   0 */  OCAML_BRANCH_2B, 1, 5,
  /*   3 */  OCAML_ACC0,
  /*   4 */  OCAML_C_CALL1, 0,
  /*   6 */  OCAML_RETURN, 1,
  /*   8 */  OCAML_RESTART,
  /*   9 */  OCAML_GRAB, 1,
  /*  11 */  OCAML_CONST0,
  /*  12 */  OCAML_PUSHACC2,
  /*  13 */  OCAML_C_CALL1, 1,
  /*  15 */  OCAML_OFFSETINT_1B, (opcode_t) -1,
  /*  17 */  OCAML_PUSH,
  /*  18 */  OCAML_PUSHACC2,
  /*  19 */  OCAML_GTINT,
  /*  20 */  OCAML_BRANCHIF_1B, 18,
  /*  22 */  OCAML_CHECK_SIGNALS,
  /*  23 */  OCAML_ACC1,
  /*  24 */  OCAML_PUSHACC4,
  /*  25 */  OCAML_GETBYTESCHAR,
  /*  26 */  OCAML_PUSHACC3,
  /*  27 */  OCAML_APPLY1,
  /*  28 */  OCAML_ACC1,
  /*  29 */  OCAML_PUSH,
  /*  30 */  OCAML_OFFSETINT_1B, 1,
  /*  32 */  OCAML_ASSIGN, 2,
  /*  34 */  OCAML_ACC1,
  /*  35 */  OCAML_NEQ,
  /*  36 */  OCAML_BRANCHIF_1B, (opcode_t) -14,
  /*  38 */  OCAML_CONST0,
  /*  39 */  OCAML_RETURN, 4,
  /*  41 */  OCAML_RESTART,
  /*  42 */  OCAML_GRAB, 1,
  /*  44 */  OCAML_ACC0,
  /*  45 */  OCAML_BRANCHIFNOT_1B, 12,
  /*  47 */  OCAML_ACC1,
  /*  48 */  OCAML_PUSHACC1,
  /*  49 */  OCAML_GETFIELD0,
  /*  50 */  OCAML_MAKEBLOCK2, 0,
  /*  52 */  OCAML_PUSHACC1,
  /*  53 */  OCAML_GETFIELD1,
  /*  54 */  OCAML_PUSHOFFSETCLOSURE0,
  /*  55 */  OCAML_APPTERM2, 4,
  /*  57 */  OCAML_ACC1,
  /*  58 */  OCAML_RETURN, 2,
  /*  60 */  OCAML_RESTART,
  /*  61 */  OCAML_GRAB, 1,
  /*  63 */  OCAML_ACC1,
  /*  64 */  OCAML_BRANCHIFNOT_1B, 12,
  /*  66 */  OCAML_ACC1,
  /*  67 */  OCAML_GETFIELD0,
  /*  68 */  OCAML_PUSHACC1,
  /*  69 */  OCAML_APPLY1,
  /*  70 */  OCAML_ACC1,
  /*  71 */  OCAML_GETFIELD1,
  /*  72 */  OCAML_PUSHACC1,
  /*  73 */  OCAML_PUSHOFFSETCLOSURE0,
  /*  74 */  OCAML_APPTERM2, 4,
  /*  76 */  OCAML_RETURN, 2,
  /*  78 */  OCAML_RESTART,
  /*  79 */  OCAML_GRAB, 1,
  /*  81 */  OCAML_ACC1,
  /*  82 */  OCAML_BRANCHIFNOT_1B, 24,
  /*  84 */  OCAML_ACC1,
  /*  85 */  OCAML_GETFIELD1,
  /*  86 */  OCAML_PUSHACC2,
  /*  87 */  OCAML_GETFIELD0,
  /*  88 */  OCAML_PUSH,
  /*  89 */  OCAML_PUSHENVACC2,
  /*  90 */  OCAML_APPLY1,
  /*  91 */  OCAML_BRANCHIFNOT_1B, 10,
  /*  93 */  OCAML_ACC1,
  /*  94 */  OCAML_PUSHACC3,
  /*  95 */  OCAML_PUSHACC2,
  /*  96 */  OCAML_MAKEBLOCK2, 0,
  /*  98 */  OCAML_PUSHOFFSETCLOSURE0,
  /*  99 */  OCAML_APPTERM2, 6,
  /* 101 */  OCAML_ACC1,
  /* 102 */  OCAML_PUSHACC3,
  /* 103 */  OCAML_PUSHOFFSETCLOSURE0,
  /* 104 */  OCAML_APPTERM2, 6,
  /* 106 */  OCAML_ACC0,
  /* 107 */  OCAML_PUSHENVACC1,
  /* 108 */  OCAML_APPTERM1, 3,
  /* 110 */  OCAML_ACC0,
  /* 111 */  OCAML_PUSHGETFLASHGLOBAL_1B, 2,
  /* 113 */  OCAML_CLOSUREREC_1B, 1, 2, (opcode_t) -34,
  /* 117 */  OCAML_CONST0,
  /* 118 */  OCAML_PUSHACC1,
  /* 119 */  OCAML_APPTERM1, 3,
  /* 121 */  OCAML_CONST0,
  /* 122 */  OCAML_PUSHACC1,
  /* 123 */  OCAML_PUSHGETFLASHGLOBAL_1B, 3,
  /* 125 */  OCAML_APPTERM2, 3,
  /* 127 */  OCAML_ACC0,
  /* 128 */  OCAML_C_CALL1, 2,
  /* 130 */  OCAML_RETURN, 1,
  /* 132 */  OCAML_ACC0,
  /* 133 */  OCAML_C_CALL1, 3,
  /* 135 */  OCAML_RETURN, 1,
  /* 137 */  OCAML_ACC0,
  /* 138 */  OCAML_C_CALL1, 4,
  /* 140 */  OCAML_RETURN, 1,
  /* 142 */  OCAML_ACC0,
  /* 143 */  OCAML_PUSHGETFLASHGLOBAL_1B, 4,
  /* 145 */  OCAML_APPLY1,
  /* 146 */  OCAML_PUSHGETFLASHGLOBAL_1B, 5,
  /* 148 */  OCAML_APPTERM1, 2,
  /* 150 */  OCAML_ACC0,
  /* 151 */  OCAML_C_CALL1, 4,
  /* 153 */  OCAML_RETURN, 1,
  /* 155 */  OCAML_ACC0,
  /* 156 */  OCAML_PUSH,
  /* 157 */  OCAML_CLOSURE_1B, 0, (opcode_t) -7,
  /* 160 */  OCAML_PUSHGETFLASHGLOBAL_1B, 6,
  /* 162 */  OCAML_APPTERM2, 3,
  /* 164 */  OCAML_RESTART,
  /* 165 */  OCAML_GRAB, 1,
  /* 167 */  OCAML_ACC1,
  /* 168 */  OCAML_PUSHACC1,
  /* 169 */  OCAML_GTINT,
  /* 170 */  OCAML_BRANCHIFNOT_1B, 5,
  /* 172 */  OCAML_CONST0,
  /* 173 */  OCAML_RETURN, 2,
  /* 175 */  OCAML_ACC1,
  /* 176 */  OCAML_PUSHACC1,
  /* 177 */  OCAML_OFFSETINT_1B, 1,
  /* 179 */  OCAML_PUSHOFFSETCLOSURE0,
  /* 180 */  OCAML_APPLY2,
  /* 181 */  OCAML_PUSHACC1,
  /* 182 */  OCAML_MAKEBLOCK2, 0,
  /* 184 */  OCAML_RETURN, 2,
  /* 186 */  OCAML_ACC0,
  /* 187 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 189 */  OCAML_GETFIELD, 4,
  /* 191 */  OCAML_APPLY1,
  /* 192 */  OCAML_CONSTINT_1B, 44,
  /* 194 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 196 */  OCAML_GETFIELD2,
  /* 197 */  OCAML_APPTERM1, 2,
  /* 199 */  OCAML_ACC0,
  /* 200 */  OCAML_BRANCHIFNOT_1B, 27,
  /* 202 */  OCAML_ACC0,
  /* 203 */  OCAML_GETFIELD1,
  /* 204 */  OCAML_PUSHACC1,
  /* 205 */  OCAML_GETFIELD0,
  /* 206 */  OCAML_PUSHENVACC2,
  /* 207 */  OCAML_PUSHACC1,
  /* 208 */  OCAML_PUSHACC2,
  /* 209 */  OCAML_MULINT,
  /* 210 */  OCAML_GTINT,
  /* 211 */  OCAML_BRANCHIFNOT_1B, 5,
  /* 213 */  OCAML_ACC2,
  /* 214 */  OCAML_RETURN, 3,
  /* 216 */  OCAML_ACC1,
  /* 217 */  OCAML_PUSHACC1,
  /* 218 */  OCAML_PUSHENVACC1,
  /* 219 */  OCAML_APPLY2,
  /* 220 */  OCAML_PUSHOFFSETCLOSURE0,
  /* 221 */  OCAML_APPLY1,
  /* 222 */  OCAML_PUSHACC1,
  /* 223 */  OCAML_MAKEBLOCK2, 0,
  /* 225 */  OCAML_RETURN, 3,
  /* 227 */  OCAML_CONST0,
  /* 228 */  OCAML_RETURN, 1,
  /* 230 */  OCAML_ACC0,
  /* 231 */  OCAML_PUSHGETFLASHGLOBAL_1B, 8,
  /* 233 */  OCAML_CLOSUREREC_1B, 1, 2, (opcode_t) -34,
  /* 237 */  OCAML_ACC1,
  /* 238 */  OCAML_PUSHCONST2,
  /* 239 */  OCAML_PUSHGETFLASHGLOBAL_1B, 9,
  /* 241 */  OCAML_APPLY2,
  /* 242 */  OCAML_PUSHACC1,
  /* 243 */  OCAML_APPTERM1, 3,
  /* 245 */  OCAML_ENVACC1,
  /* 246 */  OCAML_PUSHACC1,
  /* 247 */  OCAML_MODINT,
  /* 248 */  OCAML_PUSHCONST0,
  /* 249 */  OCAML_EQ,
  /* 250 */  OCAML_BOOLNOT,
  /* 251 */  OCAML_RETURN, 1,
  /* 253 */  OCAML_ACC0,
  /* 254 */  OCAML_CLOSURE_1B, 1, (opcode_t) -9,
  /* 257 */  OCAML_PUSHGETFLASHGLOBAL_1B, 10,
  /* 259 */  OCAML_APPTERM1, 2,
  /* 261 */  OCAML_APPLY1,
  /* 262 */  OCAML_GETFLASHGLOBAL_1B, 11,
  /* 264 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 266 */  OCAML_GETFIELD3,
  /* 267 */  OCAML_APPLY1,
  /* 268 */  OCAML_PUSHTRAP_1B, 40,
  /* 270 */  OCAML_CONST0,
  /* 271 */  OCAML_PUSHCONSTINT_1B, 10,
  /* 273 */  OCAML_PUSH,
  /* 274 */  OCAML_PUSHACC2,
  /* 275 */  OCAML_GTINT,
  /* 276 */  OCAML_BRANCHIF_1B, 27,
  /* 278 */  OCAML_CHECK_SIGNALS,
  /* 279 */  OCAML_CONSTINT_1B, 100,
  /* 281 */  OCAML_PUSHACC7,
  /* 282 */  OCAML_APPLY1,
  /* 283 */  OCAML_PUSH,
  /* 284 */  OCAML_PUSH,
  /* 285 */  OCAML_CLOSURE_1B, 0, (opcode_t) -99,
  /* 288 */  OCAML_PUSHGETFLASHGLOBAL_1B, 12,
  /* 290 */  OCAML_APPLY2,
  /* 291 */  OCAML_POP, 1,
  /* 293 */  OCAML_ACC1,
  /* 294 */  OCAML_PUSH,
  /* 295 */  OCAML_OFFSETINT_1B, 1,
  /* 297 */  OCAML_ASSIGN, 2,
  /* 299 */  OCAML_ACC1,
  /* 300 */  OCAML_NEQ,
  /* 301 */  OCAML_BRANCHIF_1B, (opcode_t) -23,
  /* 303 */  OCAML_POP, 2,
  /* 305 */  OCAML_POPTRAP,
  /* 306 */  OCAML_BRANCH_1B, 38,
  /* 308 */  OCAML_PUSHGETFLASHGLOBAL_1B, 1,
  /* 310 */  OCAML_PUSHACC1,
  /* 311 */  OCAML_EQ,
  /* 312 */  OCAML_BRANCHIFNOT_1B, 10,
  /* 314 */  OCAML_GETFLASHGLOBAL_1B, 13,
  /* 316 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 318 */  OCAML_GETFIELD3,
  /* 319 */  OCAML_APPLY1,
  /* 320 */  OCAML_BRANCH_1B, 22,
  /* 322 */  OCAML_GETFLASHGLOBAL_1B, 0,
  /* 324 */  OCAML_PUSHACC1,
  /* 325 */  OCAML_EQ,
  /* 326 */  OCAML_BRANCHIFNOT_1B, 10,
  /* 328 */  OCAML_GETFLASHGLOBAL_1B, 14,
  /* 330 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 332 */  OCAML_GETFIELD3,
  /* 333 */  OCAML_APPLY1,
  /* 334 */  OCAML_BRANCH_1B, 8,
  /* 336 */  OCAML_CONSTINT_1B, 63,
  /* 338 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 340 */  OCAML_GETFIELD2,
  /* 341 */  OCAML_APPLY1,
  /* 342 */  OCAML_POP, 1,
  /* 344 */  OCAML_CONST0,
  /* 345 */  OCAML_C_CALL1, 5,
  /* 347 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 349 */  OCAML_GETFIELD, 4,
  /* 351 */  OCAML_APPLY1,
  /* 352 */  OCAML_GETFLASHGLOBAL_1B, 15,
  /* 354 */  OCAML_PUSHGETFLASHGLOBAL_1B, 7,
  /* 356 */  OCAML_GETFIELD3,
  /* 357 */  OCAML_APPLY1,
  /* 358 */  OCAML_POP, 2,
  /* 360 */  OCAML_STOP
};

#include </home/xian/Projets/M1-S2/PSTL/OMicroB/src/byterun/vm/runtime.c>

PROGMEM void * const ocaml_primitives[OCAML_PRIMITIVE_NUMBER] = {
  /*  0 */  (void *) &caml_string_of_int,
  /*  1 */  (void *) &caml_ml_string_length,
  /*  2 */  (void *) &caml_avr_serial_init,
  /*  3 */  (void *) &caml_avr_serial_read,
  /*  4 */  (void *) &caml_avr_serial_write,
  /*  5 */  (void *) &caml_gc_collections,
};
