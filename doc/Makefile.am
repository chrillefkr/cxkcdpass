EXTRA_DIST = manfiles htmlfiles infofiles


# Man pages manual

manfilesdir = $(man1dir)
manfiles_DATA = cxkcdpass.1

cxkcdpass.1: manual.md
	pandoc $< -s -t man -o $@


# HTML manual

htmlfilesdir = $(htmldir)
htmlfiles_DATA = cxkcdpass-$(VERSION).html

cxkcdpass-$(VERSION).html: manual.md
	pandoc $< -s -t html -o $@


# Info manual

infofilesdir = $(infodir)
infofiles_DATA = cxkcdpass.info
BUILT_SOURCES = cxkcdpass.texi

cxkcdpass.texi: manual.md
	pandoc $< -s -t texinfo -o $@
cxkcdpass.info: cxkcdpass.texi
	makeinfo --no-validate --force $< -o $@


# PDF manual

pdffilesdir = $(pdfdir)
pdffiles_DATA = cxkcdpass-$(VERSION).pdf

cxkcdpass-$(VERSION).pdf: manual.md
	pandoc $< -s -t pdf -o $@
