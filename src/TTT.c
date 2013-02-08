/** Main file "Train the teacher"
 *
 * @file TTT.c
 * @author Sven Rudolph (9744081)
 * @author Felix Huettner (2295235)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "TTT.h"
#include "graphics.h"
#include "Glut/glut.h"

static struct Info info;

int getImagePath() {
	errno = 0;
	fgets(info.ImagePath, 200, info.InfoFile);
	info.ImagePath[strcspn(info.ImagePath, "\n")] = '\0';
	rewind(info.InfoFile);
	return 0;
}

int file_exists(const char * filename) {
	FILE * file;
	if ((file = fopen(filename, "r")) == NULL ) {
		fclose(file);
		return 0;
	}
	return -1;
}

int isName(const char* name) {
	errno = 0;
	for (int i = 0; i < strlen(name); i++) {
		if (!(((((int) name[i]) >= 32) && (((int) name[i]) <= 122)))) {
			errno = -1;
		}
	}
	return errno;
}

struct Student* randomStudent() {
	int number;
	do {
	number = rand() % (info.students);
	} while(info.student[number].correct);
	return &info.student[number];
}

int countStudents() {
	errno = 0;
	int counter = 0;
	char s[100];
	char *p;
	bool hasNL;
	bool hasCounted;
	while (fgets(s, 100, info.InfoFile) != NULL ) { //count the lines of the InfoFile
		if (ferror(info.InfoFile) != 0) {
			fprintf(stderr, "Error reading configuration-file\n");
			errno = -1;
			return -1;
		}
		hasNL = false;
		hasCounted = false;
		p = s;
		while (*p != '\0') {
			if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r' && !hasCounted) {
				hasCounted = true;
			}
			if (*p == '\n') {
				hasNL = true;
			}
			p++;
		}
		if (hasCounted && hasNL) {
			counter++;
		}
	}
	rewind(info.InfoFile);					//rewind InfoFile for further use
	info.students = counter - 2;//substract 2 from the line count for the first two non-student lines in the InfoFile
	return errno;
}

int getStudents() {
	errno = 0;
	char s[100];
	info.maxNameLength = 0;

	if (info.students <= 0) {
		fprintf(stderr, "No Students in Inputfile");
		errno = -1;
		return -1;
	}

	info.student = malloc(info.students * sizeof(struct Student));
	if (info.student == NULL ) {
		fprintf(stderr, "Error allocating memory for students");
		errno = -1;
		return -1;
	}

	for (int i = 0; i < 2; i++) {
		fgets(s, 100, info.InfoFile);
		if (feof(info.InfoFile) != 0 || ferror(info.InfoFile) != 0) {
			errno = -1;
			return -1;
		}
	}

	for (int i = 0; i < info.students; i++) {
		fscanf(info.InfoFile, "%d%*1c %199[^;]%*c %199[^\n]",
				&info.student[i].matnumber, info.student[i].firstname,
				info.student[i].lastname);
		if (ferror(info.InfoFile) != 0) {
			errno = -1;
			return -1;
		}
		info.student[i].correct = false;
		if (!(((info.student[i].matnumber > 0)
				&& (info.student[i].matnumber < 10000))
				&& (isName(info.student[i].firstname) == 0)
				&& (isName(info.student[i].lastname) == 0))) {
			errno = i + 1;
			break;
		}
		if (strlen(info.student[i].firstname)+strlen(info.student[i].lastname)+1 > info.maxNameLength) {
			info.maxNameLength = strlen(info.student[i].firstname)+strlen(info.student[i].lastname)+1;
		}
	}
	return errno;
}

int getImageInfo() {
	errno = 0;
	char s[100];
	fgets(s, 100, info.InfoFile);
	if (fscanf(info.InfoFile, "%d %d %d %s\n", &info.width, &info.height, &info.bpp, info.format) == 4) {
		if (!((info.height > 0) && (info.height < 10000) && (info.width > 0)
				&& (info.width < 10000)
				&& ((info.bpp == 8) || (info.bpp == 24)))) {
			errno = -1;
		}
		if (strcmp(info.format, "raw") != 0) {
			fprintf(stderr, "Incompatible fileformat %s. Please convert to raw\n", info.format);
			errno = -2;
		}
	} else {
		errno = -1;
	}
	if (feof(info.InfoFile) != 0 || ferror(info.InfoFile) != 0) {
		errno = -1;
	}

	rewind(info.InfoFile);
	return errno;
}

int getInfoFile() {

	errno = 0;

	info.InfoFile = fopen(info.InfoFilePath, "r");
	if (info.InfoFile == NULL ) {
		errno = -1;
	}

	return errno;
}

void cleanupMain() {
	if (prefix != NULL ) {
		free(prefix);
	}
	if (info.InfoFilePath != NULL ) {
		free(info.InfoFilePath);
	}
	if (info.student != NULL ) {
		free(info.student);
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	atexit(cleanupMain);

	prefix = NULL;
	info.InfoFilePath = NULL;
	info.student = NULL;
	srand(time(NULL));
	prefix = malloc(sizeof(char)*500);

	if (argc == 2) {
		if (prefix != NULL) {
			getcwd(prefix, 500);
			info.InfoFilePath = (char*) malloc(((unsigned) strlen(prefix)) + 100);
			if (info.InfoFilePath != NULL) {
				if (file_exists(argv[1])) {
					sprintf(info.InfoFilePath, "%s", argv[1]);
				} else {
					sprintf(info.InfoFilePath, "%s%c%s", prefix, DELIMITER, argv[1]);
				}
				if (file_exists(info.InfoFilePath)) {
					if (getInfoFile(&info) == 0) {
						if (getImageInfo(&info) == 0) {
							getImagePath(&info);
							if (countStudents(&info) == 0) {
								if (getStudents(&info) == 0) {
									window(&info);
								} else {
									fprintf(stderr, "Error reading students from file %s on student %d\n", info.InfoFilePath, errno);
								}
							}

						} else {
							if (errno == -1) {
								fprintf(stderr, "Error reading image info from file %s\n", info.InfoFilePath);
							}
						}

						fclose(info.InfoFile);

					} else {
						fprintf(stderr, "Error opening info file %s:\n", info.InfoFilePath);
					}
				} else {
					printf("File %s does not exist\n", info.InfoFilePath);
				}
			}
		} else {
			fprintf(stderr, "Error allocating memory\n");
		}
	} else {
		printf("Usage: %s <Path to configuration-file>\n", argv[0]);
	}

	if (errno != 0) {
		printf("Press any key to continue\n");
		char s[1];
		gets(s);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
