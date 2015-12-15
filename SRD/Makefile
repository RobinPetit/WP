all:
	pdflatex srd.tex
	/home/poupou/texmf/scripts/glossaries/makeglossaries srd
	#makeindex -s srd.ist -t srd.glg -o srd.gls srd.glo
	pdflatex srd.tex
	evince srd.pdf &
