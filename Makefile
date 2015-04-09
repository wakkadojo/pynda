all: pandalib pandabin pandapython pandapython2

pandalib: 
	cd lib; make $(MAKEFLAGS)

pandabin: 
	cd bin; make $(MAKEFLAGS) ; cd ../ 

pandapython:
	cd pywrap; make $(MAKEFLAGS) ; cd ../

pandapython2:
	cd pywrap2; make $(MAKEFLAGS) ; cd ../

clean:
	cd bin; make clean; cd ../lib; make clean; cd ../pywrap; make clean; cd ../pywrap2; make clean; cd ../
