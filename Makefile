all: pandalib pandabin pandapython

pandalib: 
	cd lib; make $(MAKEFLAGS)

pandabin: 
	cd bin; make $(MAKEFLAGS) ; cd ../ ; ln -sf bin/test

pandapython:
	cd python; make $(MAKEFLAGS) ; cd ../

clean:
	cd bin; make clean; cd ../lib; make clean; cd ../python; make clean; cd ../; rm test 
