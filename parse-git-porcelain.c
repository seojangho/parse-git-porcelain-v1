#include <stdio.h>
#include <stdbool.h>

struct git_status {
	bool branch_ahead;
	bool branch_behind;
	bool branch_diverged;
	bool dirty;
	bool untracked;
	bool added;
	bool modified;
	bool renamed;
	bool deleted;
	bool unmerged;
};

void process_line(int first_char, struct git_status *st);
void process_tracking_info(struct git_status *st);
bool match_tracking_info(const char *condition);
void skip_until_newline(void);
void print_status(struct git_status *st);

void print_status(struct git_status *st) {
	if (st->branch_ahead) {
		printf("A");
	}
	if (st->branch_behind) {
		printf("B");
	}
	if (st->branch_diverged) {
		printf("D");
	}
	if (st->dirty) {
		printf("X");
	} else {
		printf("C");
	}
	if (st->untracked) {
		printf("u");
	}
	if (st->added) {
		printf("a");
	}
	if (st->modified) {
		printf("m");
	}
	if (st->renamed) {
		printf("r");
	}
	if (st->deleted) {
		printf("d");
	}
	if (st->unmerged) {
		printf("n");
	}
	fflush(stdout);
}

int main(void) {
	struct git_status git_status = {0};
	int first_char;
	while ((first_char = getc(stdin)) != EOF) {
		if (first_char == '\n') {
			continue;
		}
		process_line(first_char, &git_status);
	}
	print_status(&git_status);
	return 0;
}

void process_line(int fi, struct git_status *st) {
	int se = getc(stdin);
	if (se == EOF || se == '\n') {
		return;
	}
	if (fi == '#' && se == '#') {
		process_tracking_info(st);
		return;
	}
	st->dirty = true;
	if (fi == '?' && se == '?') {
		st->untracked = true;
		skip_until_newline();
		return;
	}
	if (fi == 'A' && se == ' ') {
		st->added = true;
		skip_until_newline();
		return;
	}
	if (fi == 'M' && se == ' ') {
		st->added = true;
		skip_until_newline();
		return;
	}
	if (fi == 'M' && se == 'M') {
		st->added = true;
		skip_until_newline();
		return;
	}
	if (fi == ' ' && se == 'M') {
		st->modified = true;
		skip_until_newline();
		return;
	}
	if (fi == 'A' && se == 'M') {
		st->modified = true;
		skip_until_newline();
		return;
	}
	if (fi == 'M' && se == 'M') {
		st->modified = true;
		skip_until_newline();
		return;
	}
	if (fi == ' ' && se == 'T') {
		st->modified = true;
		skip_until_newline();
		return;
	}
	if (fi == 'R' && se == ' ') {
		st->renamed = true;
		skip_until_newline();
		return;
	}
	if (fi == ' ' && se == 'D') {
		st->deleted = true;
		skip_until_newline();
		return;
	}
	if (fi == 'D' && se == ' ') {
		st->deleted = true;
		skip_until_newline();
		return;
	}
	if (fi == 'A' && se == 'D') {
		st->deleted = true;
		skip_until_newline();
		return;
	}
	if (fi == 'U' && se == 'U') {
		st->unmerged = true;
		skip_until_newline();
		return;
	}
}

void process_tracking_info(struct git_status *st) {
	int first_char;
	while (true) {
		first_char = getc(stdin);
		if (first_char == EOF || first_char == '\n') {
			return;
		}
		if (first_char == '[') {
			break;
		}
	}
	first_char = getc(stdin);
	if (first_char == EOF || first_char == '\n') {
		return;
	}
	if (first_char == 'a') {
		if (match_tracking_info("ahead")) {
			st->branch_ahead = true;
		}
		return;
	}
	if (first_char == 'b') {
		if (match_tracking_info("behind")) {
			st->branch_behind = true;
		}
		return;
	}
	if (first_char == 'd') {
		if (match_tracking_info("diverged")) {
			st->branch_diverged = true;
		}
		return;
	}
	skip_until_newline();
}

bool match_tracking_info(const char *condition) {
	int index = 1;
	int first_char;
	while (true) {
		first_char = getc(stdin);
		if (first_char == EOF || first_char == '\n') {
			return false;
		}
		if (first_char != condition[index]) {
			skip_until_newline();
			return false;
		}
		index++;
		if (condition[index] == 0) {
			skip_until_newline();
			return true;
		}
	}
}

void skip_until_newline(void) {
	int first_char;
	while (true) {
		first_char = getc(stdin);
		if (first_char == EOF || first_char == '\n') {
			break;
		}
	}
}
