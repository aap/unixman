# UNIX V1-V6 manual

This is a collection of programs and scripts to freshly typeset the old UNIX manuals.
Everything is done from Plan 9.

I reconstructed the ?roff sources of the V1, V2 and V5 manuals
from scans.

For V5, unfortunately the pages for
*glob*, *mkfs*, *mknod*, *reloc*, *restor*, *sa*, *su* (all VIII)
are missing in that scan.
I reconstructed them by making educated guesses,
but ultimately they're my creation.

Another strangeness in the V5 scan is that some pages are dated
earlier than their V4 and V6 counterparts. These are:
*csw*, *fstat*, *link*, *nice*, *seek*, *setgid*, *setuid*, *sleep*, *sync*, *time*, *write* (all II).
*read* (II) is not dated earlier but the content seems to be earlier than V4.

In V2, only *nroff*(I) is missing.

# OUTPUT

The V1-V6 manuals are nroff'ed to html.
V1-V3 also have the intro pages.
V4-V6 have only the manual pages themselves.

The V4-V6 manuals are troff'ed to pdf.

You can find the output here:
[v1](http://squoze.net/UNIX/v1man/)
[v2](http://squoze.net/UNIX/v2man/)
[v3](http://squoze.net/UNIX/v3man/)
[v4](http://squoze.net/UNIX/v4man/)
[v5](http://squoze.net/UNIX/v5man/)
[v6](http://squoze.net/UNIX/v6man/)
or as archives:
[v1.tgz](http://squoze.net/UNIX/v1man.tgz)
[v2.tgz](http://squoze.net/UNIX/v2man.tgz)
[v3.tgz](http://squoze.net/UNIX/v3man.tgz)
[v4.tgz](http://squoze.net/UNIX/v4man.tgz)
[v5.tgz](http://squoze.net/UNIX/v5man.tgz)
[v6.tgz](http://squoze.net/UNIX/v6man.tgz)

# BUILDING

The build process is a bit ugly...but it works.
*nroffv1* - *nroffv6* build html versions of the manual,
they are mostly very identical.
First they are processed into something that would
display correctly on a Teletype 37
and then converted to html by two little programs:
37map to convert the special characters of the tty 37 to unicode,
37html to convert everything to html.

*mktoc* is used for V4-V6 to generate the table of contents,
it's basically a translation of the old *tocrc* to rc.
*troffv4* - *troffv6* then typeset everything to pdf.
*mkpdf* is used to combine the individual sections into one file,
and to generate one file with the complete manual.
The TOC for V1-3 seems to have been done only half-automated.

A few helper programs and scripts are:
*ptx* is a port of the old *ptx* to ANSI C.
*oldtroff* simulates some behaviour of the old troff in Plan 9 troff.
*fixv2* - *fixv6* are used to fix the input somewhat.

# BUGS

Not everything is displayed correctly.
eqn (I) (V5, V6) doesn't look too good with nroff,
it's also missing the Bell Labs logo in the PDF.

Generally it seems that the old troff had
a different conversion between inches and points.
The source does not indicate that this is the case,
nevertheless there's some strangeness going on.
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

As for the nroff output, hyphenated underlined words look terrible
(e.g. goto(I), mount(II) in V2).

# TODO

- convert the tty 37 nroff output to postscript (and pdf)

- try to get more accurate results
