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
void displayCanvas() {
    int i, j;

    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

void plotPoint(int x, int y) {
    if(x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = '*';
    }
}

void drawLineCanvas(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int steps = dx > dy ? dx : dy;

    float xInc = (float)(x2 - x1) / steps;
    float yInc = (float)(y2 - y1) / steps;

    float x = x1;
    float y = y1;

    int i;

    for(i = 0; i <= steps; i++) {
        plotPoint((int)(x + 0.5), (int)(y + 0.5));
        x += xInc;
        y += yInc;
    }
}

void drawRectangleCanvas(int x, int y, int width, int height) {
    int i;

    for(i = x; i < x + width; i++) {
        plotPoint(i, y);
        plotPoint(i, y + height - 1);
    }

    for(i = y; i < y + height; i++) {
        plotPoint(x, i);
        plotPoint(x + width - 1, i);
    }
}

void drawCircleCanvas(int xc, int yc, int r) {
    int angle;

    for(angle = 0; angle < 360; angle++) {

        double rad = angle * 3.14159265 / 180.0;

        int x = xc + (int)(r * cos(rad));
        int y = yc + (int)(r * sin(rad));

        plotPoint(x, y);
    }
}

void drawTriangleCanvas(
        int x1, int y1,
        int x2, int y2,
        int x3, int y3) {

    drawLineCanvas(x1, y1, x2, y2);
    drawLineCanvas(x2, y2, x3, y3);
    drawLineCanvas(x3, y3, x1, y1);
}

void redrawCanvas() {

    int i;

    clearCanvas();

    for(i = 0; i < objectCount; i++) {

        switch(objects[i].type) {

            case 1:
                drawLineCanvas(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case 2:
                drawRectangleCanvas(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].width,
                    objects[i].height
                );
                break;

            case 3:
                drawCircleCanvas(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].radius
                );
                break;

            case 4:
                drawTriangleCanvas(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2,
                    objects[i].x3,
                    objects[i].y3
                );
                break;
        }
    }
}

void addLine() {

    Shape s;

    s.type = 1;

    printf("Enter x1 y1 x2 y2: ");
    scanf("%d%d%d%d",
          &s.x1, &s.y1,
          &s.x2, &s.y2);

    objects[objectCount++] = s;
}

void addRectangle() {

    Shape s;

    s.type = 2;

    printf("Enter x y width height: ");
    scanf("%d%d%d%d",
          &s.x1, &s.y1,
          &s.width, &s.height);

    objects[objectCount++] = s;
}

void addCircle() {

    Shape s;

    s.type = 3;

    printf("Enter centerX centerY radius: ");
    scanf("%d%d%d",
          &s.x1, &s.y1,
          &s.radius);

    objects[objectCount++] = s;
}

void addTriangle() {

    Shape s;

    s.type = 4;

    printf("Enter x1 y1 x2 y2 x3 y3: ");

    scanf("%d%d%d%d%d%d",
          &s.x1, &s.y1,
          &s.x2, &s.y2,
          &s.x3, &s.y3);

    objects[objectCount++] = s;
}

void deleteObject() {

    int index, i;

    if(objectCount == 0) {
        printf("No objects available.\n");
        return;
    }

    printf("Enter object index (0-%d): ",
           objectCount - 1);

    scanf("%d", &index);

    if(index < 0 || index >= objectCount) {
        printf("Invalid index.\n");
        return;
    }

    for(i = index; i < objectCount - 1; i++) {
        objects[i] = objects[i + 1];
    }

    objectCount--;
}

void modifyObject() {

    int index;

    if(objectCount == 0) {
        printf("No objects available.\n");
        return;
    }

    printf("Enter object index (0-%d): ",
           objectCount - 1);

    scanf("%d", &index);

    if(index < 0 || index >= objectCount) {
        printf("Invalid index.\n");
        return;
    }

    switch(objects[index].type) {

        case 1:
            printf("Enter new x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &objects[index].x1,
                  &objects[index].y1,
                  &objects[index].x2,
                  &objects[index].y2);
            break;

        case 2:
            printf("Enter new x y width height: ");
            scanf("%d%d%d%d",
                  &objects[index].x1,
                  &objects[index].y1,
                  &objects[index].width,
                  &objects[index].height);
            break;

        case 3:
            printf("Enter new centerX centerY radius: ");
            scanf("%d%d%d",
                  &objects[index].x1,
                  &objects[index].y1,
                  &objects[index].radius);
            break;

        case 4:
            printf("Enter new x1 y1 x2 y2 x3 y3: ");
            scanf("%d%d%d%d%d%d",
                  &objects[index].x1,
                  &objects[index].y1,
                  &objects[index].x2,
                  &objects[index].y2,
                  &objects[index].x3,
                  &objects[index].y3);
            break;
    }
}

int main() {

    int choice;

    clearCanvas();

    while(1) {

        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Line\n");
        printf("2. Add Rectangle\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. Display Picture\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch(choice) {

            case 1:
                addLine();
                redrawCanvas();
                break;

            case 2:
                addRectangle();
                redrawCanvas();
                break;

            case 3:
                addCircle();
                redrawCanvas();
                break;

            case 4:
                addTriangle();
                redrawCanvas();
                break;

            case 5:
                deleteObject();
                redrawCanvas();
                break;

            case 6:
                modifyObject();
                redrawCanvas();
                break;

            case 7:
                displayCanvas();
                break;

            case 8:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}