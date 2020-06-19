void clearscreen();

void cbreak();

void nocbreak();

int *up(int **maze, int *pos);
int *down(int **maze, int *pos);
int *right(int **maze, int *pos);
int *left(int **maze, int *pos);

int *mapa(int **maze);
void cursor_pos(int *pos);
int *mov_screen(int **maze,int *pos, char buf);

void sense(int **maze,int *info, int * surroundings);
