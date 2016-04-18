# Make or clean EVERY project.  Takes a while!
# Useful to make sure changes to common library aren't breaking previous code.
# If anyone can improve the output, please do.

subdirs=$(shell find * -name atmega88a)
subdirs+=$(shell find * -name atmega168a)
subdirs+=$(shell find * -name atmega32u4)
all:
	@for d in $(subdirs); do $(MAKE) -s -C $$d; done # make each project
	@echo ""
	@echo ""
#	@echo "[------ SUMMARY ------]"
#	@echo ""
#	@for d in $(subdirs); do $(MAKE) -s -C $$d; done # again to get reports in one screen

clean:
	@for d in $(subdirs); do $(MAKE) -s -C $$d $@; done
