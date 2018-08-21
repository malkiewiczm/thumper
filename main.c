#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static void _fatal(const char *msg, const char *fname, const int line) {
	fprintf(stderr, "fatal: %s (%s:%d)\n", msg, fname, line);
	exit(1);
}

#define fatal(what)(_fatal(what, __FILE__, __LINE__))

static void print_usage(const char *bin_name) {
	printf("usage: %s <input file> <output file>\n", bin_name);
	exit(0);
}

static FILE *fopen_s(const char *fname, const char *mode) {
	FILE *f = fopen(fname, mode);
	if (f == NULL) {
		fprintf(stderr, "fname = %s\n", fname);
		fatal("cannot open file");
	}
	return f;
}

static int trim(char *const orig_src, char *const orig_dst) {
	char *src = orig_src;
	char *dst = orig_dst;
	while (*src) {
		if (*src <= ' ') {
			++src;
		} else {
			break;
		}
	}
	while (*src) {
		*dst = *src;
		++dst;
		++src;
	}
	do {
		*dst = '\0';
		--dst;
		--src;
	} while (src != orig_src && *src <= ' ');
	int res =  dst - orig_dst + 1;
	return (res > 0) ? res : 0;
}

static char *self_trim(char *start) {
	while (*start) {
		if (*start <= ' ') {
			++start;
		} else {
			break;
		}
	}
	char *i;
	for (i = start; *i; ++i);
	for ( ; i != start; --i) {
		if (*i > ' ') {
			break;
		}
		*i = '\0';
	}
	return start;
}

static void process(char *src, char **a, char **b) {
	char *mid;
	for (mid = src; *mid; ++mid) {
		if (*mid == '=')
			break;
	}
	if (*mid == '\0')
		fatal("no equal sign?");
	*mid = '\0';
	*a = self_trim(src);
	*b = self_trim(mid + 1);
}

#define BUF_SIZE 1024

//#define outp stdout

int main(int argc, char **argv) {
	if (argc == 0) {
		print_usage("thumper");
	}
	if (argc <= 2) {
		print_usage(argv[0]);
	}
	FILE *inp = fopen_s(argv[1], "r");
	FILE *outp = fopen_s(argv[2], "w");
	char * const buf = malloc(BUF_SIZE);
	char * const line = malloc(BUF_SIZE);
	bool needs_close = false;
	while (fgets(buf, BUF_SIZE, inp)) {
		int amt = trim(buf, line);
		if (amt >= 2 && line[0] == '%') {
			bool header;
			if (line[1] == '@') {
				header = true;
			} else if (line[1] == '%') {
				header = false;
			} else {
				fatal("bad control signal");
				header = false;
			}
			char *a;
			char *b;
			process(line + 2, &a, &b);
			if (header) {
				if (needs_close)
					fprintf(outp, "</p>");
				fprintf(outp, "<p><a href=\"%s\">%s</a>\n", b, a);
			} else {
				fprintf(outp, "&nbsp;&nbsp;<a href=\"%s\">(%s)</a>\n", b, a);
			}
			needs_close = true;
		} else {
			if (needs_close)
				fprintf(outp, "</p>");
			fputs(buf, outp);
			needs_close = false;
		}
	}
	free(buf);
	free(line);
}
