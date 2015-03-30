all: pandalib pandabin pandapython

pandalib: 
	cd lib; make $(MAKEFLAGS)

pandabin: 
	cd bin; make $(MAKEFLAGS) ; cd ../ ; ln -sf bin/test

pandapython:
	cd pywrap; make $(MAKEFLAGS) ; cd ../

clean:
	cd bin; make clean; cd ../lib; make clean; cd ../pywrap; make clean; cd ../; rm test 
