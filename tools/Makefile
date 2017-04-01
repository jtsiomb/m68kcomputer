PREFIX = /usr/local
bins = byteweave

CFLAGS = -pedantic -Wall -g

byteweave: byteweave.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o byteweave

.PHONY: install
install: $(bins)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(bins) $(DESTDIR)$(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	cd $(DESTDIR)$(PREFIX)/bin && rm -f $(bins)
