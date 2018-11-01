#include <u.h>
#include <libc.h>
#include <bio.h>

enum { LL = 512 };

Biobuf *bin, *bout;
typedef struct Line Line;
struct Line {
	Rune line[LL];
	char flags[LL];
} *page;
int np;
int lp;
int pp;

int ul, ss;

void
tags(int f)
{
	if(ul != (f&1)){
		ul = f&1;
		Bprint(bout, "<%su>", ul ? "" : "/");
	}
	if(ss != (f&2)){
		ss = f&2;
		Bprint(bout, "<%ss>", ss ? "" : "/");
	}
}

void
writehtml(void)
{
	Line *l;

	ul = 0;
	for(l = &page[0]; l < &page[np]; l++){
		ul = 0;
		for(lp = 0; l->line[lp] != '\0'; lp++){
			tags(l->flags[lp]);
			switch(l->line[lp]){
			case '<':
				Bprint(bout, "&lt;");
				break;
			case '>':
				Bprint(bout, "&gt;");
				break;
			case '&':
				Bprint(bout, "&amp;");
				break;
			default:
				Bputrune(bout, l->line[lp]);
			}
		}
		tags(0);
		Bputc(bout, '\n');
	}
}

Rune
comb(Rune a, Rune b)
{
#define PAIR(A, B) a == A && b == B || a == B && b == A
	if(PAIR('o', '^')) return L'ô';
	if(PAIR('c', '|')) return L'¢';
	if(PAIR('a', '`')) return L'á';
	if(PAIR('=', '/')) return L'≠';
	fprint(2, "warning: overwriting char %C %C\n", a, b);
	return b;
}

void
put(Rune c)
{
	Line *l;
	int p;
	Rune d;

	l = &page[pp];
	/* make sure we have spaces before this character */
	for(p = lp-1; p >= 0; p--)
		if(l->line[p] == '\0')
			l->line[p] = ' ';
		else
			break;

	d = l->line[lp];
	if(c == '_' && d != '\0')
		l->flags[lp] |= 1;
	else if(d == '_'){
		l->flags[lp] |= 1;
		l->line[lp] = c;
	}else if(c == '-' && d != '\0')
		l->flags[lp] |= 2;
	else if(d == '-'){
		l->flags[lp] |= 2;
		l->line[lp] = c;
	}else if(d && c != d){
		/* overstrike */
		if(d == ' ') l->line[lp] = c;
		else if(c == ' ') {}
		else l->line[lp] = comb(d, c);
	}else
		l->line[lp] = c;
	lp++;
}

void
down(void)
{
	pp++;
	if(pp >= np){
		page = realloc(page, (pp+1)*sizeof(Line));
		memset(&page[np], 0, (pp+1-np)*sizeof(Line));
		np = pp+1;
	}
}

void
up(void)
{
	if(pp > 0)
		pp--;
}

void
main()
{
	Rune c;
	int esc;

	bin = Bfdopen(0, OREAD);
	bout = Bfdopen(1, OWRITE);

	page = malloc(sizeof(Line));
	memset(&page[0], 0, sizeof(Line));
	np = 1;
	pp = 0;
	lp = 0;

	esc = 0;
	while(c = Bgetrune(bin), c != Beof){
		if(esc){
			switch(c){
			case '1': /* htab set */	break;
			case '2': /* htab clear */	break;
			case '3': /* red */	break;
			case '4': /* black */	break;
			case '5': /* vtab set */	break;
			case '6': /* vtab clear */	break;
			case '7': /* rev line space */
				up();
				up();
				break;
			case '8': /* half rev line space */
				up();
				break;
			case '9': /* half line space */
				down();
				break;
			case ':': /* full duplex */	break;
			case ';': /* half duplex */	break;
			}
			esc = 0;
		}else switch(c){
		case 033:
			esc = 1;
			break;
		case '\t':
			/* TODO: this is wrong but we have no tab input */
			put(' ');
			break;
		case '\n':
			down();
			down();
			/* fall through */
		case '\r':
			lp = 0;
			break;
		case '\b':
			lp--;
			if(lp < 0) lp = 0;
			if(lp >= LL) lp = LL-1;
			break;
		default:
			put(c);
		}
	}

	writehtml();

	Bterm(bin);
	Bterm(bout);
	exits(nil);
}
