#include <stdio.h>
#include <string.h>
#include <errno.h>

enum {
	OP_WEAVE,
	OP_UNWEAVE
};

int weave(FILE *evenfile, FILE *oddfile, FILE *outfile);
int unweave(FILE *infile, FILE *outfile, int skip);

int main(int argc, char **argv)
{
	int i, op = OP_WEAVE, skip = 0, num_fnames = 0;
	const char *fname[3];
	FILE *fp[3];

	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-even") == 0) {
				op = OP_UNWEAVE;
				skip = 0;
			} else if(strcmp(argv[i], "-odd") == 0) {
				op = OP_UNWEAVE;
				skip = 1;
			} else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
				printf("usage: %s <-even|-odd> [input file] [output file]\n", argv[0]);
				printf("       %s <input even> <input odd> [output file]\n", argv[0]);
				return 0;
			} else {
				fprintf(stderr, "invalid option: %s\n", argv[i]);
				return 1;
			}
		} else {
			if(num_fnames >= 3) {
				fprintf(stderr, "unexpected argument: %s\n", argv[i]);
			}
			fname[num_fnames++] = argv[i];
		}
	}

	if(op == OP_WEAVE) {
		if(num_fnames < 2) {
			fprintf(stderr, "you must specify the two files to weave (order: even - odd)\n");
			return 1;
		}
		fp[2] = stdout;	/* default */
		for(i=0; i<num_fnames; i++) {
			if(!(fp[i] = fopen(fname[i], i < 2 ? "rb" : "wb"))) {
				fprintf(stderr, "failed to open file: %s: %s\n", fname[i], strerror(errno));
				return 1;
			}
		}
		weave(fp[0], fp[1], fp[2]);

	} else {	/* OP_UNWEAVE */
		if(num_fnames > 2) {
			fprintf(stderr, "unexpected argument: %s\n", fname[2]);
			return 1;
		}
		fp[0] = stdin;
		fp[1] = stdout;
		for(i=0; i<num_fnames; i++) {
			if(!(fp[i] = fopen(fname[i], i == 0 ? "rb" : "wb"))) {
				fprintf(stderr, "failed to open file: %s: %s\n", fname[i], strerror(errno));
				return 1;
			}
		}
		unweave(fp[0], fp[1], skip);
	}

	for(i=0; i<num_fnames; i++) {
		fclose(fp[i]);
	}
	return 0;
}

int weave(FILE *evenfile, FILE *oddfile, FILE *outfile)
{
	int c0, c1;

	while((c0 = fgetc(evenfile)) != -1) {
		c1 = fgetc(oddfile);
		if(c1 == -1) {
			fprintf(stderr, "warning: even file is larger than odd file\n");
			break;
		}
		fputc(c0, outfile);
		fputc(c1, outfile);
	}

	if(fgetc(oddfile) != -1) {
		fprintf(stderr, "warning: odd file is larger than even file\n");
	}
	return 0;
}

int unweave(FILE *infile, FILE *outfile, int skip)
{
	int c;

	if(skip) {
		fgetc(infile);
	}

	while((c = fgetc(infile)) != -1) {
		fputc(c, outfile);
		fgetc(infile);	/* throw every other byte away */
	}
	return 0;
}
