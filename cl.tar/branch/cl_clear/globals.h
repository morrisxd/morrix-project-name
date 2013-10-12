
#define MAXSYMLEN 126
typedef struct node {
	int id;
	char name[1024];
} treenode;

extern char g_cur_sym[];

typedef struct _variable {
	char var_name[128];
	int namespace; /* global or function locally */
	int storage_class_specifier; /* extern, auto */
	int type_specifier; /* char, int, etc ... */
	int type_qualifier; /* const, volitail */
} variable;

extern int lineno;
extern int in_typedef;
extern int external_func_decl;
extern int in_namelist;
extern int after_type_specifiers;
