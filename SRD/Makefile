all: srd.pdf

# Faudrait eviter les espaces dans les noms de fichier
srd.pdf: $(wildcard *.tex) $(wildcard Diagrams/*) $(wildcard *.mdj)
	pdflatex srd.tex
	#(${HOME}/texmf/scripts/glossaries/makeglossaries $(subst .tex,,$<) || makeglossaries $(subst .tex,,$<))
	pdflatex srd.tex
	#pdflatex srd.tex

view: srd.pdf
	evince srd.pdf &

clean:
	rm *.aux *.log *.toc 2> /dev/null; return 0
	rm *.glg *.glo *.gls *.xdy *.sbl *.slm *.slg *.ist 2> /dev/null; return 0

cleanbak:
	rm *.default 2> /dev/null; return 0
	rm *~ 2> /dev/null; return 0

mrproper: clean cleanbak
	rm *.pdf; return 0

.PHONY: all clean mrproper clean-bak view
