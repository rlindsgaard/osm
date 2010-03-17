BASENAME=g5-rapport

default: compile view
compile:
	pdflatex $(BASENAME).tex && pdflatex $(BASENAME).tex

view: compile
	evince $(BASENAME).pdf

print:
	cat $(BASENAME).pdf | ssh ask.diku.dk lpr -P m1a

clean:
	rm $(BASENAME).aux $(BASENAME).log $(BASENAME).toc

