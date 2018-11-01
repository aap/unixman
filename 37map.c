#include <u.h>
#include <libc.h>
#include <bio.h>

Biobuf *bin, *bout;
int greek;
int esc;

Rune
map(Rune c)
{
	static long charmap[] = {
		L'ν', L'α', L'β', L'ω', L'δ', L'Λ', L'Φ', L'Γ',
		L'Ψ', L'τ', L'π', L'ρ', L'λ', L'μ', L'η', L'Θ',
		L'Π', L'ζ', L'Σ', L'ε', L'θ', L'φ', L'ψ', L'Δ',
		L'ξ', L'σ', L'Ω', L'∇', L'γ', L'∂', L'∫', L'¬',
	};
	if(greek){
		if(c < 0100) return c;
		if(c >= 0140) return 0;
		return charmap[c-0100];
	}
	return c;
}

void
main()
{
	Rune c;
	bin = Bfdopen(0, OREAD);
	bout = Bfdopen(1, OWRITE);

	while(c = Bgetrune(bin), c != Beof){
		if(c == 016)
			greek = 1;
		else if(c == 017)
			greek = 0;
		else{
			c = map(c);
			if(c){
				Bputrune(bout, c);
			}
			esc = 0;
		}
	}

	Bterm(bin);
	Bterm(bout);
	exits(nil);
}
