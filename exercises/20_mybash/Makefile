# =============================================
# 主 Makefile
# =============================================

SUBDIRS := mysed myfile mywc mytrans

.PHONY: all $(SUBDIRS) mybash clean

all: mybash

$(SUBDIRS):
	$(MAKE) -C src/$@ OBJDIR=../../obj/$@

mybash: $(SUBDIRS)
	@mkdir -p bin
	$(MAKE) -C src/mybash BINDIR=../../bin OBJDIR=./obj

clean:
	@for d in $(SUBDIRS); do $(MAKE) -C src/$$d OBJDIR=../../obj/$$d clean; done
	$(MAKE) -C src/mybash BINDIR=../../bin OBJDIR=./obj clean
	@rm -rf bin obj
