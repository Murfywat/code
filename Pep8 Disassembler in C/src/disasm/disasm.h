#ifndef __DISASSEMBLE_DATA__
#define __DISASSEMBLE_DATA__

/* ************************************************************************* *
 * parse.h                                                                   *
 * -------                                                                   *
 *  Author:   Murphy Cooke                                                   *
 *  Purpose:  Header file for parse.c.                                       *
 * ************************************************************************* */
typedef enum { BLOCK, LINE, ASCII, WORD } SYMTYPE;
#define INVALID_SYM_TYPE -1

typedef enum { A, X } reg_t;                    /* registers */
typedef enum { IMM, DIR, IND, S, SDEF, INDX, SINDX, SINDXDEF} mode_a;    /* addressing modes */
typedef enum {
    STOP, RETTR, MOVSPA, MOVFLGA, BR, BRLE, BRLT, BREQ, BRNE, BRGE, BRGT, BRV,
    BRC, CALL, NOT, NEG, ASL, ASR, ROL, ROR, NOP, DECI, DECO, STRO,
    CHARI, CHARO, RET, ADDSP, SUBSP, ADD, SUB, AND, OR, CP, LD, LDBYTE,
    ST, STBYTE
} inst_t;
#define INVALID_INST_TYPE -1

/*
    Circular  - a memory location needs an instruction specifier and the
    instruction specifier needs a memory location.
*/
struct mem_loc;
typedef struct {
    inst_t instruction;
    char * bytes; /* Holds the hex bytes for an instruction*/
    reg_t regist;
    mode_t mode;
    bool unary;
    bool active;
} inst_spec;

typedef struct mem_loc{
    int16_t intloc;
    uint8_t byte;
    inst_spec spec;
    struct mem_loc * next;
    struct symbol * sym;; /* holds Symbol */
} mem_loc;

struct symbol{
    SYMTYPE type;
    char * symBytes; /* Storage for any key bytes of the Symbol*/
    char * label;
    int16_t addressByte;
    int16_t sizeInt; /* if block or ascii is type, stores the size*/
    struct symbol * next;
};


/* ************************************************************************* *
 * Function prototypes here. Note that variable names are often omitted.     *
 * ************************************************************************* */
int disassemble_data(const char *, mem_loc **, const char *);
inst_spec determine_instr(uint8_t);
inst_t determine_opcode(uint8_t);
void format_disasm_output(const char **, mem_loc **);
void parse_symfile(const char *);
bool is_symbol(int16_t);
struct symbol * get_symbol(int16_t);
void free_program();
/* ************************************************************************* *
 * Externally defined variables here. An externally defined variable is a    *
 * global variable in parse.c that is made to be accessible to parts of the  *
 * program that include this header file. For instance, if you have a global *
 * variable defined as:                                                      *
 *                                                                           *
 *   bool some_global_boolean;                                               *
 *                                                                           *
 * In this file, you would put:                                              *
 *                                                                           *
 *   extern bool some_global_boolean;                                        *
 *                                                                           *
 * This comment should be deleted.                                           *
 * ************************************************************************* */
extern bool symStatus;

#endif
