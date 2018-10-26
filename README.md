# UNIX V3-V6 manual

This is a collection of programs and scripts to freshly typeset the old UNIX manuals.
Everything is done from Plan 9.

I reconstructed the n/troff sources of the V5 manual from the scan.
Unfortunately the pages for
*glob*, *mkfs*, *mknod*, *reloc*, *restor*, *sa*, *su* (all VIII)
are missing in that scan.
I reconstructed them by making educated guesses,
but ultimately they're my creation.

Another strangeness in the V5 scan is that some pages are dated
earlier than their V4 and V6 counterparts. These are:
*csw*, *fstat*, *link*, *nice*, *seek*, *setgid*, *setuid*, *sleep*, *sync*, *time*, *write* (all II).
*read* (II) is not dated earlier but the content seems to be earlier than V4.

# OUTPUT

The V3-V6 manuals are nroff'ed to html.

The V4-V6 manuals are troff'ed to pdf.

You can find the output here:
[v3](http://squoze.net/UNIX/v3man/)
[v4](http://squoze.net/UNIX/v4man/)
[v5](http://squoze.net/UNIX/v5man/)
[v6](http://squoze.net/UNIX/v6man/)

# BUILDING

The build process is a bit ugly...but it works.
*nroffv3* - *nroffv6* build html versions of the manual.
First they are processed into something that would
display correctly on a Teletype 37
and then converted to html by two little programs:
37map to convert the special characters of the tty 37 to unicode,
37html to convert everything to html.

For the nroff output I made changes to *naa* so it doesn't
paginate and called the new files *naa_x*.

*mktoc* is used for V4-V6 to generate the table of contents,
it's basically a translation of the old *tocrc* to rc.
*troffv4* - *troffv6* then typeset everything to pdf.
*mkpdf* is used to combine the individual sections into one file,
and to generate one file with the complete manual.

A few helper programs and scripts are:
*ptx* is a port of the old *ptx* to ANSI C.
*fixli* and *fixta* convert some semantics of
told troff to plan 9 troff (.li and .ta requests).
*fixv3* - *fixv6* are used to fix the input somewhat.

# BUGS

Not everything is displayed correctly.
Currently eqn (I) does not render correctly when nroff'ed
because the 37html program is too primitive to fully emulate
a tty 37.

Eqn (I) is also a problem in troff as it's missing the bell labs logo.

Generally it seems that the old troff had
a different conversion between inches and points.
The default font size should be 10pt and the default
line length should be 6.5in,
yet lines come out too long with that setting.
Also tab stops are not quite right either, the font is too small.
This can be observed in (V5)
*cref*, *prof*, *roff*, *write*, *nroff*, *ps* (I),
*getc* (III),
*dn*, *kl* (IV),
*lc* (VI),
*dump*, *lpd* (VIII).

I will have to look into the troff source of different versions
(v7 and plan 9) and compare how things are done.
