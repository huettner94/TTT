/** Graphics headerfile "Train the teacher"
 *
 * @file graphics.h
 * @author Sven Rudolph (9744081)
 * @author Felix Huettner (2295235)
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "TTT.h"
#include "Glut/glut.h"


/** Opens the File and Displays the Image
 * @param [in] i Structure with general information
 * @return 0 the image was displayed correctly, else -1
 */
void window(struct Info* i);

/** Frees allocated memory in graphics.c
 */
void cleanupGraphics();

/** Loads an Image
 * @return 0 if successfull, else -1
 */
int loadImage();

/** Initializes the window and adds the Menu
 */
void initDisplay();

/** Checks if the entered name is correct
 */
void checkName();

/** Menuhandler
 * @param[in] entry the clicked entry
 */
void menuHandler(int entry);

/** Creates the negative of an image
 */
void makeNegative();

/** Keyboardhandler
 * @param[in] key the key that was pressed
 * @param[in] x x-Position of the Mouse
 * @param[in] y y-Position fo the Mouse
 */
void keyPressed(unsigned char key, int x, int y);

/** Draws the image
 */
void drawImage();


#endif /* GRAPHICS_H_ */
