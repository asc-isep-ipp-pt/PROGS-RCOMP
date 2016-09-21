
all:
	(cd C; make)
	(cd JAVA; make)

clean:
	(cd C; make clean)
	(cd JAVA; make clean)

