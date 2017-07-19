#define MAX_KEYWORDS    20
#define MAX_ERRORCOUNT  10
#define MAX_CONSTANTS   1024
#define MAX_SYMBOLS     1024

#define ERROR_OVERFLOW  1
#define ERROR_SYNTAX    2
#define ERROR_UNDEFINED 3
#define ERROR_DOUBLE    4
#define ERROR_TYPE      5
#define ERROR_INTERNAL  6

#define TYPE_LONG       1
#define TYPE_WORD       2
#define TYPE_BYTE       3
#define OFFSET_AUTO     -1

#define UNDEFINED       -1

typedef struct {
    char *name;
    int value;
} keyword;
extern keyword keywords[];

typedef struct {
    char *value;
    char *label;
} constant;
extern constant constants[];

typedef struct {
    char *label;
    int type;
    int level;
    int size;
    int offset;
} symbol;
extern symbol symbols[];

extern int m_option;

/* scanner.c */
extern void gen_code(char *op, char *opr);
extern void convert(FILE *in, FILE *out);
/* keywords.c */
extern int find_keywords(char *the_name);
extern void setup_keywords();
/* symbols.c */
extern int find_symbols(char *label);
extern int enter_symbols(char *label, int type);
extern void setup_symbols();
extern void frame_start();
extern void frame_end();
extern int frame_size();
extern void frame_cancel();
extern void encode_symbols(FILE *out);
/* constants.c */
extern int find_constants(char *value);
extern int enter_constants(char *value);
extern void setup_constants();
extern void encode_constants(FILE *out);
/* encoder.c */
extern void convert2(FILE *in, FILE *out);
/* misc.c */
extern int char_pos(char *s, char c);
extern void error(int type, char *hint, int lineno);
extern char *new_label();
