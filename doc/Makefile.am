EXTRA_DIST = cxkcdpass.1 cxkcdpass-$(VERSION).html cxkcdpass.info cxkcdpass-$(VERSION).pdf
pandoc_metadata_args = -M "title=CXKCDPASS" -M "header=$(PACKAGE)" -M "footer=$(PACKAGE_STRING)"

# Man pages manual

manfilesdir = $(mandir)/man1
manfiles_DATA = cxkcdpass.1

cxkcdpass.1: manual.md
	pandoc $< -s -t man $(pandoc_metadata_args) -o $@
	echo $(mandir)


# HTML manual

htmlfilesdir = $(htmldir)
htmlfiles_DATA = cxkcdpass-$(VERSION).html

cxkcdpass-$(VERSION).html: manual.md
	pandoc $< -s -t html $(pandoc_metadata_args) -o $@


# Info manual

infofilesdir = $(infodir)
infofiles_DATA = cxkcdpass.info
BUILT_SOURCES = cxkcdpass.texi

cxkcdpass.texi: manual.md
	pandoc $< -s -t texinfo $(pandoc_metadata_args) -o $@
cxkcdpass.info: cxkcdpass.texi
	makeinfo --no-validate --force $< -o $@


# PDF manual

pdffilesdir = $(pdfdir)
pdffiles_DATA = cxkcdpass-$(VERSION).pdf

cxkcdpass-$(VERSION).pdf: manual.md
	pandoc $< -s -t pdf $(pandoc_metadata_args) -o $@
