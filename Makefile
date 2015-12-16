all:
	make -C SRD/

clean:
	make -C SRD/ clean

cleanbak:
	make -C SRD/ cleanbak

mrproper: clean cleanbak
	make -C SRD/ mrproper

.PHONY: all clean cleanbak mrproper
