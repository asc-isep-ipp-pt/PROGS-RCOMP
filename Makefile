

push:
	#git remote add origin https://github.com/asc-isep-ipp-pt/PROGS-RCOMP.git
	git init
	git add --all
	git push -u origin master -v --progress

clean:
	(cd C; make clean)
	(cd JAVA; make clean)
