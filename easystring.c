#include "CB_base.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define STRDEBUG
//{ DECLARATIE
struct struEASYSTR{
	int memspace;
	int len;
	char *val;
	char *subval[10];
};

typedef struct struSTRHEAP* STRHEAP;
struct struSTRHEAP{
	string es;
	STRHEAP next;
};
STRHEAP heap = NULL;
//}
//{SIGNATUREN
static string str_create(char* value);
static string str_createsized(int size);

static string str_dispose(string es);
static char* s_to_cs(string es);
static void str_replace(string es,char* old ,char* new);
static char *buf_replace( char *buf, char* old, char* new);
static void str_insert(string es, int insertpos, char *to_insert);
static SPLITSTRING cstring_split(char *s, char splitbalk);
static SPLITSTRING str_split(string es, char splitbalk);
static void splitstring_dispose(SPLITSTRING sp);
static void str_append(string es1, string es2);
static void str_cs_append(string es, char *cs);
static void str_cs_append(string es, char *cs);
static void str_show(string es);
static void str_set(string es, char* value);
static void str_set_from_file(string *es, char *fName, int fPos, int size_to_read);
static void str_substr(string es, string subes, int start, int len);
static int str_len(string es);
static int offset(string es, int startpos, char *tofind);
static void strClass_quit(void);

//}

void strClass_init(void){
	static predikaat initialized=false;
	if(initialized==false){
		func_in(0);
		initialized=true;

		str.new = str_create;
		str.new_sized = str_createsized;
		str.dispose = str_dispose;
		str.cs = s_to_cs;
		str.cnct = str_append;
		str.cs_cnct = str_cs_append;
		str.repl = str_replace;
		str.ins = str_insert;
		str.buf_repl=buf_replace;
		str.split =str_split;
		str.show=str_show;
		str.set=str_set;
		str.substr=str_substr;
		str.len=str_len;
		str.offset = offset;
		str.set_from_file=str_set_from_file;
		str.cs_split=cstring_split;
		str.splitstring_dispose	= splitstring_dispose;
		str.Class_quit = strClass_quit;
		func_out(0);
	}
 }

static void strClass_quit(void){
}

//{I M P L E M E N T E N
static string str_create(char* value){
	func_in(0);
	string es=NULL;	es = ram.sf_alloc("es1",es,sizeof(string*));
	es->len = strlen(value);
	es->memspace = es->len+1;
	es->val=NULL;		es->val	= ram.sf_alloc("es2",es->val,es->memspace); // '/0' mee reserveren
	ram.sf_memmove("str_create",es->val,0,value,es->len + 1); // '/0' mee kopiëren
	func_out(0);
	return es;
}
static string str_createsized(int size){
	func_in(0);
	string es=NULL;	es = ram.sf_alloc("es1",es,sizeof(string*));
	es->len = 0;
	es->memspace = size;
	es->val=NULL;		es->val	= ram.sf_alloc("es2",es->val,es->memspace); // '/0' mee reserveren
	func_out(0);
	return es;
}
static string str_dispose(string es){
	func_in(0);
	if (es){
		es->val	= ram.sf_free("es2", es->val);
		es = ram.sf_free("es1",es);
	}
	func_out(0);
	return es;

}
static char* s_to_cs(string es){
	return es->val;
}
static void str_replace( string est, char* old, char* new){
	func_in(0);
	//snprintf(trace.msg, trace.msglen, "replace \n\t|%s|\nwith\n\t|%s| \nin \"%s\"", old, new, est->val);
	snprintf(trace.msg, trace.msglen, "replace \n\t|%s|\nwith\n\t|%s|\n", old, new);
	LOGcr(trace.msg,false,0);
	est->val = buf_replace( est->val, old, new);
	est->len=strlen(est->val);
	est->memspace=ram.sf_allocsize_get(est->val);
	func_out(0);
	return;

/*
	char *next, *tmp;
	next = strstr(est->val,old);
	if (next == NULL){
		func_out(0);
		return;
	}

	int 	oldlen	= strlen(old),
			newlen	= strlen(new),
			delta		= oldlen - newlen,
			resizelen;
			//dispsize = 0;


	if (delta < 0){ // newlen > oldlen
		delta = - delta;
		//count
		int Qt = 0;
		tmp = next;
		while(tmp!=NULL){
			Qt++;
			tmp=strstr(tmp+1,old);
		}
		//realloc keeps the original content!!
		resizelen = est->len + ((Qt+1) * delta);
		if(resizelen > est->memspace){
			est->memspace = resizelen;
			if (est->val==NULL)
				est->val = ram.sf_alloc("es3all",est->val,resizelen);
			else
				est->val = ram.sf_realloc("es3",est->val, resizelen);
		}
	}

	while(next!=NULL){
		//dispsize = strlen(next) - oldlen + 1;
		ram.sf_memmove("es20",	est->val,	next - est->val + newlen,	next+oldlen, 	strlen(next) - oldlen + 1 );
		ram.sf_memmove("es21",	est->val,	next - est->val,				new,				newlen);
		next = strstr(next+newlen,old);//In de nieuwe string kan de oude vervat zitten !!
	}
	est->len=strlen(est->val);
	func_out(0);
	*/
};
static void str_append(string es, string es2){
	func_in(0);
	int oldlen=es->len;
	int new_len=oldlen + es2->len;
	if (es->memspace < new_len + 1){
		es->memspace=new_len + 1;
		es->val=ram.sf_realloc("es4", es->val, es->memspace);
	}
	ram.sf_memmove("es23",es->val, oldlen, es2->val, es2->len+1);
	es->len = strlen(es->val);
	func_out(0);
}
static void str_cs_append(string es, char *cs){
	func_in(0);
	int new_len = es->len + strlen(cs);
	if (es->memspace < new_len+1){
		es->memspace=new_len+128;
		es->val=ram.sf_realloc("es4",es->val,es->memspace);
	}
	ram.sf_memmove("es23",es->val,es->len, cs, strlen(cs)+1);
	es->len = strlen(es->val);
	func_out(0);
}

static void str_insert(string es, int insertpos, char *to_insert){
	int inslen = strlen(to_insert);
	int newsize = es->len + inslen + 1;
	int moveslice_len = es->len - insertpos;
	if (es->memspace < newsize){
		es->val = ram.sf_realloc("strins3", es->val, newsize);
		es->memspace = newsize;
	}
	ram.sf_memmove("strins", es->val, insertpos + inslen, es->val + insertpos, moveslice_len);
	ram.sf_memmove("strins2", es->val,insertpos, to_insert, inslen);
	es->len = strlen(es->val);
}
static SPLITSTRING str_split(string es, char splitbalk){
	func_in(0);
	SPLITSTRING sps = cstring_split(es->val, splitbalk);
	func_out(0);
	return sps;
}
static void str_show(string es){


	printf("memspace: %i   length: %i\nvalue: %s\n",es->memspace,es->len,es->val);
}
static void str_set(string es, char* value){
	func_in(0);
	if (es==NULL){
		fLOGcr("UNCREATED STRING %s",value,"",false,0);
		err.program_halt();
		return;
	}
	es->len = strlen(value);
	if (es->memspace < es->len+1){
		es->memspace=es->len+1;
		es->val=ram.sf_realloc("es7",es->val,es->memspace);
	}
	ram.sf_memmove("es24",es->val,0, value, es->len+1);
	func_out(0);
}
static void str_set_from_file(string *esp, char *fName, int fPos, int size_to_read){
	func_in(0);
	FILE *fh;

	fh=fopen(fName,"r");
	if (size_to_read == 0){
		fseek(fh,0,SEEK_END);
		size_to_read = ftell(fh);
	}
	if((*esp==NULL)||(*esp && (*esp)->memspace < size_to_read)){
		str_dispose(*esp);
		*esp = str.new_sized(size_to_read+1);
	}

	fseek(fh,fPos,SEEK_SET);
	size_to_read = fread((*esp)->val, 1, size_to_read, fh);
	fclose(fh);
	(*esp)->len=size_to_read;
	*((*esp)->val + size_to_read)='\0';
	func_out(0);
}
static void str_substr(string es, string subes, int start, int len){
	//places the substring start, len from es into subes, iow subes := substring (es,start,len)
	func_in(0);


	if (start>=es->len) start=es->len;
	if ((start+len)>es->len) len=es->len-start;
	if (subes->memspace<len+1) {
		subes->memspace=len+1;
		subes->val=ram.sf_realloc("es8",subes->val,subes->memspace);
	}
	ram.sf_memmove("es25",subes->val,0, es->val+start, len);
	(*(subes->val+len))='\0';
	subes->len=strlen(subes->val);
	func_out(0);
}

static int str_len(string es){
	return es->len;
}
static int offset(string es, int startpos, char *tofind){
	if (startpos > es->len)
		return -2;
	char *c = strstr(es->val + startpos, tofind);
	if (c)
		return (c - es->val);
	else
		return -1;
	}

static SPLITSTRING cstring_split(char *s, char splitbalk){
	/*
typedef struct splitstring{
	char *buf;
	char **item; // 50 pointers to char* => max 50 items will be splitted.
} SPLITSTRING;

	*/
	func_in(0);
#ifdef STRDEBUG

	char c[2];
	c[0]=splitbalk;c[1]='\0';
	fLOGcr("Splitting string |%s| over [%s]",s,c,false,0);
#endif
	SPLITSTRING spst;
	int	i,
			len = strlen(s)+1;// we nemen de \0 mee
	spst.buf=ram.sf_alloc("cstrsplit1",spst.buf,len);
	ram.sf_memmove("cstrsplit2",spst.buf,0, s,len);
	//-- buf == inface.inf0002.btn_submit.click
	i=1;
	//count splitbalks;
	int splitpower=0;
	char *it, *runner = s;
	while((runner = strchr(runner,splitbalk))!=NULL){
		runner++;
		splitpower++;
	}
	spst.item = ram.sf_alloc("cstrsplit3",spst.item,sizeof(*spst.item) * (splitpower + 1));
	it = runner = spst.buf;
	i=0;
	while ((i < splitpower) && (len > (runner - spst.buf))){
		if (*runner == splitbalk){
			*runner='\0';
			spst.item[i]=it;
			it = runner+1;
			i++;
		}
		runner ++;		//past '\0'
	}
	func_out(0);
	return spst;
}
static void splitstring_dispose(SPLITSTRING sp){


}

static char *buf_replace( char *buf, char* old, char* new){
	func_in(0);

	char *next = strstr(buf,old);
	if (next==NULL){
		func_out(0);

		return buf;
	}
	char *tmp = next;
	int 	oldlen = strlen(old),
			newlen = strlen(new),
			delta = newlen - oldlen,
			buflen = strlen(buf),
			expansie = 0,
			bufsize=ram.sf_allocsize_get(buf);
	if (bufsize<0){
		LOGcr("your trying a buf_replace in a non safe memblock",false, -1);
		func_out(0);

		return buf;
	}
	if (oldlen==0){
		LOGcr("your trying to replace NOTHING?",false, -1);
		func_out(0);
		return buf;
	}
	if (delta > 0){ // bereken benodigde plaats. Is er nog plaats genoeg?
		//count
		int Qt = 0;
		while(tmp!=NULL){
			Qt++;
			tmp=strstr(tmp+1,old);
		}
		expansie = buflen+(Qt*(delta+1));
		//realloc keeps the original content!!
		if(expansie > bufsize){
			LOGcr("reallocating buf...",false,0);
			buf = ram.sf_realloc("es20", buf, expansie);
			bufsize=expansie;
		}

	}
	/*
	//--strstr techniek
	next = strstr(buf,old);
	while(next!=NULL){
		//--maak plaats
		buf=ram.sf_memmove("es21",buf, next - buf + newlen , next + oldlen, strlen(next+oldlen)+1);
		//--schuif in
		buf=ram.sf_memmove("es22",buf, next - buf, new, newlen);

		next = strstr(next+newlen,old); // OPPASSEN VOOR RECURSIEVE AANWEZIGHEID VAN DE ZOEKSTRING!!
	}
	*/
	//-- scan techiek
	next = strchr(buf, *old);
	while(next){
		if (strncmp(next,old,oldlen)==0){
			//--maak plaats
			//-- dest, offset, source, sizeToMove
			buf=ram.sf_memmove("es21",buf, next - buf + newlen , next + oldlen, strlen(next+oldlen)+1);
			//--schuif in
			buf=ram.sf_memmove("es22",buf, next - buf, new, newlen);
			next += newlen;
		}
		else
			next++;

		next = strchr(next, *old);
	}
	func_out(0);
	return buf;

};

//}

