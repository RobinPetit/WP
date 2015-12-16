srd:
	make -C SRD/

srd-clean:
	make -C SRD/ clean

srd-cleanbak:
	make -C SRD/ cleanbak

srd-mrproper: srd-clean srd-cleanbak
	make -C SRD/ mrproper

.PHONY: srd srd-clean srd-cleanbak srd-mrproper
