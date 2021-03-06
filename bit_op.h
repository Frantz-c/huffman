#ifndef FC_BITOP_151107_H
#define FC_BITOP_151107_H

#define BIT0   1
#define BIT7   128
#define BIT15  32768
#define BIT23  8388608
#define BIT31  2147483648

void PrintByte(char byte);
void Set_CompressedBytes(char *byte, int16_t bsize, char *ret, int16_t *size, char *rembyte, char *remlen, unsigned char *curs, codelist_t *code);
char *Uint_To_BinStr(uint32_t n, char *n_bytes);
char *Reduce(char *binstr, char *n_bytes);
void Set_FullBinStr(char **full, const char *binstr, const char *rem);
char *GetBytes(char *binstr, uint16_t *size, char get_rem, char *remlen);
char *UintToBinStr(uint32_t n, char n_bytes);
char *Char_To_BinStr(char n);
uint32_t BinStr_To_Uint32(char *binstr);
char *GetBinStr(char *byte, uint16_t size);
char AddBits(char byte, char nbits, int16_t *size, char *rem, char *remlen);
char *Uint32_To_4char(uint32_t n);
char GetNBytes(char *_4bytes);

#endif
