all: pandalib pandabin

pandalib: 
	cd lib; make $(MAKEFLAGS)

pandabin: 
	cd bin; make $(MAKEFLAGS) ; cd ../ ; ln -sf bin/test

clean:
	cd bin; make clean; cd ../; cd lib; make clean; rm test
