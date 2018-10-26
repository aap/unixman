#include <stdio.h>
#include <stdlib.h>
#define _POSIX_SOURCE
#include <unistd.h>
#include <sys/wait.h>

/* V6 ptx */

char	*tfil = "/tmp/p.tmp";
char	*sw[] = {
	"a",
	"an",
	"and",
	"as",
	"for",
	"is",
	"of",
	"on",
	"or",
	"the",
	"to",
	"up",
	NULL};
char	line[200];
int	ch;
int	ptflg;
int	llen = 72;
FILE	*fin;
FILE	*fout;

void pass1(void), pass2(void), p1char(int c);

int getchr(void) { return getc(fin); }
void putchr(char c) { putc(c, fout); }
#define prntf(...) fprintf(fout, __VA_ARGS__)

int
main(int argc, char *argv[])
{
	int f;

	if(argc>1 && *argv[1]=='-') {
		llen = 100;
		ptflg++;
		argc--;
		argv++;
	}
	if(argc<2) {
		printf("arg count\n");
		exit(1);
	}
	fin = fopen(argv[1], "r");
	if(fin == NULL) {
		printf("%s: cannot open\n", argv[1]);
		exit(1);
	}
	fout = fopen(tfil, "w");
	if(fout == NULL) {
		printf("cannot create %s\n", tfil);
		exit(1);
	}
	pass1();
	fclose(fin);
	fclose(fout);

	f = fork();
	if(f < 0) {
		printf("try again\n");
		exit(1);
	}
	if(f == 0){
		execl("/bin/sort", "sort", "-df", "-o", tfil, tfil, NULL);
		exit(1);
	}
	while(wait(NULL) != f);
	fin = fopen(tfil, "r");
	if(fin == NULL) {
		printf("cannot reopen %s\n", tfil);
		exit(1);
	}
	if (argc>=3)
		fout = fopen(argv[2], "w");
	else
		fout = stdout;
	if(fout == NULL) {
		printf("%s: cannot open\n", argv[2]);
		exit(1);
	}
	pass2();

//	unlink(tfil);
	return 0;
}

void
pass1(void)
{
	int n, c, i, ll, j, k, cc, ccc;

loop:
	if ((c=getchr())==EOF)
		return;
	n = 0;
	i = 0;
	while(c!='\n' && c!=EOF) {
		if(c == '(')
			c = 0177;
		if(c==' ' || c=='\t') {
			i++;
			c = getchr();
			continue;
		}
		if(i) {
			i = 0;
			if(n<=llen) line[n++] = ' ';
		}
		if (n<=llen) line[n++] = c;
		c = getchr();
	}
	ll = n;
	line[n++] = 0;
	i = -1;
l1:
	while((cc=line[++i])==' ');
	n = i;
	j = 0;
	while(sw[j]) {
		i = n;
		k = 0;
		while ((cc=sw[j][k++])==line[i++]);
		if(cc==0 && ((ccc=line[--i])==' '||ccc==0))
			goto l1;
		j++;
	}
	i = n;
	while (c=line[n++]) putchr(c);
	putchr('~');
	n = 0;
	while (n<i) {
		c = line[n++];
		if (c!=' ' || n!=i)
			putchr(c);
	}
	putchr('\n');
	while((c=line[i++])!=0 && c!=' ');
	--i;
	if (c) goto l1;
	goto loop;
}

void
pass2(void)
{
	int i, n, c, tilde, llen2, nbfore, nafter;


	llen2 = llen/2+6;
loop:
	if ((c=getchr())==EOF)
		return;
	n = nbfore = nafter = 0;
	tilde = -1;
	while(c!='\n' && c!=EOF) {
		if(c == 0177)
			c = '(';
		if (n<=llen) line[n] = c;
		if (c=='~') tilde = n;
		if (tilde>=0) nafter++; else nbfore++;
		n++;
		c = getchr();
	}
	if (tilde<0)
		tilde = n++;
	nafter--;
	if (nbfore>llen2) {
		i = tilde;
		while (nbfore > llen2)
			while(line[--i]!=' ' && i>=0) nbfore--;
		if (i<0) goto l1;
		line[tilde] = 0200;
		nafter =+ (tilde-i+2);
		tilde = i;
	}
	if (nafter >= llen-llen2) {
		i = tilde;
		while(nafter-- >= llen-llen2)
			while(line[++i]!=' ' && i<n) nafter--;
		if (i>=n) goto l1;
		line[tilde] = 0200;
		nafter++;
		tilde = i;
	}
l1:
	if(!ptflg) {
		for(i=llen-llen2-nafter; i>=8; i =- 8)
			putchr('\t');
		while(--i>=0)
			putchr(' ');
	} else
		prntf(".xx \"");
	i = tilde;
	while (++i<n) p1char(line[i]);
	if(!ptflg)
		prntf("  "); else
		prntf("\" \"");
	i = -1;
	while(++i<tilde) p1char(line[i]);
	if(ptflg)
		putchr('"');
	putchr('\n');
	goto loop;
}

void
p1char(int c)
{
	if ((c&0377) == 0200) {
		putchr('.');
		putchr('.');
		c = '.';
	}
	putchr(c);
}
