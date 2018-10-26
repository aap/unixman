#include <u.h>
#include <libc.h>
#include <bio.h>

/* flags */
enum
{
	Sub = 1,
	Super = 2,
	Under = 4,
};

Biobuf *bin, *bout;
long chars[1024];
char flags[1024];
int lp;

/* This is not a proper Teletype 37 emulator.
 * It just does what the manual actually uses. */
int
readline(void)
{
	long c;
	int esc;
	int linepos;

	esc = 0;
	linepos = 0;
	lp = 0;
	memset(chars, 0, sizeof(chars));
	memset(flags, 0, sizeof(flags));
	while(c = Bgetrune(bin), c >= 0){
		if(c == 033){
			esc = 1;
			continue;
		}	
		if(esc){
			switch(c){
			case '1': /* htab set */	break;
			case '2': /* htab clear */	break;
			case '3': /* red */	break;
			case '4': /* black */	break;
			case '5': /* vtab set */	break;
			case '6': /* vtab clear */	break;
			case '7': /* rev line space */	break;
			case '8': /* half rev line space */
				linepos--;
				break;
			case '9': /* half line space */
				linepos++;
				break;
			case ':': /* full duplex */	break;
			case ';': /* half duplex */	break;
			}
			esc = 0;
			continue;
		}
		if(c == '\n')
			break;
		switch(c){
		case '\r':
			lp = 0;
			break;
		case '\b':
			lp--;
			if(lp < 0) lp = 0;
			break;
		default:
			if(c == '_' && chars[lp])
				flags[lp++] |= Under;
			else{
				if(linepos < 0)
					flags[lp] |= Super;
				else if(linepos > 0)
					flags[lp] |= Sub;
				chars[lp++] = c;
			}
			break;
		}
	}
	return c >= 0;
}

int sub, sup, u;

void
handletags(int f)
{
	if(f & Sub && !sub){
		Bprint(bout, "<sub>");
		sub = 1;
	}
	if(f & Super && !sup){
		Bprint(bout, "<sup>");
		sup = 1;
	}
	if(f & Under && !u){
		Bprint(bout, "<u>");
		u = 1;
	}
	if(!(f & Under) && u){
		Bprint(bout, "</u>");
		u = 0;
	}
	if(!(f & Super) && sup){
		Bprint(bout, "</sup>");
		sup = 0;
	}
	if(!(f & Sub) && sub){
		Bprint(bout, "</sub>");
		sub = 0;
	}
}

void
htmlprintln(void)
{
	long c;
	sub = 0;
	sup = 0;
	u = 0;
	for(lp = 0; chars[lp]; lp++){
		handletags(flags[lp]);
		c = chars[lp];
		switch(c){
		case '<':
			Bprint(bout, "&lt;");
			break;
		case '>':
			Bprint(bout, "&gt;");
			break;
		default:
			Bputrune(bout, c);
			break;
		}
	}
	handletags(0);
	Bputc(bout, '\n');
}

void
main()
{
	bin = Bfdopen(0, OREAD);
	bout = Bfdopen(1, OWRITE);

	while(readline())
		htmlprintln();

	Bterm(bin);
	Bterm(bout);
	exits(nil);
}
