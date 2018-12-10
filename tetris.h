#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

typedef struct board board;

struct board{
	unsigned char squares[10][20];
	unsigned char current_piece;
	unsigned char current_orientation;
	unsigned char next_piece;
	unsigned char next_orientation;
	char current_x;
	char current_y;
	char level;
};

unsigned char I_piece1[] = {1, 1, 1, 1};
unsigned char *I_piece[4];

unsigned char J_piece1[] = {1, 1, 1, 0, 0, 1};
unsigned char J_piece2[] = {0, 1, 0, 1, 1, 1};
unsigned char J_piece3[] = {1, 0, 0, 1, 1, 1};
unsigned char J_piece4[] = {1, 1, 1, 0, 1, 0};
unsigned char *J_piece[4];

unsigned char L_piece1[] = {1, 1, 1, 1, 0, 0};
unsigned char L_piece2[] = {1, 1, 0, 1, 0, 1};
unsigned char L_piece3[] = {0, 0, 1, 1, 1, 1};
unsigned char L_piece4[] = {1, 0, 1, 0, 1, 1};
unsigned char *L_piece[4];

unsigned char O_piece1[] = {1, 1, 1, 1};
unsigned char *O_piece[4];

unsigned char S_piece1[] = {0, 1, 1, 1, 1, 0};
unsigned char S_piece2[] = {1, 0, 1, 1, 0, 1};
unsigned char *S_piece[4];

unsigned char T_piece1[] = {1, 1, 1, 0, 1, 0};
unsigned char T_piece2[] = {0, 1, 1, 1, 0, 1};
unsigned char T_piece3[] = {0, 1, 0, 1, 1, 1};
unsigned char T_piece4[] = {1, 0, 1, 1, 1, 0};
unsigned char *T_piece[4];

unsigned char Z_piece1[] = {1, 1, 0, 0, 1, 1};
unsigned char Z_piece2[] = {0, 1, 1, 1, 1, 0};
unsigned char *Z_piece[4];

unsigned char **pieces[7];
unsigned char dimensions[7][2] = {{4, 1}, {3, 2}, {3, 2}, {2, 2}, {3, 2}, {3, 2}, {3, 2}};

unsigned char tile[5] = {0b11111000,
			 0b10001000,
			 0b10001000,
			 0b10001000,
			 0b11111000};

