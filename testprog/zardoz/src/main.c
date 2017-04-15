#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "intr.h"

static void proc_input(char c);
/*static int tokenize_input(void);
static void runcmd(void);
*/

static char input[128];
static short inp_sz, cursor;

int main(void)
{
	int c;

	printf("initializing zardoz v" VERSION "\n");
	init_intr();

	while((c = getchar()) != 4) {
		proc_input(c);
	}

	cleanup_intr();
	return 0;
}

static void proc_input(char c)
{
	static short csi, esc;
	/*
	if(isprint(c)) {
		printf("got char: '%c'\n", c);
	} else {
		printf("got char: %d\n", (int)c);
	}
	*/

	if(csi) {
		switch(c) {
		case 'A':	/* cursor up */
			break;

		case 'B':	/* cursor down */
			break;

		case 'C':	/* cursor right */
			if(cursor < inp_sz) {
				++cursor;
			}
			break;

		case 'D':	/* cursor left */
			if(cursor > 0) {
				--cursor;
			}
			break;

		default:
			break;
		}
		csi = 0;

	} else {

		switch(c) {
		case '\b':
		case 127:
			if(cursor > 0) {
				if(cursor < inp_sz) {
					memmove(input + cursor - 1, input + cursor, inp_sz - cursor);
				}
				--cursor;
				--inp_sz;
				if(c != '\b') {
					putchar(8);		/* echo ^H */
				}
			}
			break;

		case 21:	/* ctrl-U */
			if(cursor > 0) {
				printf("\033[1K");
				printf("\033[0G");
				inp_sz -= cursor;
				memmove(input, input + cursor, inp_sz);
				cursor = 0;
			}
			break;

		case 11:	/* ctrl-k */
			if(cursor < inp_sz) {
				printf("\033[K");
				inp_sz -= inp_sz - cursor;
				cursor = inp_sz;
			}
			break;

		case '\n':
			input[inp_sz] = 0;
			inp_sz = 0;
			cursor = 0;

			/*
			tokenize_input();
			runcmd();
			*/
			printf("CMD: %s\n", input);
			break;

		case 27:
			esc = 1;
			break;

		case '[':
			if(esc) {
				csi = 1;
				esc = 0;
				return;
			}

		default:
			if(cursor < inp_sz) {
				memmove(input + cursor + 1, input + cursor, inp_sz - cursor);
			}
			input[cursor++] = c;
			++inp_sz;
		}
	}

	/*
	{
		int i;
		printf("%d: ", cursor);
		for(i=0; i<inp_sz; i++) {
			if(cursor == i) {
				putchar('|');
			}
			putchar(input[i]);
		}
		if(cursor == i && i > 0) {
			putchar('|');
		}
		putchar('\n');
	}
	*/
}
