#ifndef __MNEMONIC_STRINGS__
#define __MNEMONIC_STRINGS__
const char * REGISTERS[] = {"A", "X" };                 /* registers */
const char * SYMBOLTYPES[] = {".BLOCK", ".LINE", ".ASCII", ".WORD"};
const char * MODES[] = {"i", "d", "n", "s", "sf", "x", "sx", "sxf"};
const char * MNEMONICS[] = {
    "STOP", "RETTR", "MOVSPA", "MOVFLGA", "BR", "BRLE", "BRLT", "BREQ",
    "BRNE", "BRGE", "BRGT", "BRV", "BRC", "CALL", "NOT", "NEG", "ASL",
    "ASR", "ROL", "ROR", "NOP", "DECI", "DECO", "STRO", "CHARI","CHARO",
    "RET", "ADDSP", "SUBSP", "ADD", "SUB", "AND", "OR", "CP", "LD",
    "LDBYTE", "ST", "STBYTE"
};
#endif
