#ifndef FC_DECOMPRESS_221107_H
#define FC_DECOMPRESS_221107_H

extern void Decompress(const char *org, const char *dest);
/*
static codelist_t *GetCodeList(char     **remainder   , char    *tmpheader, 
                              uint16_t   n_characters, FILE    *orgfile  , uint16_t curs);

static void D_WriteBody(codelist_t *codelist, FILE *orgfile  , 
                      FILE     *destfile, char *remainder, uint32_t n_occurrences);

static char CmpCodes(const char *code, const char *s);
static void AddToTab(char **tab, char value, int16_t *size);
static void ArrayCopy(char dest[], const char org[], char value, int16_t *size);
static char *TranslateCodes(codelist_t *codelist, char *binstr, int16_t *size, uint32_t n_occurrences, uint32_t *count);

static void Set_HeaderInfo(char    **tmpheader   , uint32_t *n_occurrences, 
                           uint16_t *n_characters, FILE     *orgfile      , uint16_t *curs);

static void D_Set_CodeList(codelist_t **codelist    , char     *header, 
                         uint16_t   n_characters, uint16_t *curs  );
*/
#endif
