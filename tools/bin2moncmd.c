#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int i, c, execute = 0;
	uint16_t word;
	uint32_t addr = 0x40100;
	FILE *fin = stdin, *fout = stdout;
	char *endp;

	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-' && argv[i][2] == 0) {
			switch(argv[i][1]) {
			case 'a':
				addr = strtol(argv[++i], &endp, 0);
				if(endp == argv[i]) {
					fprintf(stderr, "-a must be followed by an address\n");
					return 1;
				}
				break;

			case 'o':
				if(!(fout = fopen(argv[++i], "wb"))) {
					fprintf(stderr, "failed to open output file %s: %s\n", argv[i], strerror(errno));
					return 1;
				}
				break;

			case 'x':
				execute = 1;
				break;

			default:
				fprintf(stderr, "invalid option: %s\n", argv[i]);
				return 1;
			}
		} else {
			if(fin != stdin) {
				fprintf(stderr, "unexpected argument: %s\n", argv[i]);
				return 1;
			}
			if(!(fin = fopen(argv[i], "rb"))) {
				fprintf(stderr, "failed to open input file %s: %s\n", argv[i], strerror(errno));
				return 1;
			}
		}
	}

	fprintf(fout, "a%06x\n", addr);
	do {
		if((c = fgetc(fin)) == -1) {
			break;
		}
		word = (uint16_t)c << 8;

		if((c = fgetc(fin)) != -1) {
			word |= (uint16_t)c;
		}

		fprintf(fout, "w%04x\n", word);
	} while(c >= 0);
	fprintf(fout, "a%06x\n", addr);

	if(execute) {
		fprintf(fout, "x\n");
	}

	return 0;
}
