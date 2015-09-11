#include <stdio.h>              /* standard I/O */
#include <unistd.h>             /* gives us getopt() */
#include <ctype.h>              /* gives us isprint() */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "disasm.h"
#include "disasm-const.c"
    uint8_t * dataelements;
    FILE *fp;
    int readnumber;
    int returnCode = 0;
    int memoryAddress = 0; /* Keeps track of current mem address, will be hex*/
    size_t filesize = 0;
    size_t i; /*Index Variable*/
    mem_loc * firstLocation = NULL;
    mem_loc * currentLocation = NULL;
    mem_loc * nextLocation = NULL;

    struct symbol * firstSymbol = NULL;
    struct symbol * currentSymbol = NULL;
    struct symbol * nextSymbol = NULL;

    bool operand = false;

int
disassemble_data(const char * filename, mem_loc ** firstMem, const char * symfile)
{

    if(symStatus)
    {
        parse_symfile(symfile);
    }
    fp = fopen(filename, "r");

    if (fp == 0)
    {
        fprintf(stderr, "Hexdump: Could not open file %s\n", filename);
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    uint8_t data[filesize];
    readnumber = fread(data, filesize, filesize, fp);
    if(readnumber != 1)
    {
        fprintf(stderr, "File read error");
    }
    dataelements = data;

    inst_spec currentInstruction;
    for(i = 0; i < filesize; i++)
    {
        if(firstLocation == NULL)
        {
            firstLocation = malloc(sizeof(mem_loc));
            currentLocation = firstLocation;
        }
        else
        {
            currentLocation->next = malloc(sizeof(mem_loc));
            currentLocation = currentLocation->next;
        }
        currentLocation->intloc = memoryAddress;
        currentLocation ->byte = dataelements[i];
        if(!symStatus || !(is_symbol(currentLocation->intloc)))
        {
            currentInstruction = determine_instr(dataelements[i]);
            if(!currentInstruction.unary) /* Unary operand*/
            {
                size_t temp = 0; /* keeps track of how many bytes saved*/
                currentInstruction.bytes = malloc(3 * sizeof(uint8_t *));
                currentInstruction.bytes[0] = dataelements[i];
                memoryAddress++;
                if((i + 1) < filesize)
                {
                    currentInstruction.bytes[1] = dataelements[i + 1];
                    temp++;
                    memoryAddress++;
                    if((i + 2)< filesize)
                    {
                        currentInstruction.bytes[2] = dataelements[i + 2];
                        temp++;
                        memoryAddress++;
                    }
                }
                i += temp;
            }
            else
            {
                currentInstruction.bytes = malloc(sizeof(uint8_t *));
                currentInstruction.bytes[0] = dataelements[i];
                memoryAddress += 1;
            }

            currentLocation->spec = currentInstruction; /* Sets the instruction to the loc*/
            currentLocation->spec.active = true;
        }
        else
        {
            currentLocation->spec.active = false;
            currentSymbol = get_symbol(currentLocation->intloc);

            if(currentSymbol->type == LINE)
            {
                currentInstruction = determine_instr(dataelements[i]);
                if(!currentInstruction.unary) /* Unary operand*/
                {
                    size_t temp = 0; /* keeps track of how many bytes saved*/
                    currentInstruction.bytes = malloc(3 * sizeof(uint8_t *));
                    currentInstruction.bytes[0] = dataelements[i];
                    memoryAddress++;
                    if((i + 1) < filesize)
                    {
                        currentInstruction.bytes[1] = dataelements[i + 1];
                        temp++;
                        memoryAddress++;
                        if((i + 2)< filesize)
                        {
                            currentInstruction.bytes[2] = dataelements[i + 2];
                            temp++;
                            memoryAddress++;
                        }
                    }
                    i += temp;
                }
                else
                {
                    currentInstruction.bytes = malloc(sizeof(uint8_t *));
                    currentInstruction.bytes[0] = dataelements[i];
                    memoryAddress += 1;
                }

                currentLocation->spec = currentInstruction; /* Sets the instruction to the loc*/
            }
            else if(currentSymbol->type == BLOCK)
            {

                    currentSymbol->symBytes = malloc(sizeof(char *) * currentSymbol->sizeInt);

                    int tempIndx = 0;
                    for(tempIndx = 0; tempIndx < currentSymbol->sizeInt; tempIndx++)
                    {
                        currentSymbol->symBytes[tempIndx] = dataelements[i + tempIndx];
                    }
                    i += currentSymbol->sizeInt - 1;
                    memoryAddress += currentSymbol->sizeInt;
            }
            else if(currentSymbol->type == WORD)
            {
                currentSymbol->symBytes = malloc(sizeof(char * ) * currentSymbol->sizeInt);
                currentSymbol->symBytes[0] = dataelements[i];
                currentSymbol->symBytes[1] = dataelements[i + 1];
                i+= 1;
                memoryAddress+= 2;
            }
            else if(currentSymbol->type == ASCII)
            {
                int asciiSize = 1;
                bool nullChar = false;
                int tempIndx = i + 1;
                currentSymbol->symBytes = malloc(sizeof(char *));
                currentSymbol->symBytes[0] = dataelements[i];

                while(!nullChar)
                {
                    asciiSize += 1;
                    currentSymbol->symBytes = realloc(currentSymbol->symBytes,
                                                      sizeof(char *) * asciiSize);
                    currentSymbol->symBytes[asciiSize - 1] = dataelements[tempIndx];

                    if(dataelements[tempIndx] == '\0')
                    {
                        nullChar = true;
                    }
                    tempIndx += 1;

                }
                i += asciiSize - 1;
                memoryAddress += asciiSize;
                currentSymbol->sizeInt = asciiSize;
            }
            else
            {
                memoryAddress+= 1;
            }
            currentLocation->sym = currentSymbol;
        }
    }

    free(currentLocation);
    *firstMem = firstLocation;
    return 0;
}

struct symbol *
get_symbol(int16_t loc)
{
    currentSymbol = firstSymbol;
    struct symbol * returnSymbol = NULL;
    while(currentSymbol != NULL)
    {
        if(loc == currentSymbol->addressByte)
        {
            returnSymbol = currentSymbol;
        }
        currentSymbol = currentSymbol->next;
    }

    return returnSymbol;
}

bool
is_symbol(int16_t loc)
{
    currentSymbol = firstSymbol;
    bool contains = false;

    while(currentSymbol != NULL)
    {
        if(loc == currentSymbol->addressByte)
        {
            contains = true;
        }
        currentSymbol = currentSymbol->next;
    }
    return contains;
}

void
format_disasm_output(const char ** outString, mem_loc ** startAddress)
{
    mem_loc * tempCurrent = NULL;
    tempCurrent = firstLocation;

    while(tempCurrent != NULL)
    {
        printf("%04X ", tempCurrent->intloc);

        if(tempCurrent->spec.active == true || symStatus != true)
        {
            if((tempCurrent->spec.bytes != NULL) && (tempCurrent->spec.unary != true))
            {
                printf(" %02X%02X%02X   ", (uint8_t)tempCurrent->spec.bytes[0],
                       (uint8_t)tempCurrent->spec.bytes[1], (uint8_t)tempCurrent->spec.bytes[2]);
            }
            if ((tempCurrent->spec.bytes != NULL) && (tempCurrent->spec.unary == true))
            {
                printf(" %02X       ", (uint8_t)tempCurrent->spec.bytes[0]);
            }
            printf("      ");
            if(tempCurrent->spec.instruction != INVALID_INST_TYPE)
            {
                if(tempCurrent->spec.regist != -1)
                {
                char tempStr[8];
                strcpy(tempStr, MNEMONICS[tempCurrent->spec.instruction]);
                strcat(tempStr, REGISTERS[tempCurrent->spec.regist]);
                printf("  %-7s", tempStr);
                }
                else
                {
                printf("  %-7s", MNEMONICS[tempCurrent->spec.instruction]);
                }
            }

            if(!tempCurrent->spec.unary) /* Prints operand*/
            {
                int16_t loc = tempCurrent->spec.bytes[2];
                if(is_symbol(loc))
                {
                printf("  %s", get_symbol(tempCurrent->spec.bytes[2])->label);
                }
                else
                {
                printf("  0x%02X%02X", (uint8_t)tempCurrent->spec.bytes[1],
                       (uint8_t)tempCurrent->spec.bytes[2]);
                }
            }
            if(tempCurrent->spec.mode != -1)
            {
            printf(",%s", MODES[tempCurrent->spec.mode]);
            }
            printf("\n");
        }
        else
        {
            if(tempCurrent->sym->type == BLOCK)
            {
                printf(" ");
                for(i = 0; i < tempCurrent->sym->sizeInt; i++)
                {
                    printf("%02X", (uint8_t)tempCurrent->sym->symBytes[i]);
                }
                printf("   ");
            }
            else if(tempCurrent->sym->type == WORD)
            {
                printf(" %02X%02X   ",(uint8_t)tempCurrent->sym->symBytes[0],
                       (uint8_t)tempCurrent->sym->symBytes[1]);
            }
            else if(tempCurrent->sym->type == ASCII)
            {
                printf(" %02X%02X%02X ", (uint8_t)tempCurrent->sym->symBytes[0],
                       (uint8_t)tempCurrent->sym->symBytes[1],
                       (uint8_t)tempCurrent->sym->symBytes[2]);
            }
            else if(tempCurrent->sym->type == LINE)
            {
                printf(" %02X%02X%02X ", (uint8_t)tempCurrent->spec.bytes[0],
                       (uint8_t)tempCurrent->spec.bytes[1],
                       (uint8_t)tempCurrent->spec.bytes[2]);
            }
            else
            {

            }
            char * labelCopy = malloc(sizeof(char *) * strlen(tempCurrent->sym->label));
            strcpy(labelCopy, tempCurrent->sym->label);
            strcat(labelCopy, ":");
            printf(" %-8s", labelCopy);
            free(labelCopy);
            if(tempCurrent->sym->type != LINE)
            {
                printf(" %-7s", SYMBOLTYPES[tempCurrent->sym->type]);

                if(tempCurrent->sym->type == BLOCK)
                {
                    printf("  %d", tempCurrent->sym->sizeInt);
                }
                if(tempCurrent->sym->type == ASCII)
                {
                    printf("  \"");
                    for(i = 0; i < tempCurrent->sym->sizeInt; i++)
                    {

                        if(isprint(tempCurrent->sym->symBytes[i]))
                        {
                            printf("%c", tempCurrent->sym->symBytes[i]);
                        }
                        if(i == tempCurrent->sym->sizeInt - 1)
                        {
                            printf("\\x00\"");
                        }
                    }
                }
            }
            else
            {
                int16_t loc = (tempCurrent->spec.bytes[1] << 8) | tempCurrent->spec.bytes[2];
                printf(" %-7s", MNEMONICS[tempCurrent->spec.instruction]);
                if(!tempCurrent->spec.unary)
                {

                    if(is_symbol(loc))
                    {
                    printf("  %s", get_symbol(tempCurrent->spec.bytes[2])->label);
                    }
                    else
                    {
                    printf("  0x%02X%02X", (uint8_t)tempCurrent->spec.bytes[1],
                           (uint8_t)tempCurrent->spec.bytes[2]);
                    }
                }
                if(tempCurrent->spec.mode != -1)
                {
                printf(",%s", MODES[tempCurrent->spec.mode]);
                }
            }
                if(tempCurrent->sym->type == ASCII && tempCurrent->sym->sizeInt > 3)
                {
                    printf("\n");
                    printf("      ");

                    size_t tempIndx = 3;
                    int byteCount = 0; /* Counts the bytes, 3 per row*/
                    for(tempIndx = 3; tempIndx < tempCurrent->sym->sizeInt; tempIndx++)
                    {
                        if(byteCount == 3)
                        {
                            printf("\n      ");
                            byteCount = 0;
                        }
                        printf("%02X", tempCurrent->sym->symBytes[tempIndx]);
                        byteCount += 1;
                    }
                }

            printf("\n");
        }



        tempCurrent = tempCurrent->next;
    }
    free(tempCurrent);
}

void
parse_symfile(const char * sym)
{
    char label[8];
    char type[6];
    int16_t byte;
    int16_t blockSize = -1;
    char buffer[50];
    char *token = NULL;

    FILE *fp = fopen(sym, "r");

    if(fp == 0)
    {
        fprintf(stderr, "Hexdump: Could not open file %s\n", sym);
        exit(1);
    }

    while(fgets (buffer, 50, fp) != NULL)
    {
        token = strtok (buffer, " ");
        strncpy (label, buffer, 7);
        label[7] = '\0'; /* just in case... */

        token = strtok (NULL, " ");
        strncpy (type, token, 5);
        type[5] = '\0';
        SYMTYPE theType = INVALID_SYM_TYPE;

        if(strcmp(token, "line") == 0)
        {
            theType = LINE;
        }
        else if(strcmp(token, "word") == 0)
        {
            theType = WORD;
        }
        else if(strcmp(token, "ascii") == 0)
        {
            theType = ASCII;
        }
        else if(strcmp(token, "block") == 0)
        {
            theType = BLOCK;
            token = strtok(NULL, " ");
            blockSize = atoi (token);

        }
        else
        {
            theType = INVALID_SYM_TYPE;
        }


        token = strtok (NULL, " ");
        byte = atoi (token);

        if(firstSymbol == NULL)
        {
            firstSymbol = malloc(sizeof(struct symbol));
            currentSymbol = firstSymbol;
        }
        else
        {
            currentSymbol->next = malloc(sizeof(struct symbol));
            currentSymbol = currentSymbol->next;
        }

        currentSymbol->addressByte = byte;
        currentSymbol->label = malloc(sizeof(char *) * 8);
        currentSymbol->sizeInt = blockSize;
        size_t j = 0;
        for(j = 0; j < 8; j++)
        {
            currentSymbol->label[j] = label[j];
        }

        currentSymbol->type = theType;

    }
}
inst_spec
determine_instr(uint8_t opByte)
{

    uint8_t op = opByte;
    inst_t opCode = determine_opcode(op);
    inst_spec instruction;
    switch(opCode)
    {
        case STOP:
            instruction.instruction = STOP;
            instruction.unary = true;
            instruction.regist = -1;
            instruction.mode = -1;
            break;
        case RETTR:
            instruction.instruction = RETTR;
            instruction.unary = true;
            instruction.regist = -1;
            instruction.mode = -1;
            break;
        case MOVSPA:
            instruction.instruction = STOP;
            instruction.unary = true;
            instruction.regist = -1;
            instruction.mode = -1;
            break;
        case MOVFLGA:
            instruction.instruction = MOVFLGA;
            instruction.unary = true;
            instruction.regist = -1;
            instruction.mode = -1;
            break;
        case BR:
            instruction.instruction = BR;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRLE:
            instruction.instruction = BRLE;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRLT:
            instruction.instruction = BRLT;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BREQ:
            instruction.instruction = BREQ;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRNE:
            instruction.instruction = BRNE;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRGE:
            instruction.instruction = BRGE;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRGT:
            instruction.instruction = BRGT;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRV:
            instruction.instruction = BRV;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case BRC:
            instruction.instruction = BRC;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case CALL:
            instruction.instruction = CALL;
            instruction.unary = false;
            instruction.mode = op % 2;
            instruction.regist = -1;
            break;
        case NOT:
            instruction.instruction = NOT;
            instruction.unary = false;
            instruction.regist = op % 2;
            instruction.mode = -1;
            break;
        case NEG:
            instruction.instruction = NEG;
            instruction.unary = false;
            instruction.regist = op % 2;
            instruction.mode = -1;
            break;
        case ASL:
            instruction.instruction = ASL;
            instruction.unary = false;
            instruction.regist = op % 2;
            instruction.mode = -1;
            break;
        case ASR:
            instruction.instruction = ASR;
            instruction.unary = false;
            instruction.regist = op % 2;
            instruction.mode = -1;
            break;
        case ROL:
            instruction.instruction = ROL;
            instruction.unary = false;
            instruction.regist = op % 2;
            instruction.mode = -1;
            break;
        case ROR:
            instruction.instruction = ROR;
            instruction.unary = false;
            instruction.regist = op % 2;
            instruction.mode = -1;
            break;
        case NOP:

            if(op <= 0x27)
            {
                instruction.unary = false;
                instruction.mode = op % 8;
                instruction.regist = -1;
                instruction.instruction = NOP;

            }
            else
            {
                instruction.mode = -1;
                instruction.unary = false;
                instruction.regist = -1;
                instruction.instruction = NOP;
            }
            break;
        case DECI:
            instruction.instruction = DECI;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;
            break;
        case DECO:
            instruction.instruction = DECO;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;
            break;
        case STRO:
            instruction.instruction = STRO;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;
            break;
        case CHARI:
            instruction.instruction = CHARI;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;

            break;
        case CHARO:
            instruction.instruction = CHARO;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;

            break;
        case RET:
            instruction.instruction = RET;
            instruction.unary = true;
            instruction.regist = -1;
            instruction.mode = -1;


            break;
        case ADDSP:
            instruction.instruction = ADDSP;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;

            break;
        case SUBSP:
            instruction.instruction = SUBSP;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = -1;

            break;
        case ADD:
            instruction.instruction = ADD;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case SUB:
            instruction.instruction = SUB;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case AND:
            instruction.instruction = AND;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case OR:
            instruction.instruction = OR;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case CP:
            instruction.instruction = CP;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case LD:
            instruction.instruction = LD;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case LDBYTE:
            instruction.instruction = LDBYTE;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case ST:
            instruction.instruction = ST;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;
        case STBYTE:
            instruction.instruction = STBYTE;
            instruction.unary = false;
            instruction.mode = op % 8;
            instruction.regist = (op >> 3) % 2;
            break;

    }
    return instruction;
}
inst_t
determine_opcode(uint8_t op) /* Determines the instruction enum/opcode */
{
    inst_t opCode;
    if(op == 0x00)
    {
        opCode = 0; /* Stop */
    }
    else if(op == 0x01) /* RETTR */
    {
        opCode = 1;
    }
    else if(op == 0x02) /* MOVSPA */
    {
        opCode = 2;
    }
    else if(op == 0x03) /* MOVFLGA */
    {
        opCode = 3;
    }
    else if(op == 0x04 || op == 0x05) /* BR */
    {
        opCode = 4;
    }
    else if(op == 0x06 || op == 0x07) /* BRLE */
    {
        opCode = 5;
    }
    else if(op == 0x08 || op == 0x09) /* BRLT */
    {
        opCode = 6;
    }
    else if(op == 0x0A || op == 0x0B) /* BREQ */
    {
        opCode = 7;
    }
    else if(op == 0x0C || op == 0x0D) /* BRNE */
    {
        opCode = 8;
    }
    else if(op == 0x0E || op == 0x0F) /* BRGE */
    {
        opCode = 9;
    }
    else if(op == 0x10 || op == 0x11) /* BRGT */
    {
        opCode = 10;
    }
    else if(op == 0x12 || op == 0x13) /* BRV */
    {
        opCode = 11;
    }
    else if(op == 0x14 || op == 0x15) /* BRC */
    {
        opCode = 12;
    }
    else if(op == 0x16 || op == 0x17) /* CALL */
    {
        opCode = 13;
    }
    else if(op == 0x18 || op == 0x19) /* NOTr */
    {
        opCode = 14;
    }
    else if(op == 0x1A || op == 0x1B) /* NEGr */
    {
        opCode = 15;
    }
    else if(op == 0x1C || op == 0x1D) /* ASLr */
    {
        opCode = 16;
    }
    else if(op == 0x1E || op == 0x1F) /* ASRr */
    {
        opCode = 17;
    }
    else if(op == 0x20 || op == 0x21) /* ROLr */
    {
        opCode = 18;
    }
    else if(op == 0x22 || op == 0x23) /* RORr */
    {
        opCode = 19;
    }
    else if(op >= 0x24 && op <= 0x27) /* NOP */
    {
        opCode = 20;
    }
    else if(op >= 0x28 && op <= 0x2F) /* NOP */
    {
        opCode = 20;
    }
    else if(op >= 0x30 && op <= 0x37) /* DECI */
    {
        opCode = 21;
    }
    else if(op >= 0x38 && op <= 0x3F) /* DECO */
    {
        opCode = 22;
    }
    else if(op >= 0x40 && op <= 0x47) /* STRO */
    {
        opCode = 23;
    }
    else if(op >= 0x48 && op <= 0x4F) /* CHARI */
    {
        opCode = 24;
    }
    else if(op >= 0x50 && op <= 0x57) /* CHARO */
    {
        opCode = 25;
    }
    else if(op >= 0x58 && op <= 0x5F) /* RETn */
    {
        opCode = 26;
    }
    else if(op >= 0x60 && op <= 0x67) /* ADDSP */
    {
        opCode = 27;
    }
    else if(op >= 0x68 && op <= 0x6F) /* SUBSP */
    {
        opCode = 28;
    }
    else if(op >= 0x70 && op <= 0x7F) /* ADDr */
    {
        opCode = 29;
    }
    else if(op >= 0x80 && op <= 0x8F) /* SUBr */
    {
        opCode = 30;
    }
    else if(op >= 0x90 && op <= 0x9F) /* ANDr */
    {
        opCode = 31;
    }
    else if(op >= 0xA0 && op <= 0xAF) /* ORr */
    {
        opCode = 32;
    }
    else if(op >= 0xB0 && op <= 0xBF) /* CPr */
    {
        opCode = 33;
    }
    else if(op >= 0xC0 && op <= 0xCF) /* LDr */
    {
        opCode = 34;
    }
    else if(op >= 0xD0 && op <= 0xDF) /* LDBYTEr */
    {
        opCode = 35;
    }
    else if(op >= 0xE0 && op <= 0xEF) /* STr */
    {
        opCode = 36;
    }
    else if(op >= 0xF0 && op <= 0xFF) /* STBYTEr */
    {
        opCode = 37;
    }
    else
    {
        printf("invalid value\n");
    }
    return opCode;
}

void
free_program()
{
    currentLocation = firstLocation;
    while(currentLocation != NULL)
    {
        firstLocation = currentLocation->next;
        if(currentLocation->sym != NULL)
        {
            free(currentLocation->sym->symBytes);
            free(currentLocation->sym->label);
            free(currentLocation->sym->next);
            free(currentLocation->sym);
        }
       free(currentLocation->spec.bytes);
       free(currentLocation);
       currentLocation = firstLocation;
    }
}
