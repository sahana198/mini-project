 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

#define MAX_ROWS 20
#define MAX_COLS 60
#define MAX_OBJECTS 20

typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int x1, y1; // Base points
    int x2, y2; // Secondary points (used depending on shape)
    int r;      // Radius for circle
    int x3, y3; // Third point for triangle
    char brush; // '*' or '_'
} Shape;

Shape objects[MAX_OBJECTS];
int object_count = 0;
int next_id = 1;
char canvas[MAX_ROWS][MAX_COLS];

// --- Canvas Management ---
void clear_canvas() {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            canvas[i][j] = ' '; // Clear with spaces
        }
    }
}