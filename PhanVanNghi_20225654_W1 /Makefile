SUBDIRS = ex1 ex2

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir all; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: all clean
