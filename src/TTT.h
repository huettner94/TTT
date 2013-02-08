/** Main headerfile "Train the teacher"
 *
 * @file TTT.h
 * @author Sven Rudolph (9744081)
 * @author Felix Huettner (2295235)
 */

#ifndef TTT_H_
#define TTT_H_

#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32							//
#define DELIMITER '\\'					//Changes the delimiter in file paths
#else									//For Unix-based OS it is '/'
#define DELIMITER '/'					//For DOS-based OS it is '\\'
#endif									//

char * prefix;							//Contains the absolute path to the program(max 500 characters), if there are no paths to the images or to the InfoFile given, then they are searched in this location

struct Student{
	char firstname[200];				//First name of student(max 50 characters)
	char lastname[200];					//Last name of student(max 50 characters)
	int matnumber;						//Matrikulation number of student
	bool correct;						//Saves if guessed correctly
};

struct Info{
		FILE * InfoFile; 				//File pointer to InfoFile
		int width;		 				//Width of student picture
		int height;						//Height of student picture
		int bpp;						//Bit per Pixel
		struct Student * student; 		//Array of structure student, contains info to every student in the InfoFile
		int students;					//Number of students in the InfoFile
		int maxNameLength;				//Maximum length of the Name
		char ImagePath[200];			//Path to images
		char format[10];				//Image format(RAW) of the student picture(max 10 characters)
		char * InfoFilePath;			//Absolute Path to InfoFile(max 100 characters)
};


/** Extracts the path to the main directory from the path to the configuration-file
 * @return 0
 */
int getImagePath();

/** Tests if a file exists
 * @param[in] filename The filename
 * @return 0 if the file exists, -1 if not
 */
int file_exists(const char * filename);

/** Tests if the given String consists only of printable ASCII characters
 * @param[in] name The String to test
 * @return 0 if the String consists only of printable ASCII characters, -1 if not
 */
int isName(const char * name);

/** Takes a random student and calls openPic for this student
 * @return a random Student
 */
struct Student* randomStudent();

/** Gets the student-count
 *	@return 0
 */
int countStudents();

/** Gets all students from the configuration-file
 * @return 0 if everything works, -1 if malloc fails,  else the number of the student, that caused the failure
 */
int getStudents();

/** Reads the general information of the pictures from the configuration-file
 * @return 0 if read successfully, else -1
 */
int getImageInfo();

/** Opens the configuration-file and saves the filepointer
 * @return 0 if file is successfully opened, else -1
 */
int getInfoFile();

/** frees memory in TTT.c
 */
void cleanupMain();

#endif /* TTT_H_ */
