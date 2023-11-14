#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 5
#define COLS 5

typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    Point* path;
    int size;
} PathResult;

typedef struct Node {
    struct Node* parent;
    Point position;
    int g;
    int h;
    int f;
} Node;

Node* createNode(Node* parent, Point position) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->parent = parent;
    node->position = position;
    node->g = 0;
    node->h = 0;
    node->f = 0;
    return node;
}

bool isEqual(Node* a, Node* b) {
    return (a->position.row == b->position.row && a->position.col == b->position.col);
}

PathResult* astar(int maze[][COLS], Point start, Point end) {
    // Create start and end nodes
    Node* startNode = createNode(NULL, start);
    startNode->g = startNode->h = startNode->f = 0;
    Node* endNode = createNode(NULL, end);
    endNode->g = endNode->h = endNode->f = 0;
    // Allocates memory for two arrays of pointers to Node object: openlist and closedlist
    Node** openList = (Node**)malloc(sizeof(Node*) * (ROWS * COLS));
    Node** closedList = (Node**)malloc(sizeof(Node*) * (ROWS * COLS));
    // Declares integer variables, top keep track of the number of elements
    int openListSize = 0;
    int closedListSize = 0;
    // Assigning start node to openlist at the index indicated by openlistsize
    // After that, increment openlistsize by 1
    openList[openListSize++] = startNode;
    while (openListSize > 0) {
        // This section of code ensures that at each iteration of the while loop, the node with the lowest f cost is selected from the open list, moved to the closed list
        // The current node is the first node in the open list
        // The current index keeps track of the index of the current node
        Node* currentNode = openList[0];
        int currentIndex = 0;
        // Find a node inside the open list in which the f cost is smaller than the current node f cost
        // After finding it, the node with lower f cost is the new current node, and its index is the current index
        for (int i = 1; i < openListSize; i++) {
            if (openList[i]->f < currentNode->f) {
                currentNode = openList[i];
                currentIndex = i;
            }
        }
        // Shifts the elements in the openList array one position to the left, effectively overwriting the currentNode with the next node in the list.
        // This loop is used to remove the currentNode from the openList while maintaining the order of the remaining elements.
        for (int i = currentIndex; i < openListSize - 1; i++) {
            openList[i] = openList[i + 1];
        }
        // Decrement the openlistsize variable by 1
        openListSize--;
        // Adds the currentNode to the closedList array at the index indicated by closedListSize.
        // Increment closedlistsize by 1
        closedList[closedListSize++] = currentNode;

        // -------------------------------------------------------------------------------------------------------
        
        // Check if currentnode and endnode are equal
        if (isEqual(currentNode, endNode)) {
            // Allocates memory for an array of Point objects, will represent the path from start to end node
            Point* path = (Point*)malloc(sizeof(Point) * (ROWS * COLS));
            // This variable is used to keep track of the number of points in the path
            int pathSize = 0;
            // Variable current is assigned the value of currentnode
            Node* current = currentNode;

            while (current != NULL) {
                // Current node position is appended to path. Increment pathsize by 1
                path[pathSize++] = current->position;
                // The current node is updated to its parent node, until current = NULL, which means it has reached the start node
                current = current->parent;
            }
            // Reverse the path elements
            Point* reversedPath = (Point*)malloc(sizeof(Point) * pathSize);
            for (int i = 0; i < pathSize; i++) {
                reversedPath[i] = path[pathSize - 1 - i];
            }
            free(path);  // Free the original path array
            PathResult* result = (PathResult*)malloc(sizeof(PathResult));
            result->path = reversedPath;
            result->size = pathSize;
            return result;
        }

        // ----------------------------------------------------------------------------------

        // Allocates memory for an array children to store pointers to Node objects
        Node** children = (Node**)malloc(sizeof(Node*) * 4);
        //Initializes numchildren = 0 to keep track of the number of child nodes created
        int numChildren = 0;
        // Declares newrow and newcol, will store the coordinates of the neighbouring positions
        int newRow, newCol;
        // Iterates four new adjacent nodes
        int newPositions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        for (int i = 0; i < 4; i++) {
            // Get node position
            newRow = currentNode->position.row + newPositions[i][0];
            newCol = currentNode->position.col + newPositions[i][1];
            // Make sure within range (the grid)
            if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS) {
                continue;
            }
            // Make sure walkable terrain (not obstacle)
            if (maze[newRow][newCol] != 0) {
                continue;
            }
            // Create a new node. The current node is passed as the parent node
            Node* newChild = createNode(currentNode, (Point){newRow, newCol});
            // Assign the new node to the children array. Increment numchildren by 1
            children[numChildren++] = newChild;
        }

        // ---------------------------------------------------------------------------------

        // iterates over the children array
        for (int i = 0; i < numChildren; i++) {
            // Assign children[i] to child variable for convenience
            Node* child = children[i];
            bool isInClosedList = false;
            // check if child node already in closedlist
            for (int j = 0; j < closedListSize; j++) {
                if (isEqual(child, closedList[j])) {
                    isInClosedList = true;
                    break;
                }
            }
            // Check if the isinclosedlist is true, indicating that the child node is already in closed list
            // If so, it means that this child node has already been processed and should be ignored
            if (isInClosedList) {
                // The child node is freed to release the allocated memory, and 'continue' to skip the remaining code in the loop and move to the next iteration
                free(child);
                continue;
            }
            // Calculate f,g,h values for the child node if it is not in the closed list
            child->g = currentNode->g + 1;
            child->h = sqrt(pow((child->position.row - endNode->position.row), 2) +
                           pow((child->position.col - endNode->position.col), 2));
            child->f = child->g + child->h;
            // Check if child node is on open list, and check if it has greater g value than the existing node in the open list
            bool isInOpenList = false;
            for (int j = 0; j < openListSize; j++) {
                // IF both conditions are satisfied, it means the child node has already been considered with a lower cost, no need to proceed with it.
                if (isEqual(child, openList[j]) && child->g > openList[j]->g) {
                    isInOpenList = true;
                    break;
                }
            }

            if (isInOpenList) {
                free(child);
                continue;
            }
            // If the child node is not in the clostlist or openlist, it is added to the openlist
            // Increment openlistsize by 1
            openList[openListSize++] = child;
        }
        free(children);
    }

    free(openList);
    free(closedList);
    
    return NULL;
}

// void printPath(Point* path, int size) {
//     // int mypath[25][3];
//     printf("Path:\n");
//     for (int i = 0; i < size; i++) {
//         // mypath[i][0] = 300 + path[i].row * 50;
//         // mypath[i][1] = -100 + path[i].col * 50;
//         // mypath[i][2] = 200;
//         // printf("(%d,%d,%d)\n", 300 + path[i].row * 50, -100 + path[i].col * 50, 200);
//         // printf("{%d, %d, %d}\n", path[i][0], path[i][1], path[i][2]);
//         printf("{%d, %d}\n", path[i].row, path[i].col);
//     }
// }

int main() {
    int maze[ROWS][COLS] = {
        {0, 0, 1, 1},
        {0, 0, 0, 0},
        {0, 1, 1, 0},
    };

    Point start = {0, 0};
    Point end = {2, 3};

    PathResult* result = astar(maze, start, end);
    if (result != NULL) {
        // printPath(result->path, result->size);
        for (int i = 0; i < result->size; i++){
            printf("{%d, %d}\n", result->path[i].row, result->path[i].col);
        }
        free(result->path);
        free(result);
    } else {
        printf("No path found.\n");
    }

    return 0;
}
