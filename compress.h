#ifndef FC_COMPRESS_151107_H
#define FC_COMPRESS_151107_H

#define WRITESIZE 50


extern void Compress(const char *org, const char *dest);




static char Get_header_write_method(
                                     uint8_t     longer_code_length,
                                     uint64_t    bigger_occ_number,
                                     uint64_t    n_occurrences,
                                     occlist_t  *occlist,
                                     codelist_t *codelist
                                   );

static uint8_t Get_longer_code_length(codelist_t *list_of_codes);

static void Write_header(
                         occlist_t  *list_of_occ,
                         codelist_t *list_of_codes,
                         uint64_t    occ_table[],
                         uint64_t    n_occurrences,
                         uint16_t    n_characters,
                         FILE       *destfile
                       );

static occlist_t *Get_occurrences_list(
                                        const char *filename,
                                        uint64_t   *occurrences_table,
                                        uint64_t   *n_occurrences,
                                        uint16_t   *n_characters
                                      );

static void Set_occurrences_list(uint64_t t[], occlist_t **list);

static void Set_occurrences_list_data(
                                       occlist_t *list,
                                       uint64_t  *occurences,
                                       uint16_t  *characters
                                     );

static codelist_t *New_codelist(
                                 occlist_t *list,
                                 uint64_t   n_occurrences,
                                 uint16_t   n_characters
                               );

static uint32_t Get_bigger_occurrences_number(occlist_t *list_of_occ);

static char *Get_byte_array_of(uint64_t n, char len);

static void Init_array(char *a, int32_t n);

static void Print_method(char m);

static int Get_length_of_occurrences(char data);

static void Header_write_occurrences_array(
                                            char first_byte,
                                            uint64_t n_occurrences,
                                            occlist_t *list_of_occ,
                                            FILE *dest
                                          );

static void Header_write_occurrences_array_assoc(
                                                  char first_byte,
                                                  uint64_t n_occurrences,
                                                  occlist_t *list_of_occ,
                                                  FILE *dest
                                                );

static void Header_write_codes_array(
                                       char first_byte,
                                       uint64_t n_occurrences,
                                       codelist_t *list_of_codes,
                                       FILE *dest
                                     );

static void Header_write_codes_array_assoc(
                                             char first_byte,
                                             uint64_t n_occurrences,
                                             codelist_t *list_of_codes,
                                             FILE *dest
                                           );


static void Write_body(
                        codelist_t *list_of_codes,
                         uint64_t   n_occurrences,
                         uint16_t   n_characters,
                         FILE      *org,
                         FILE      *dest
                      );


static int Get_code(codelist_t *list_of_codes, char c, char **write);

static void Show_writed_code(char *code, int length);
#endif





  /*
  ** 1er byte :
  **
  **
  ** méthode d'enregistrement
  ** {
  **   sauvegarde des occurrences (array(0 => occ, 1 => occ, ...))
  ** - bit1   : tableau
  ** - bit2   : tableau associatif
  **
  **   sauvegarde des codes (length + code)
  ** - bit3   : tableau
  ** - bit4   : tableau associatif
  **
  ** - bit5   = uint8_t  
  ** - bit6   = uint16_t
  ** - bit7   = uint24_t
  ** - defaut = uint8_t + taille auto
  ** }
  **
  **
  ** méthode d'enregistrement de la longueur du fichier
  ** {
  ** - bit8   = uint32_t
  ** - defaut = uint64_t
  ** }
  **
  ** header : [write method] [file length] [data]
  */



