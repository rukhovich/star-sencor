#
# Makefile for unican and canmon
#

export
include Make.vars

###################################################################

all: chai chai-startup

chai:
	sh -c 'cd src; make  -C /lib/modules/`uname -r`/build M=`pwd` SUBDIRS=`pwd` modules' 
	mv src/unican.ko lib/unican.ko
	$(MAKE) -C src
	$(MAKE) -C src clean
	$(MAKE) -C ex

chai-startup:
	cat ./src/chai-startup.header > ./bin/chai
	echo -n "CHAIVER=" >> ./bin/chai
	echo "\"$(CHAIVER)\"" >> ./bin/chai
	sh -c 'echo "CHAIBASE=`pwd`" >> ./bin/chai'
	echo -n "CBISA_PARAM=" >> ./bin/chai
	echo "\"$(CBISA_PARAM)\"" >> ./bin/chai
	cat ./src/chai-startup.template >> ./bin/chai
	chmod +x ./bin/chai

include Make.install

include Make.uninstall

clean:
	$(MAKE) -C src cleanall
	$(MAKE) -C ex clean 
	rm -f ./bin/chai
	rm -f ./*~ ./doc/*~ ./include/*~

rebuild:
	$(MAKE) -f Makefile clean
	$(MAKE) -f Makefile chai
	$(MAKE) -f Makefile chai-startup
