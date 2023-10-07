#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <inttypes.h>
#include <malloc.h>
#include <string.h>
#include<math.h>
#include "glew.h"
#include "freeglut.h"
#include "glfw3.h"
const int SCR_WIDTH = 1080, SCR_HEIGHT = 720;
GLuint base_prog;
GLuint quad_vbo;
GLuint tex;
unsigned char* image;

struct pixel {
	uint8_t r;
	uint8_t  g;
	uint8_t  b;
};

typedef struct list {
	int x;
	int y;
	struct list* next;
}list;

void remove1eloflist(list** gertva) {
	list* del = *gertva;
	*gertva = (*gertva)->next;
	free(del);
}

void add(list** prosh, int xnew, int ynew) {
	list* new = (list*)malloc(sizeof(list));
	if (new == NULL) {
		printf("malloc error");
		exit(-1);
	}
	new->x = xnew;
	new->y = ynew;
	new->next = (*prosh);
	(*prosh) = new;
}

void glAttachShaderSource(GLuint prog, GLenum type, const char* source)
{
	GLuint sh;

	sh = glCreateShader(type);
	glShaderSource(sh, 1, &source, NULL);
	glCompileShader(sh);
	char buffer[4096];
	glGetShaderInfoLog(sh, sizeof(buffer), NULL, buffer);
	glAttachShader(prog, sh);
	glDeleteShader(sh);
}

void Init(FILE* fout)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEBUG_OUTPUT);
	base_prog = glCreateProgram();
	static const char quad_shader_vs[] =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec2 in_position;\n"
		"layout (location = 1) in vec2 in_tex_coord;\n"
		"\n"
		"out vec2 tex_coord;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"    gl_Position = vec4(in_position, 0.0, 1.0);\n"
		"    tex_coord = in_tex_coord;\n"
		"}\n"
		;
	static const char quad_shader_fs[] =
		"#version 330 core\n"
		"\n"
		"in vec2 tex_coord;\n"
		"\n"
		"layout (location = 0) out vec4 color;\n"
		"\n"
		"uniform sampler2D tex;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"    color = texture(tex,tex_coord);\n"
		"}\n"
		;
	glAttachShaderSource(base_prog, GL_VERTEX_SHADER, quad_shader_vs);
	glAttachShaderSource(base_prog, GL_FRAGMENT_SHADER, quad_shader_fs);
	glLinkProgram(base_prog);
	static const GLfloat quad_data[] =
	{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};
	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(8 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	int width, height;
	FILE* fp = fout;
	if (!fp) {
		printf("read file error\n");
		return;
	}

	unsigned char buff[4] = { 0 };
	fseek(fp, 18, SEEK_SET);
	fread(buff, 1, 4, fp);
	width = (buff[3] << 24) + (buff[2] << 16) + (buff[1] << 8) + buff[0];
	memset(buff, 0, sizeof(buff));
	fread(buff, 1, 4, fp);
	height = (buff[3] << 24) + (buff[2] << 16) + (buff[1] << 8) + buff[0];
	int line_stride = (width * 3 + 3) / 4 * 4;
	image = (unsigned char*)calloc(1, height * line_stride);
	fseek(fp, 54, SEEK_SET);
	fread(image, 1, height * line_stride, fp);
	fclose(fp);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		printf("Failed to load texture");
	}

	if (image) {
		free(image);
		image = NULL;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void display(void)
{
	glClearColor(0.0, 255, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(base_prog);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(base_prog, "tex"), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glutSwapBuffers();
}

void OnShutdown()
{
	glUseProgram(0);
	glDeleteProgram(base_prog);
	glDeleteTextures(1, &tex);
	glDeleteVertexArrays(1, &tex);
	glDeleteBuffers(1, &quad_vbo);
}

void paint(int** pole,int l,int h,int* k) {
	list* stack = NULL;
	int k1, k2, i2, j2;
	while (1) {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < l; j++) {
				k1 = i;
				k2 = j;
				if (pole[i][j] == 0) {
					add(&stack, i, j);
					break;
				}

			}
			if (stack != NULL) {
				break;
			}
		}
		if (stack == NULL) {
			break;
		}
		while (1) {
			i2 = stack->x;
			j2 = stack->y;
			pole[i2][j2] =  (*k);
			remove1eloflist(&stack);
			if ((i2 < h - 1) && (pole[i2 + 1][j2] == 0)) {
				add(&stack, i2 + 1, j2);
			}
			if ((i2 > 0) && (pole[i2 - 1][j2] == 0)) {
				add(&stack, i2 - 1, j2);
			}
			if ((j2 < l - 1) && (pole[i2][j2 + 1] == 0)) {
				add(&stack, i2, j2 + 1);
			}
			if ((j2 > 0) && (pole[i2][j2 - 1] == 0)) {
				add(&stack, i2, j2 - 1);
			}
			if (stack == NULL) {
				break;
			}
		}
		(* k)++;
		if ((k1 == h - 1) && (k2 == l - 1)) {
			break;
		}
	}
}

void smejn(int** pole, int** smej, int l, int h, int razmsm) {
	for (int i = 0; i < razmsm; i++) {
		for (int j = 0; j < razmsm; j++) {
			smej[i][j] = 0;
		}
	}
	int i2 = 0;
	int j2 = 0;
	int  dl1 = 0;
	while (1) {
		if (i2 < h - 1 && pole[i2][j2] != 1 && pole[i2 + 1][j2] == 1) {
			dl1 = 1;
			for (int i = i2 + 1; i < h; i++) {
				if (j2 + dl1 < l) {
					if (pole[i][j2 + dl1] != pole[i2][j2] && pole[i][j2 + dl1] != 1) {
						for (int temp = i; temp < h; temp++) {
							if ((pole[temp][j2] != pole[i2][j2] && pole[temp][j2] != 1)) {
								if (pole[temp][j2] == pole[i][j2 + dl1]) {
									smej[pole[temp][j2] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (j2 - dl1 >= 0) {
					if (pole[i][j2 - dl1] != pole[i2][j2] && pole[i][j2 - dl1] != 1) {
						for (int temp = i; temp < h; temp++) {
							if ((pole[temp][j2] != pole[i2][j2] && pole[temp][j2] != 1)) {
								if (pole[temp][j2] == pole[i][j2 - dl1]) {
									smej[pole[temp][j2] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (pole[i][j2] != pole[i2][j2] && pole[i][j2] != 1) {
					smej[pole[i][j2] - 2][pole[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		if (i2 > 0 && pole[i2][j2] != 1 && pole[i2 - 1][j2] == 1) {
			dl1 = 1;
			for (int i = i2 - 1; i >= 0; i--) {

				if (j2 + dl1 < l) {
					if (pole[i][j2 + dl1] != pole[i2][j2] && pole[i][j2 + dl1] != 1) {
						for (int temp = i; temp >= 0; temp--) {
							if ((pole[temp][j2] != pole[i2][j2] && pole[temp][j2] != 1)) {
								if (pole[temp][j2] == pole[i][j2 + dl1]) {
									smej[pole[temp][j2] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (j2 - dl1 >= 0) {
					if (pole[i][j2 - dl1] != pole[i2][j2] && pole[i][j2 - dl1] != 1) {
						for (int temp = i; temp >= 0; temp--) {
							if ((pole[temp][j2] != pole[i2][j2] && pole[temp][j2] != 1)) {
								if (pole[temp][j2] == pole[i][j2 - dl1]) {
									smej[pole[temp][j2] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (pole[i][j2] != pole[i2][j2] && pole[i][j2] != 1) {
					smej[pole[i][j2] - 2][pole[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		if (j2 < l - 1 && pole[i2][j2] != 1 && pole[i2][j2 + 1] == 1) {
			dl1 = 1;
			for (int j = j2 + 1; j < l; j++) {

				if (i2 + dl1 < h) {
					if (pole[i2 + dl1][j] != pole[i2][j2] && pole[i2 + dl1][j] != 1) {
						for (int temp = j; temp < l; temp++) {
							if ((pole[i2][temp] != pole[i2][j2] && pole[i2][temp] != 1)) {
								if (pole[i2][temp] == pole[i2 + dl1][j]) {
									smej[pole[i2][temp] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (i2 - dl1 >= 0) {
					if (pole[i2 - dl1][j] != pole[i2][j2] && pole[i2 - dl1][j] != 1) {
						for (int temp = j; temp < l; temp++) {
							if ((pole[i2][temp] != pole[i2][j2] && pole[i2][temp] != 1)) {
								if (pole[i2][temp] == pole[i2 - dl1][j]) {
									smej[pole[i2][temp] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (pole[i2][j] != pole[i2][j2] && pole[i2][j] != 1) {
					smej[pole[i2][j] - 2][pole[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		if (j2 > 0 && pole[i2][j2] != 1 && pole[i2][j2 - 1] == 1) {
			dl1 = 1;
			for (int j = j2 - 1; j >= 0; j--) {

				if (i2 + dl1 < h) {
					if (pole[i2 + dl1][j] != pole[i2][j2] && pole[i2 + dl1][j] != 1) {
						for (int temp = j; temp >= 0; temp--) {
							if ((pole[i2][temp] != pole[i2][j2] && pole[i2][temp] != 1)) {
								if (pole[i2][temp] == pole[i2 + dl1][j]) {
									smej[pole[i2][temp] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (i2 - dl1 >= 0) {
					if (pole[i2 - dl1][j] != pole[i2][j2] && pole[i2 - dl1][j] != 1) {
						for (int temp = j; temp >= 0; temp--) {
							if ((pole[i2][temp] != pole[i2][j2] && pole[i2][temp] != 1)) {
								if (pole[i2][temp] == pole[i2 - dl1][j]) {
									smej[pole[i2][temp] - 2][pole[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (pole[i2][j] != pole[i2][j2] && pole[i2][j] != 1) {
					smej[pole[i2][j] - 2][pole[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		j2++;
		if (j2 == l) {
			i2++;
			j2 = 0;
		}
		if ((i2 == h - 1) && (j2 == l - 1)) {
			break;
		}
	}
}

void group(int** pole,int** smej,int** groupcol,int razmsm,int l,int h) {
	int ik1 = 0, jk1 = 0, flagcol = 0, max1 = 0, sch1, ik1pr = -1, jk2;
	while (1) {
		ik1 = -1;
		max1 = 0;
		for (int i = 0; i < razmsm; i++) {
			sch1 = 0;
			for (int j = 0; j < razmsm; j++) {
				if (smej[i][j] == 1) {
					sch1++;
				}
			}
			if (sch1 > max1 && sch1 != razmsm) {
				ik1 = i;
				max1 = sch1;
			}
		}
		if (ik1 == -1) {
			break;
		}
		if (ik1 == ik1pr) {
			for (int j = 0; j < razmsm; j++) {
				smej[ik1][j] = 1;
			}
			continue;
		}
		ik1pr = ik1;
		jk1 = 0;
		flagcol = 0;
		for (int i = 0; i < razmsm; i++) {
			for (int j = 0; j < razmsm; j++) {
				if (groupcol[i][j] == ik1 + 2) {
					flagcol = 1;
					break;
				}
			}
		}
		if (flagcol == 0) {
			groupcol[ik1][jk1] = ik1 + 2;
			jk1++;
			while (1) {
				jk2 = -1;
				max1 = 0;
				for (int i = 0; i < razmsm; i++) {
					sch1 = 0;
					if (smej[ik1][i] == 0) {
						for (int j = 0; j < razmsm; j++) {
							if (smej[i][j] == 1) {
								sch1++;
							}
						}
						if (sch1 > max1 && sch1 != razmsm) {
							jk2 = i;
							max1 = sch1;
						}
					}
				}
				if (jk2 == -1) {
					break;
				}
				flagcol = 1;
				for (int i = 0; i < razmsm; i++) {
					for (int j = 0; j < razmsm; j++) {
						if (groupcol[i][j] == jk2 + 2) {
							flagcol = 0;
							break;
						}
					}
				}
				if (flagcol) {
					for (int jk3 = 0; jk3 < razmsm; jk3++) {
						if (smej[ik1][jk3] == 1 || smej[jk2][jk3] == 1) {
							smej[ik1][jk3] = 1;
							smej[jk2][jk3] = 1;
						}
						if (smej[ik1][jk3] == 0 && smej[jk2][jk3] == 0) {
							smej[ik1][jk3] = 0;
							smej[jk2][jk3] = 1;
						}
					}
					groupcol[ik1][jk1] = jk2 + 2;
					jk1++;
				}
			}
		}
	}
	int pricol = 0;
	for (int i = 0; i < razmsm; i++) {
		for (int j = 0; j < razmsm; j++) {
			if (groupcol[i][j] == 0) {
				break;
			}
			else {
				if (j == 0) {
					pricol = groupcol[i][j];
				}
				for (int ii = 0; ii < h; ii++) {
					for (int jj = 0; jj < l; jj++) {
						if (pole[ii][jj] == groupcol[i][j]) {
							pole[ii][jj] = pricol;
						}
					}
				}
			}
		}
	}
}

int main(int argc, char** argv) {

	FILE* finp;
	FILE* fout;
	char fi[15], fo[15];
	printf("Enter the name of the file containing the image.\n");
	gets(fi);
	fi[strlen(fi)] = '\0';
	printf("Enter the name of the file you want to write the result to.\n");
	gets(fo);
	fo[strlen(fo)] = '\0';
	if ((finp = fopen(fi, "rb")) == NULL)
	{
		printf("Incorect name of file input");
		_getch();
		exit(-1);
	}
	if ((fout = fopen(fo, "wb+")) == NULL)
	{
		printf("Incorect name of file out");
		_getch();
		exit(-1);
	}

	fseek(finp, 0, SEEK_SET);
	uint8_t* temp;
	for (int i = 0; i < 54; i++) {
		fread(&temp, sizeof(uint8_t), 1, finp);
		fwrite(&temp, sizeof(uint8_t), 1, fout);
	}
	int l,  h;
	fseek(finp, 18, SEEK_SET);
	fread(&l, sizeof(int), 1, finp);
	fread(&h, sizeof(int), 1, finp);
	fseek(finp, 54, SEEK_SET);

	struct pixel** pixelmass;
	pixelmass = (struct pixel**)malloc(h * sizeof(struct pixel*));
	if (pixelmass == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < h; i++) {
		pixelmass[i] = (struct pixel*)malloc(l * sizeof(struct pixel));
		if (pixelmass[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}
	int** pole;
	pole = (int**)malloc(h * sizeof(int*));
	if (pole == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < h; i++) {
		pole[i] = (int*)malloc(l * sizeof(int));
		if (pole[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < l; j++) {
			fread(&(pixelmass[i][j].r), sizeof(uint8_t), 1, finp);
			fread(&(pixelmass[i][j].g), sizeof(uint8_t), 1, finp);
			fread(&(pixelmass[i][j].b), sizeof(uint8_t), 1, finp);
			pole[i][j] = pixelmass[i][j].r + pixelmass[i][j].g + pixelmass[i][j].b;
			if (pole[i][j] == 765) {
				pole[i][j] = 0;
			}
			else {
				pole[i][j] = 1;
				pixelmass[i][j].r = 0;
				pixelmass[i][j].g = 0;
				pixelmass[i][j].b = 0;
			}
		}
	}

	int k = 2;
	paint(pole, l, h, &k);

	int razmsm = k - 2;
	int** smej;
	smej = (int**)malloc(razmsm * sizeof(int*));
	if (smej == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < razmsm; i++) {
		smej[i] = (int*)malloc(razmsm * sizeof(int));
		if (smej[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}
	
	smejn(pole, smej, l, h, razmsm);

	for (int i = 0; i < razmsm; i++) {
		for (int j = 0; j < razmsm; j++) {
			if (i == j) {
				smej[i][j] = 1;
			}
		}
	}
	for (int i = 0; i < razmsm; i++) {
		for (int j = 0; j < razmsm; j++) {
			if (smej[i][j] == 1) {
				smej[j][i] = 1;
			}
			if (smej[j][i] == 1) {
				smej[i][j] = 1;
			}

		}

	}

	int** groupcol;
	groupcol = (int**)malloc(razmsm * sizeof(int*));
	if (groupcol == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < razmsm; i++) {
		groupcol[i] = (int*)malloc(razmsm * sizeof(int));
		if (groupcol[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}
	for (int i = 0; i < razmsm; i++) {
		for (int j = 0; j < razmsm; j++) {
			groupcol[i][j] = 0;
		}
	}

	group(pole, smej, groupcol, razmsm, l, h);

	k = 0;
	for (int i = 0; i < razmsm; i++) {
		if (groupcol[i][0] != 0) {
			k++;
		}
		
	}
	printf("Count of colors: %d\n", k);

	fseek(fout, 54, SEEK_SET);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < l; j++) {
			if (pole[i][j] != 1) {
				pixelmass[i][j].r = pole[i][j] *11;
				pixelmass[i][j].g = pole[i][j] *53;
				pixelmass[i][j].b = pole[i][j] *111;
			}
			fwrite(&(pixelmass[i][j].r), sizeof(uint8_t), 1, fout);
			fwrite(&(pixelmass[i][j].g), sizeof(uint8_t), 1, fout);
			fwrite(&(pixelmass[i][j].b), sizeof(uint8_t), 1, fout);
		}
	}

	for (int i = 0; i < razmsm; i++) {
		free(groupcol[i]);
	}
	free(groupcol);
	for (int i = 0; i < razmsm; i++) {
		free(smej[i]);
	}
	free(smej);
	for (int i = 0; i < h; i++) {
		free(pole[i]);
	}
	free(pole);
	for (int i = 0; i < h; i++) {
		free(pixelmass[i]);
	}
	free(pixelmass);
	fclose(finp);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	if (l > 1080) {
		l = l / 2;
		h = h / 2;
	}
	if (h > 720) {
		h = h / 2;
		l = l / 2;
	}
	glutInitWindowSize(l, h);
	glutCreateWindow("out");

	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW ... exiting");
		exit(EXIT_FAILURE);
	}

	Init(fout);

	glutDisplayFunc(display);

	glutCloseFunc(OnShutdown);

	glutMainLoop();

	return 0;
}


