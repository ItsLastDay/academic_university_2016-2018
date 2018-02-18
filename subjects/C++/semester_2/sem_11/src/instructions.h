#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#define FOR_INSTRUCTIONS(DO)                                               \
        DO(INVALID, "Invalid instruction.", 1)                          \
        DO(DLOAD, "Load double on TOS, inlined into insn stream.", 9)   \
        DO(ILOAD, "Load int on TOS, inlined into insn stream.", 9)      \
        DO(SLOAD, "Load string reference on TOS, next two bytes - constant id.", 3)   \
        DO(DLOAD0, "Load double 0 on TOS.", 1)                          \
        DO(ILOAD0, "Load int 0 on TOS.", 1)                             \
        DO(SLOAD0, "Load empty string on TOS.", 1)                      \
        DO(DLOAD1, "Load double 1 on TOS.", 1)                          \
        DO(ILOAD1, "Load int 1 on TOS.", 1)                             \
        DO(DLOADM1, "Load double -1 on TOS.", 1)                        \
        DO(ILOADM1, "Load int -1 on TOS.", 1)                           \
        DO(DADD, "Add 2 doubles on TOS, push value back.", 1)           \
        DO(IADD, "Add 2 ints on TOS, push value back.", 1)              \
        DO(DSUB, "Subtract 2 doubles on TOS (lower from upper), push value back.", 1) \
        DO(ISUB, "Subtract 2 ints on TOS (lower from upper), push value back.", 1) \
        DO(DMUL, "Multiply 2 doubles on TOS, push value back.", 1)      \
        DO(IMUL, "Multiply 2 ints on TOS, push value back.", 1)         \
        DO(DDIV, "Divide 2 doubles on TOS (upper to lower), push value back.", 1) \
        DO(IDIV, "Divide 2 ints on TOS (upper to lower), push value back.", 1) \
        DO(IMOD, "Modulo operation on 2 ints on TOS (upper to lower), push value back.", 1) \
        DO(DNEG, "Negate double on TOS.", 1)                            \
        DO(INEG, "Negate int on TOS.", 1)                               \
        DO(IAOR, "Arithmetic OR of 2 ints on TOS, push value back.", 1) \
        DO(IAAND, "Arithmetic AND of 2 ints on TOS, push value back.", 1)  \
        DO(IAXOR, "Arithmetic XOR of 2 ints on TOS, push value back.", 1)  \
        DO(IPRINT, "Pop and print integer TOS.", 1)                     \
        DO(DPRINT, "Pop and print double TOS.", 1)                      \
        DO(SPRINT, "Pop and print string TOS.", 1)                      \
        DO(I2D,  "Convert int on TOS to double.", 1)                    \
        DO(D2I,  "Convert double on TOS to int.", 1)                    \
        DO(S2I,  "Convert string on TOS to int.", 1)                    \
        DO(SWAP, "Swap 2 topmost values.", 1)                           \
        DO(POP, "Remove topmost value.", 1)                             \
        DO(LOADDVAR0, "Load double from variable 0, push on TOS.", 1)   \
        DO(LOADDVAR1, "Load double from variable 1, push on TOS.", 1)   \
        DO(LOADDVAR2, "Load double from variable 2, push on TOS.", 1)   \
        DO(LOADDVAR3, "Load double from variable 3, push on TOS.", 1)   \
        DO(LOADIVAR0, "Load int from variable 0, push on TOS.", 1)      \
        DO(LOADIVAR1, "Load int from variable 1, push on TOS.", 1)      \
        DO(LOADIVAR2, "Load int from variable 2, push on TOS.", 1)      \
        DO(LOADIVAR3, "Load int from variable 3, push on TOS.", 1)      \
        DO(LOADSVAR0, "Load string from variable 0, push on TOS.", 1)   \
        DO(LOADSVAR1, "Load string from variable 1, push on TOS.", 1)   \
        DO(LOADSVAR2, "Load string from variable 2, push on TOS.", 1)   \
        DO(LOADSVAR3, "Load string from variable 3, push on TOS.", 1)   \
        DO(STOREDVAR0, "Pop TOS and store to double variable 0.", 1)    \
        DO(STOREDVAR1, "Pop TOS and store to double variable 1.", 1)    \
        DO(STOREDVAR2, "Pop TOS and store to double variable 0.", 1)    \
        DO(STOREDVAR3, "Pop TOS and store to double variable 3.", 1)    \
        DO(STOREIVAR0, "Pop TOS and store to int variable 0.", 1)       \
        DO(STOREIVAR1, "Pop TOS and store to int variable 1.", 1)       \
        DO(STOREIVAR2, "Pop TOS and store to int variable 0.", 1)       \
        DO(STOREIVAR3, "Pop TOS and store to int variable 3.", 1)       \
        DO(STORESVAR0, "Pop TOS and store to string variable 0.", 1)    \
        DO(STORESVAR1, "Pop TOS and store to string variable 1.", 1)    \
        DO(STORESVAR2, "Pop TOS and store to string variable 0.", 1)    \
        DO(STORESVAR3, "Pop TOS and store to string variable 3.", 1)    \
        DO(LOADDVAR, "Load double from variable, whose 2-byte is id inlined to insn stream, push on TOS.", 3) \
        DO(LOADIVAR, "Load int from variable, whose 2-byte id is inlined to insn stream, push on TOS.", 3) \
        DO(LOADSVAR, "Load string from variable, whose 2-byte id is inlined to insn stream, push on TOS.", 3) \
        DO(STOREDVAR, "Pop TOS and store to double variable, whose 2-byte id is inlined to insn stream.", 3) \
        DO(STOREIVAR, "Pop TOS and store to int variable, whose 2-byte id is inlined to insn stream.", 3) \
        DO(STORESVAR, "Pop TOS and store to string variable, whose 2-byte id is inlined to insn stream.", 3) \
        DO(LOADCTXDVAR, "Load double from variable, whose 2-byte context and 2-byte id inlined to insn stream, push on TOS.", 5) \
        DO(LOADCTXIVAR, "Load int from variable, whose 2-byte context and 2-byte id is inlined to insn stream, push on TOS.", 5) \
        DO(LOADCTXSVAR, "Load string from variable, whose 2-byte context and 2-byte id is inlined to insn stream, push on TOS.", 5) \
        DO(STORECTXDVAR, "Pop TOS and store to double variable, whose 2-byte context and 2-byte id is inlined to insn stream.", 5) \
        DO(STORECTXIVAR, "Pop TOS and store to int variable, whose 2-byte context and 2-byte id is inlined to insn stream.", 5) \
        DO(STORECTXSVAR, "Pop TOS and store to string variable, whose 2-byte context and 2-byte id is inlined to insn stream.", 5) \
        DO(DCMP, "Compare 2 topmost doubles, pushing libc-stryle comparator value cmp(upper, lower) as integer.", 1) \
        DO(ICMP, "Compare 2 topmost ints, pushing libc-style comparator value cmp(upper, lower) as integer.", 1) \
        DO(JA, "Jump always, next two bytes - signed offset of jump destination.", 3) \
        DO(IFICMPNE, "Compare two topmost integers and jump if upper != lower, next two bytes - signed offset of jump destination.", 3) \
        DO(IFICMPE, "Compare two topmost integers and jump if upper == lower, next two bytes - signed offset of jump destination.", 3) \
        DO(IFICMPG, "Compare two topmost integers and jump if upper > lower, next two bytes - signed offset of jump destination.", 3) \
        DO(IFICMPGE, "Compare two topmost integers and jump if upper >= lower, next two bytes - signed offset of jump destination.", 3) \
        DO(IFICMPL, "Compare two topmost integers and jump if upper < lower, next two bytes - signed offset of jump destination.", 3) \
        DO(IFICMPLE, "Compare two topmost integers and jump if upper <= lower, next two bytes - signed offset of jump destination.", 3) \
        DO(DUMP, "Dump value on TOS, without removing it.", 1)        \
        DO(STOP, "Stop execution.", 1)                                  \
        DO(CALL, "Call function, next two bytes - unsigned function id.", 3) \
        DO(CALLNATIVE, "Call native function, next two bytes - id of the native function.", 3)  \
        DO(RETURN, "Return to call location", 1) \
        DO(BREAK, "Breakpoint for the debugger.", 1)

enum InstructionCode {
#define ENUM_ELEM(b, d, l) INST_##b,
    FOR_INSTRUCTIONS(ENUM_ELEM)
#undef ENUM_ELEM
    INST_LAST
};



#endif //__INSTRUCTIONS_H__
