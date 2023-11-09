.PHONY: clean_all objective1 objective2 objective4

all: objective1 objective2 objective4

objective1:
	$(MAKE) -C objective1

objective2:
	$(MAKE) -C objective2

objective4:
	$(MAKE) -C objective4

clean_all:
	$(MAKE) -C objective1 clean
	$(MAKE) -C objective2 clean
	$(MAKE) -C objective4 clean