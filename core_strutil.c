#include "core_strutil.h"

struct strbuilder *pathnormalize(char *where) {
	assert(where);

	size_t slen = strlen(where);
	struct strbuilder *out = sb_new();

	if (slen == 0) {
		sb_adds(out, "./");
		return out;
	}

	int prevc = '\0';
	for (size_t i = 0; where[i]; i++) {
		int c = where[i];
		if (c == '\\' || c == '/') {
			if (prevc == '\\' || prevc == '/') {
				prevc = c;
				continue;
			}
		}
		int nc = (c == '\\' ? '/' : c);
		sb_addc(out, nc);
		prevc = c;
	}

	return out;
}

int strstarts(char *what, char *with) {
	assert(what);
	assert(with);

	size_t L1 = strlen(what);
	size_t L2 = strlen(with);
	if (L1 == 0 || L2 == 0 || (L2 > L1)) {
		return 0;
	}

	for (size_t i = 0; i < L2; i++) {
		int c1 = what[i];
		int c2 = with[i];
		if (c1 != c2) {
			return 0;
		}
	}
	return 1;
}

int strends(char *what, char *with) {
	assert(what);
	assert(with);

	size_t L1 = strlen(what);
	size_t L2 = strlen(with);
	if (L1 == 0 || L2 == 0 || (L2 > L1)) {
		return 0;
	}

	for (ptrdiff_t i = L1, j = L2; --i >= 0 && --j >= 0;) {
		int c1 = what[i];
		int c2 = with[j];
		if (c1 != c2) {
			return 0;
		}
	}
	return 1;
}

struct strbuilder *sb_new() {
	struct strbuilder *rv = malloc(sizeof(struct strbuilder));
	rv->len = 0;
	rv->alloc = 8;
	rv->str = malloc(rv->alloc * sizeof(char));
	rv->str[rv->len] = '\0';
	return rv;
}

static void sb_grow(struct strbuilder *s) {
	s->alloc *= 2;
	s->str = realloc(s->str, s->alloc * sizeof(char));
}

void sb_addc(struct strbuilder *s, char c) {
	if (!c) {
		return;
	}
	if ((s->len + 2) == s->alloc) {
		sb_grow(s);
	}
	s->str[s->len++] = c;
	s->str[s->len] = '\0';
}

void sb_adds(struct strbuilder *s, char *news) {
	if (!news) {
		return;
	}
	for (size_t i = 0; news[i]; i++) {
		sb_addc(s, news[i]);
	}
}

struct strbuilder *sb_copy(struct strbuilder *what) {
	struct strbuilder *res = sb_new();
	sb_adds(res, what->str);
	return res;
}

struct strbuilder *sb_left(struct strbuilder *from, size_t much) {
	assert(from && from->str);

	struct strbuilder *res = sb_new();
	// I) empty one or another.
	if (from->len == 0 || much == 0) {
		return res;
	}
	// II) overflow, return full content of src
	if (much >= from->len) {
		return sb_copy(from);
	}
	// III) normal cases
	for (size_t i = 0; i < much; i++) {
		sb_addc(res, from->str[i]);
	}
	return res;
}

struct strbuilder *sb_right(struct strbuilder *from, size_t much) {
	assert(from && from->str);

	struct strbuilder *res = sb_new();
	// I) empty one or another.
	if (from->len == 0 || much == 0) {
		return res;
	}
	// II) overflow, return full content of src
	if (much >= from->len) {
		return sb_copy(from);
	}
	//III) normal cases
	size_t start = from->len - much;
	for (size_t i = start; i < from->len; i++) {
		sb_addc(res, from->str[i]);
	}
	return res;
}

struct strbuilder *sb_mid(struct strbuilder *from, size_t begin, size_t much) {
	assert(from && from->str);

	struct strbuilder *res = sb_new();
	// I) empty
	if (begin >= from->len) {
		return res;
	}
	// II) overflow, return full content of src from begin to .len
	if (much >= from->len) {
		much = from->len;
	}
	size_t end = begin + much;
	if (end >= from->len) {
		end = from->len;
	}
	for (size_t i = begin; i < end; i++) {
		sb_addc(res, from->str[i]);
	}
	return res;
}

struct strbuilder *sb_trim(struct strbuilder *from) {
	assert(from && from->str);

	struct strbuilder *res = sb_new();
	if (from->len == 0) {
		return res;
	}

	size_t start = 0;
	size_t end = 0;

	for (start = 0; start < from->len; start++) {
		int c = from->str[start];
		if (c > ' ') {
			break;
		}
	}

	for (end = from->len; end != 0; end--) {
		int c = from->str[end];
		if (c > ' ') {
			break;
		}
	}

	for (size_t i = start; i <= end; i++) {
		sb_addc(res, from->str[i]);
	}
	return res;
}

static int strequal(char *what, char *with) {
	return strcmp(what, with) == 0;
}

void sb_forget_update(struct strbuilder *sb) {
	assert(sb);
	assert(sb->str);
	free(sb->str);

	sb->len = 0;
	sb->alloc = 8;
	sb->str = malloc(sb->alloc * sizeof(char));
	assert(sb->str);
	sb->str[0] = '\0';
}

struct strbuilder *sb_replace(struct strbuilder *where, char *what, char *with) {
	assert(where);
	assert(what);
	assert(with);

	struct strbuilder *res = sb_new();
	size_t buflen = where->len;
	size_t what_len = strlen(what);
	size_t with_len = strlen(with);

	// StrReplace("", "a", "b")
	if (buflen == 0) {
		return res;
	}
	// StrReplace("abc", "", "/")       :: abc
	// StrReplace("abc", "abcde", "/")  :: abc
	// StrReplace("abc", "", "")        :: abc
	// XXX: StrReplace("abc", "a", "")  :: bc
	if (what_len == 0 || what_len > buflen
			|| (what_len == 0 && with_len == 0)) {
		return sb_copy(where);
	}

	struct strbuilder *tmp = sb_new();
	for (size_t i = 0; i < buflen; i++) {
		sb_addc(tmp, where->str[i]);
		struct strbuilder *r = sb_right(tmp, what_len);
		if (strequal(r->str, what)) {
			struct strbuilder *l = sb_left(tmp, tmp->len - what_len);
			sb_adds(res, l->str);
			sb_adds(res, with);
			sb_forget_update(tmp);
		}
	}
	if (tmp->len > 0) {
		struct strbuilder *r = sb_right(tmp, what_len);
		if (strequal(r->str, what)) {
			struct strbuilder *l = sb_left(tmp, tmp->len - what_len);
			sb_adds(res, l->str);
			sb_adds(res, with);
			sb_forget_update(tmp);
		} else {
            sb_adds(res, tmp->str);
        }
	}

	return res;
}

