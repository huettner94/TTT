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

#ifdef _WIN32
#define DELIMITER '\\'					///< Delimiter in File path
#else
#define DELIMITER '/'					///< Delimiter in File path
#endif

char * prefix;							///< Contains the absolute path to the program(max 500 characters)

/** @brief A Structure for each student.
 */
struct Student{
	char firstname[200];				///< The first name of the student
	char lastname[200];					///< The last name of the student
	int matnumber;						///< The matrikulation number of the student
	bool correct;						///< if this Student has been guessed correctly
};

/** @brief A Structure with general Information
 */
struct Info{
		FILE * InfoFile;				///< InfoFile File pointer to the InfoFile
		int width;						///< width The width of the Students picture
		int height;						///< height The height of the Students picture
		int bpp;						///< bpp Bits per Pixel in a picture (can only be 8 or 24)
		struct Student * student;		///< student Array of all students
		int students;					///< students count of all students
		int maxNameLength;				///< maxNameLength The size of the longest name
		char ImagePath[200];			///< ImagePath Path to the images
		char format[10];				///< format File format
		char * InfoFilePath;			///< InfoFilePath Absolute Path to the InfoFile
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
