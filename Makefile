# Top level -*- Makefile -*- for torque-launch

ARCHES = $(patsubst config/%.inc,%,$(wildcard config/*.inc))

default:
	@echo
	@echo Available configurations:
	@echo $(ARCHES)
	@echo

$(ARCHES) :
	mkdir -p Obj_$@
	make -C Obj_$@ -f ../config/Common.mk ARCH=$@

clean:
	rm -rf Obj_* perf.data* torque-launch

spotless: clean
	rm -f *~ */*~

.SUFFIXES:

.PHONY: default clean spotless
