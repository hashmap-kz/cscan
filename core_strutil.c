#include "core_strutil.h"

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
