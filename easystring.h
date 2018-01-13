#ifndef _EASYSTRING
#define _EASYSTRING

typedef struct SPLITSTRING{
	char *buf;
	char **item; // 50 pointers to char* => max 50 items will be splitted.
	int  qt;
} SPLITSTRING;
typedef struct struEASYSTR *string;

struct struEASYSTRCLASS{
	string	 (*new)(char* value);
	string	 (*new_sized)(int size);
	string	 (*dispose)(string es);
	char		*(*cs)(string es);
	void		 (*cnct)(string es1,string es2);
	void		 (*cs_cnct)(string es, char *cs);
	void		 (*ins)(string es, int insertpos, char *to_insert);
	void		 (*repl)(string es, char* old, char* new);
	char		*(*buf_repl)( char *buf, char* old, char* new)	;
	void		 (*show)(string es);
	void		 (*set)(string es, char* value);
	void		 (*set_from_file)(string *es, char *fName, int fPos, int size_to_read);
	void		 (*substr)(string es, string subes, int start, int len);
	int		 (*len)(string es);
	int		 (*offset)(string es, int startpos, char *tofind);

	SPLITSTRING (*cs_split)(char *s, char splitbalk);
	SPLITSTRING (*split)(string es, char delim);
	void (*splitstring_dispose)(SPLITSTRING sp);
	void (*Class_quit)(void);
} str;
void strClass_init(void);

#endif //_EASYSTRING
