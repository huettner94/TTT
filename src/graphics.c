/** Graphics for "Train the teacher"
 *
 * @file graphics.c
 * @author Sven Rudolph (9744081)
 * @author Felix Huettner (2295235)
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Glut/glut.h"
#include "graphics.h"
#include "TTT.h"

static GLubyte *image;
static int correct;
static int wrong;
static int lines;
static char *text;
static struct Info *info;
static struct Student *student;
static bool hasAnswer;
static bool finish;
static bool isNegative;

void window(struct Info *i) {
	image = NULL;
	errno = 0;
	info = i;
	correct = 0;
	wrong = 0;
	student = randomStudent(info);
	hasAnswer = false;
	finish = false;
	isNegative = false;
	lines = ((info->maxNameLength+10)>43 ? info->maxNameLength+10 : 43) /27 +1;
	image = malloc((info->height)*(info->width)*((info->bpp)/8)*sizeof(GLubyte));
	text = malloc(((info->maxNameLength+10)>43 ? info->maxNameLength+10 : 43)*sizeof(char));
	if (image == NULL || text == NULL) {
		fprintf(stderr, "Error allocating memory for Image\n");
		errno = -1;
		return;
	}

	sprintf(text, "Name: ");
	atexit(cleanupGraphics);

	if (loadImage() == 0) {
		initDisplay();
		glutDisplayFunc(drawImage);
		glutKeyboardFunc(keyPressed);
		glutMainLoop();
	}

}

void cleanupGraphics() {
	if (image != NULL) {
		free(image);
	}
}

int loadImage() {
	char path[500];
	if (sprintf(path, "%s%c%d.raw", info->ImagePath, DELIMITER, student->matnumber) <0) {
		fprintf(stderr, "Error generating Path\n");
		return -1;
	}

	FILE *f = fopen(path, "rb");
	if (f == NULL) {
		fprintf(stderr, "Error opening student's picture for student %d\n", student->matnumber);
		errno = 1;
		return -1;
	} else {
		if (fread(image, sizeof(GLubyte), info->height*info->width*(info->bpp/8), f) !=
				info->height*info->width*(info->bpp/8)*sizeof(GLubyte)) {
			if (feof(f) != 0) {
				fprintf(stderr, "File of student %d is too short\n", student->matnumber);
				errno = 2;
			}
			if (ferror(f) != 0) {
				fprintf(stderr, "Error reading file of student %d\n", student->matnumber);
				errno = 2;
			}
			return -1;
		} else {
			if (isNegative) {
				makeNegative();
			}
			return 0;
		}
	}
}

void initDisplay() {
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(info->width*2, info->height+lines*15+20);
	glutCreateWindow("TTT");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, info->width*2, 0.0, info->height+lines*15+20);

	glutCreateMenu(menuHandler);
	glutAddMenuEntry("Negative", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void checkName() {
	char name[info->maxNameLength];
	sprintf(name, "%s %s", student->firstname, student->lastname);
	sscanf(text, "Name: %[^\n]", text);
	if (strcmp(name, text) == 0) {
		sprintf(text, "Correct");
		student->correct = true;
		correct++;
	} else {
		sprintf(text, "Wrong: %s", name);
		wrong++;
	}
	hasAnswer = true;
	drawImage();
}

void menuHandler(int entry) {
	if (entry == 1) {
		isNegative = !isNegative;
		makeNegative();
	}
	drawImage();
}

void makeNegative() {
	GLubyte *konverter = image;
	int i = 0;
	int size = (info->height)*(info->width)*((info->bpp)/8);
	while (i < size) {
		konverter[i] = 255 - konverter[i];
		i++;
	}
}

void keyPressed (unsigned char key, int x, int y) {
	if ((key >= 32) && (key <= 126) && !hasAnswer) {
		if (strlen(text) >= info->maxNameLength+6) {
			printf("The is no Student with a name longer than %d Characters\n", info->maxNameLength);
		} else {
			sprintf(text, "%s%c", text, key);
		}
	}
	if (key == 8 && strlen(text) > 6 && !hasAnswer) {
		text[((int) strlen(text))-1] = '\0';
	}
	if (key == 13) {
		if (!finish) {
			if (!hasAnswer) {
				checkName();
			} else {
				if (correct < info->students) {
					student = randomStudent(info);
					if (loadImage() != 0) {
						exit(0);
					}
					sprintf(text, "Name: ");
					hasAnswer = false;
				} else {
					finish = true;
					sprintf(text, "Congratulations! You had %d wrong Answers.", wrong);
				}
			}
		} else {
			exit(0);
		}
	}
	drawImage();
}

void drawImage() {
	int i = 0;
	int j = 0;

	glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glPixelZoom(1.0, -1.0);
	glBitmap(0, 0, 0, 0, info->width*0.5, info->height+lines*15+19, NULL);
	if (info->bpp/8 == 3) {
		glDrawPixels(info->width, info->height, GL_RGB, GL_UNSIGNED_BYTE, image);
	} else {
		glDrawPixels(info->width, info->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
	}
	glBitmap(0, 0, 0, 0, -info->width*0.5, -info->height-lines*15-19, NULL);

	glBegin(GL_LINES); //Border left
	 glVertex2f(info->width*0.5, lines*15+19);
	 glVertex2f(info->width*0.5, info->height+lines*15+19);
	glEnd();
	glBegin(GL_LINES); //Border bottom
	 glVertex2f(info->width*0.5-1, lines*15+19);
	 glVertex2f(info->width*1.5+1, lines*15+19);
	glEnd();
	glBegin(GL_LINES); //Border right
	 glVertex2f(info->width*1.5+1, lines*15+19);
	 glVertex2f(info->width*1.5+1, info->height+lines*15+19);
	glEnd();
	glBegin(GL_LINES); //Border top
	 glVertex2f(info->width*0.5-1, info->height+lines*15+19);
	 glVertex2f(info->width*1.5+1, info->height+lines*15+19);
	glEnd();

	do {
		glRasterPos2f(info->width*0.1, (lines-1-i)*15+10);
		for (j = 0; j < 27 && (i*27+j < (int)strlen(text)); j++) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i*27+j]);
		}
		i++;
	} while (i <= ((int)strlen(text))/27);
	glRasterPos2f(0,0);

	glFlush();
}
