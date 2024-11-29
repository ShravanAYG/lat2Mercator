#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *db = fopen("world-administrative-boundaries.csv", "r");
	if (!db) {
		perror("Failed to open file");
		return EXIT_FAILURE;
	}

	char *buf = NULL;
	size_t len = 0;

	while (getline(&buf, &len, db) != -1) {
		if (strstr(buf, "IOT")) {
			printf("%s\n", buf);

			char *fs = strchr(buf, ';');
			if (fs) {
				char *ss = strchr(fs + 1, ';');
				if (ss) {
					for (char *p = fs + 1; p < ss; p++) {
						putc(*p, stdout);
					}
					putc('\n', stdout);
				}
			}
		}
	}

	free(buf);
	fclose(db);
	return EXIT_SUCCESS;
}
