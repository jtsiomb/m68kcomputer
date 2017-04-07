PREFIX = /usr/local
bins = byteweave bin2moncmd

CFLAGS = -pedantic -Wall -g

.PHONY: all
all: $(bins)

byteweave: byteweave.o
	$(CC) -o $@ $^ $(LDFLAGS)

bin2moncmd: bin2moncmd.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(bins)

.PHONY: install
install: $(bins)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(bins) $(DESTDIR)$(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	cd $(DESTDIR)$(PREFIX)/bin && rm -f $(bins)
