#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <inttypes.h>
#include <malloc.h>
#include <string.h>
#include<math.h>
#include <time.h>

struct pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

typedef struct stack {
	int x;
	int y;
	struct stack* next;
}stack;

void pop(stack** head) {
	stack* del = *head;
	*head = (*head)->next;
	free(del);
}

void push(stack** head, int x_new, int y_new) {
	stack* new = (stack*)malloc(sizeof(stack));
	if (new == NULL) {
		printf("malloc error");
		exit(-1);
	}
	new->x = x_new;
	new->y = y_new;
	new->next = (*head);
	(*head) = new;
}



void identify_every_area(int** integer_pixelmap_image, int lenght_image, int height_image, int* count_of_colors) {
	stack* stack = NULL;
	int k1 = 0, k2 = 0, i2, j2;
	for (int i = 0; i < height_image; i++) {
		for (int j = 0; j < lenght_image; j++) {
			if (integer_pixelmap_image[i][j] == 0) {
				push(&stack, i, j);
				while (1) {
					i2 = stack->x;
					j2 = stack->y;
					integer_pixelmap_image[i2][j2] = (*count_of_colors);
					pop(&stack);
					if ((i2 < height_image - 1) && (integer_pixelmap_image[i2 + 1][j2] == 0)) {
						push(&stack, i2 + 1, j2);
					}
					if ((i2 > 0) && (integer_pixelmap_image[i2 - 1][j2] == 0)) {
						push(&stack, i2 - 1, j2);
					}
					if ((j2 < lenght_image - 1) && (integer_pixelmap_image[i2][j2 + 1] == 0)) {
						push(&stack, i2, j2 + 1);
					}
					if ((j2 > 0) && (integer_pixelmap_image[i2][j2 - 1] == 0)) {
						push(&stack, i2, j2 - 1);
					}
					if (stack == NULL) {
						break;
					}
				}
				(*count_of_colors)++;
			}
		}
	}
}

void find_neighboring_areas(int** integer_pixelmap_image, uint8_t** matrix_of_smej, int length_image, int height_image, int sizeof_matrix_of_smej) {
	int i2 = 0;
	int j2 = 0;
	int  dl1 = 0;
	while (1) {
		if (i2 < height_image - 1 && integer_pixelmap_image[i2][j2] != 1 && integer_pixelmap_image[i2 + 1][j2] == 1) {
			dl1 = 1;
			for (int i = i2 + 1; i < height_image; i++) {
				if (j2 + dl1 < length_image) {
					if (integer_pixelmap_image[i][j2 + dl1] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i][j2 + dl1] != 1) {
						for (int temp = i; temp < height_image; temp++) {
							if ((integer_pixelmap_image[temp][j2] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[temp][j2] != 1)) {
								if (integer_pixelmap_image[temp][j2] == integer_pixelmap_image[i][j2 + dl1]) {
									matrix_of_smej[integer_pixelmap_image[temp][j2] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (dl1 <= j2) {
					if (integer_pixelmap_image[i][j2 - dl1] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i][j2 - dl1] != 1) {
						for (int temp = i; temp < height_image; temp++) {
							if ((integer_pixelmap_image[temp][j2] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[temp][j2] != 1)) {
								if (integer_pixelmap_image[temp][j2] == integer_pixelmap_image[i][j2 - dl1]) {
									matrix_of_smej[integer_pixelmap_image[temp][j2] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (integer_pixelmap_image[i][j2] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i][j2] != 1) {
					matrix_of_smej[integer_pixelmap_image[i][j2] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		if (i2 > 0 && integer_pixelmap_image[i2][j2] != 1 && integer_pixelmap_image[i2 - 1][j2] == 1) {
			dl1 = 1;
			for (int i = i2 - 1; i >= 0; i--) {

				if (j2 + dl1 < length_image) {
					if (integer_pixelmap_image[i][j2 + dl1] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i][j2 + dl1] != 1) {
						for (int temp = i; temp >= 0; temp--) {
							if ((integer_pixelmap_image[temp][j2] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[temp][j2] != 1)) {
								if (integer_pixelmap_image[temp][j2] == integer_pixelmap_image[i][j2 + dl1]) {
									matrix_of_smej[integer_pixelmap_image[temp][j2] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (dl1 <= j2) {
					if (integer_pixelmap_image[i][j2 - dl1] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i][j2 - dl1] != 1) {
						for (int temp = i; temp >= 0; temp--) {
							if ((integer_pixelmap_image[temp][j2] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[temp][j2] != 1)) {
								if (integer_pixelmap_image[temp][j2] == integer_pixelmap_image[i][j2 - dl1]) {
									matrix_of_smej[integer_pixelmap_image[temp][j2] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (integer_pixelmap_image[i][j2] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i][j2] != 1) {
					matrix_of_smej[integer_pixelmap_image[i][j2] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		if (j2 < length_image - 1 && integer_pixelmap_image[i2][j2] != 1 && integer_pixelmap_image[i2][j2 + 1] == 1) {
			dl1 = 1;
			for (int j = j2 + 1; j < length_image; j++) {

				if (i2 + dl1 < height_image) {
					if (integer_pixelmap_image[i2 + dl1][j] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2 + dl1][j] != 1) {
						for (int temp = j; temp < length_image; temp++) {
							if ((integer_pixelmap_image[i2][temp] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2][temp] != 1)) {
								if (integer_pixelmap_image[i2][temp] == integer_pixelmap_image[i2 + dl1][j]) {
									matrix_of_smej[integer_pixelmap_image[i2][temp] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (i2 >= dl1) {
					if (integer_pixelmap_image[i2 - dl1][j] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2 - dl1][j] != 1) {
						for (int temp = j; temp < length_image; temp++) {
							if ((integer_pixelmap_image[i2][temp] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2][temp] != 1)) {
								if (integer_pixelmap_image[i2][temp] == integer_pixelmap_image[i2 - dl1][j]) {
									matrix_of_smej[integer_pixelmap_image[i2][temp] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (integer_pixelmap_image[i2][j] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2][j] != 1) {
					matrix_of_smej[integer_pixelmap_image[i2][j] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		if (j2 > 0 && integer_pixelmap_image[i2][j2] != 1 && integer_pixelmap_image[i2][j2 - 1] == 1) {
			dl1 = 1;
			for (int j = j2 - 1; j >= 0; j--) {

				if (i2 + dl1 < height_image) {
					if (integer_pixelmap_image[i2 + dl1][j] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2 + dl1][j] != 1) {
						for (int temp = j; temp >= 0; temp--) {
							if ((integer_pixelmap_image[i2][temp] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2][temp] != 1)) {
								if (integer_pixelmap_image[i2][temp] == integer_pixelmap_image[i2 + dl1][j]) {
									matrix_of_smej[integer_pixelmap_image[i2][temp] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (i2 >= dl1) {
					if (integer_pixelmap_image[i2 - dl1][j] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2 - dl1][j] != 1) {
						for (int temp = j; temp >= 0; temp--) {
							if ((integer_pixelmap_image[i2][temp] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2][temp] != 1)) {
								if (integer_pixelmap_image[i2][temp] == integer_pixelmap_image[i2 - dl1][j]) {
									matrix_of_smej[integer_pixelmap_image[i2][temp] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
								}
								break;
							}
						}
						break;
					}
				}
				if (integer_pixelmap_image[i2][j] != integer_pixelmap_image[i2][j2] && integer_pixelmap_image[i2][j] != 1) {
					matrix_of_smej[integer_pixelmap_image[i2][j] - 2][integer_pixelmap_image[i2][j2] - 2] = 1;
					break;
				}
				dl1++;
			}
		}
		j2++;
		if (j2 == length_image) {
			i2++;
			j2 = 0;
		}
		if ((i2 == height_image - 1) && (j2 == length_image - 1)) {
			break;
		}
	}
}





void find_groups_of_areas(int** integer_pixelmap_image, uint8_t** matrix_of_smej, int sizeof_matrix_of_smej, int length_image, int height_image) {
	int ik1 = 0, jk1 = 0, flagcol = 0, max1 = 0, ik1pr = -1, jk2;
	int** new_groups_of_areas;
	new_groups_of_areas = (int**)calloc(5, sizeof(int*));
	if (new_groups_of_areas == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < 5; i++) {
		new_groups_of_areas[i] = (int*)calloc(sizeof_matrix_of_smej, sizeof(int));
		if (new_groups_of_areas[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}
	uint8_t* buffer_full_1;
	buffer_full_1 = (uint8_t*)calloc(sizeof_matrix_of_smej, sizeof(uint8_t));
	if (buffer_full_1 == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < sizeof_matrix_of_smej; i++) {
		buffer_full_1[i] = 1;
	}

	uint8_t* buffer_full_0;
	buffer_full_0 = (uint8_t*)calloc(sizeof_matrix_of_smej, sizeof(uint8_t));
	if (buffer_full_0 == NULL) {
		printf("malloc error");
		exit(-1);
	}
	uint64_t* buffer_temp1;
	buffer_temp1 = (uint64_t*)calloc(sizeof_matrix_of_smej + (sizeof_matrix_of_smej % 8), sizeof(uint8_t));
	if (buffer_temp1 == NULL) {
		printf("malloc error");
		exit(-1);
	}
	uint64_t* buffer_temp2;
	buffer_temp2 = (uint64_t*)calloc(sizeof_matrix_of_smej + (sizeof_matrix_of_smej % 8), sizeof(uint8_t));
	if (buffer_temp2 == NULL) {
		printf("malloc error");
		exit(-1);
	}
	uint64_t* buffer_temp3;
	buffer_temp3 = (uint64_t*)calloc(sizeof_matrix_of_smej + (sizeof_matrix_of_smej % 8), sizeof(uint8_t));
	if (buffer_temp3 == NULL) {
		printf("malloc error");
		exit(-1);
	}

	int ilisch = sizeof_matrix_of_smej / 8;



	int* count_one_in_str_of_matrix;
	int temp_count = 0;
	count_one_in_str_of_matrix = (int*)calloc(sizeof_matrix_of_smej, sizeof(int));
	for (int i = 0; i < sizeof_matrix_of_smej; i++) {
		for (int j = 0; j < sizeof_matrix_of_smej; j++) {
			if (matrix_of_smej[i][j] == 1) {
				count_one_in_str_of_matrix[i]++;
			}
		}
	}



	while (1) {
		ik1 = -1;
		max1 = 0;
		for (int i = 0; i < sizeof_matrix_of_smej; i++) {
			if (count_one_in_str_of_matrix[i] != sizeof_matrix_of_smej && count_one_in_str_of_matrix[i] > max1) {
				ik1 = i;
				max1 = count_one_in_str_of_matrix[i];
			}
		}
		if (ik1 == -1) {
			break;
		}
		if (ik1 == ik1pr) {
			memcpy(matrix_of_smej[ik1], buffer_full_1, sizeof_matrix_of_smej);
			count_one_in_str_of_matrix[ik1] = sizeof_matrix_of_smej;
			continue;
		}
		ik1pr = ik1;
		jk1 = 0;
		flagcol = 0;
		for (int i = 0; i < 5; i++) {
			if (new_groups_of_areas[i][0] == 0) {
				break;
			}
			for (int j = 0; j < sizeof_matrix_of_smej; j++) {
				if (new_groups_of_areas[i][j] == ik1 + 2) {
					flagcol = 1;
					break;
				}
			}
		}
		if (flagcol == 0) {
			new_groups_of_areas[temp_count][0] = ik1 + 2;
			jk1++;
			while (1) {
				jk2 = -1;
				max1 = 0;
				for (int i = 0; i < sizeof_matrix_of_smej; i++) {
					if (matrix_of_smej[ik1][i] == 0 && count_one_in_str_of_matrix[i] != sizeof_matrix_of_smej && count_one_in_str_of_matrix[i] > max1) {
						jk2 = i;
						max1 = count_one_in_str_of_matrix[i];
					}
				}
				if (jk2 == -1) {
					break;
				}
				flagcol = 1;
				for (int i = 0; i < 5; i++) {
					if (new_groups_of_areas[i][0] == 0) {
						break;
					}
					for (int j = 0; j < sizeof_matrix_of_smej; j++) {
						if (new_groups_of_areas[i][j] == jk2 + 2) {
							flagcol = 0;
							break;
						}
					}
				}
				if (flagcol == 1) {
					for (int jk3 = 0; jk3 < sizeof_matrix_of_smej; jk3++) {
						if (matrix_of_smej[ik1][jk3] == 0 && matrix_of_smej[jk2][jk3] == 1) {
							count_one_in_str_of_matrix[ik1]++;
						}
					}
					memcpy(buffer_temp1, matrix_of_smej[ik1], sizeof_matrix_of_smej);
					memcpy(buffer_temp2, matrix_of_smej[jk2], sizeof_matrix_of_smej);
					for (int il = 0; il <= ilisch; il++) {
						buffer_temp3[il] = buffer_temp1[il] | buffer_temp2[il];
					}
					memcpy(matrix_of_smej[ik1], buffer_temp3, sizeof_matrix_of_smej);
					memcpy(buffer_temp1, buffer_full_0, sizeof_matrix_of_smej);
					memcpy(buffer_temp2, buffer_full_0, sizeof_matrix_of_smej);
					memcpy(buffer_temp3, buffer_full_0, sizeof_matrix_of_smej);


					memcpy(matrix_of_smej[jk2], buffer_full_1, sizeof_matrix_of_smej);
					count_one_in_str_of_matrix[jk2] = sizeof_matrix_of_smej;
					new_groups_of_areas[temp_count][jk1] = jk2 + 2;
					jk1++;
				}
			}
			temp_count++;
		}
	}
	int number_of_group = sizeof_matrix_of_smej + 3;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < sizeof_matrix_of_smej; j++) {
			if (new_groups_of_areas[i][j] == 0) {
				break;
			}
			else {
				if (j == 0) {
					number_of_group += 1;
				}
				for (int ii = 0; ii < height_image; ii++) {
					for (int jj = 0; jj < length_image; jj++) {
						if (integer_pixelmap_image[ii][jj] == new_groups_of_areas[i][j]) {
							integer_pixelmap_image[ii][jj] = number_of_group;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		free(new_groups_of_areas[i]);
	}
	free(new_groups_of_areas);
	free(buffer_full_0);
	free(buffer_full_1);
	free(buffer_temp1);
	free(buffer_temp2);
	free(buffer_temp3);

}


void main(int argc, char** argv) {



	FILE* fileinput;
	FILE* fileout;
	char name_file_input[15], name_file_out[15];

	//read input file name

	printf("Enter the name of the file containing the image.\n");
	gets(name_file_input);
	int bmp = strlen(name_file_input);
	name_file_input[bmp] = '.';
	name_file_input[++bmp] = 'b';
	name_file_input[++bmp] = 'm';
	name_file_input[++bmp] = 'p';
	name_file_input[++bmp] = '\0';

	// read out file name
	printf("Enter the name of the file you want to write the result to.\n");
	gets(name_file_out);
	bmp = strlen(name_file_out);
	name_file_out[bmp] = '.';
	name_file_out[++bmp] = 'b';
	name_file_out[++bmp] = 'm';
	name_file_out[++bmp] = 'p';
	name_file_out[++bmp] = '\0';


	// check file
	if ((fileinput = fopen(name_file_input, "rb")) == NULL)
	{
		printf("Incorect name of file input");
		exit(-1);
	}
	if ((fileout = fopen(name_file_out, "wb+")) == NULL)
	{
		printf("Incorect name of file out");
		exit(-1);
	}

	//copy metadata
	fseek(fileinput, 0, SEEK_SET);
	uint64_t* temp;
	for (int i = 0; i < 9; i++) {
		fread(&temp, sizeof(uint8_t), 6, fileinput);
		fwrite(&temp, sizeof(uint8_t), 6, fileout);
	}

	// read legth, height from input image
	int length_input_image, height_input_image;
	fseek(fileinput, 18, SEEK_SET);
	fread(&length_input_image, sizeof(int), 1, fileinput);
	fread(&height_input_image, sizeof(int), 1, fileinput);
	fseek(fileinput, 54, SEEK_SET);

	// create pixel map for image 
	struct pixel** pixelmap_image;
	pixelmap_image = (struct pixel**)malloc(height_input_image * sizeof(struct pixel*));
	if (pixelmap_image == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < height_input_image; i++) {
		pixelmap_image[i] = (struct pixel*)malloc(length_input_image * sizeof(struct pixel));
		if (pixelmap_image[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}

	// create pixelmap as massive of integers (sum rgb for every pixel)
	int** integer_pixelmap_image;
	integer_pixelmap_image = (int**)malloc(height_input_image * sizeof(int*));
	if (integer_pixelmap_image == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < height_input_image; i++) {
		integer_pixelmap_image[i] = (int*)malloc(length_input_image * sizeof(int));
		if (integer_pixelmap_image[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}

	// sum rgb (white = 765 -> 0, other colors = black -> 1)
	for (int i = 0; i < height_input_image; i++) {
		for (int j = 0; j < length_input_image; j++) {
			fread(&(pixelmap_image[i][j].r), sizeof(uint8_t), 1, fileinput);
			fread(&(pixelmap_image[i][j].g), sizeof(uint8_t), 1, fileinput);
			fread(&(pixelmap_image[i][j].b), sizeof(uint8_t), 1, fileinput);
			integer_pixelmap_image[i][j] = pixelmap_image[i][j].r + pixelmap_image[i][j].g + pixelmap_image[i][j].b;
			if (integer_pixelmap_image[i][j] == 765) {
				integer_pixelmap_image[i][j] = 0;
			}
			else {
				integer_pixelmap_image[i][j] = 1;
				pixelmap_image[i][j].r = 0;
				pixelmap_image[i][j].g = 0;
				pixelmap_image[i][j].b = 0;
			}
		}
	}

	// count areas, assign int of area for every pixel in area
	int count_of_colors = 2;
	identify_every_area(integer_pixelmap_image, length_input_image, height_input_image, &count_of_colors);

	// create matrix of smej
	int sizeof_matrix_of_smej = count_of_colors - 2;
	printf("Count of areas: %d\n", sizeof_matrix_of_smej);
	uint8_t** matrix_of_smej;
	matrix_of_smej = (uint8_t**)calloc(sizeof_matrix_of_smej, sizeof(uint8_t*));
	if (matrix_of_smej == NULL) {
		printf("malloc error");
		exit(-1);
	}
	for (int i = 0; i < sizeof_matrix_of_smej; i++) {
		matrix_of_smej[i] = (uint8_t*)calloc(sizeof_matrix_of_smej, sizeof(uint8_t));
		if (matrix_of_smej[i] == NULL) {
			printf("malloc error");
			exit(-1);
		}
	}

	// identify slots for areas in matrix of smej
	find_neighboring_areas(integer_pixelmap_image, matrix_of_smej, length_input_image, height_input_image, sizeof_matrix_of_smej);

	// correct matrix of smej
	for (int i = 0; i < sizeof_matrix_of_smej; i++) {
		matrix_of_smej[i][i] = 1;
	}
	for (int i = 0; i < sizeof_matrix_of_smej; i++) {
		for (int j = 0; j < sizeof_matrix_of_smej; j++) {
			if (matrix_of_smej[i][j] == 1) {
				matrix_of_smej[j][i] = 1;
			}
			if (matrix_of_smej[j][i] == 1) {
				matrix_of_smej[i][j] = 1;
			}

		}

	}



	// try to combinate areas in groups
	find_groups_of_areas(integer_pixelmap_image, matrix_of_smej, sizeof_matrix_of_smej, length_input_image, height_input_image);



	uint8_t colors[5][3];
	colors[0][0] = 255;
	colors[0][1] = 0;
	colors[0][2] = 0;

	colors[1][0] = 255;
	colors[1][1] = 0;
	colors[1][2] = 255;

	colors[2][0] = 0;
	colors[2][1] = 255;
	colors[2][2] = 0;

	colors[3][0] = 0;
	colors[3][1] = 0;
	colors[3][2] = 255;

	colors[4][0] = 0;
	colors[4][1] = 255;
	colors[4][2] = 255;


	// write to output file
	fseek(fileout, 54, SEEK_SET);
	for (int i = 0; i < height_input_image; i++) {
		for (int j = 0; j < length_input_image; j++) {
			if (integer_pixelmap_image[i][j] != 1) {
				pixelmap_image[i][j].r = colors[integer_pixelmap_image[i][j] - sizeof_matrix_of_smej - 4][0];
				pixelmap_image[i][j].g = colors[integer_pixelmap_image[i][j] - sizeof_matrix_of_smej - 4][1];
				pixelmap_image[i][j].b = colors[integer_pixelmap_image[i][j] - sizeof_matrix_of_smej - 4][2];
			}
			fwrite(&(pixelmap_image[i][j].r), sizeof(uint8_t), 1, fileout);
			fwrite(&(pixelmap_image[i][j].g), sizeof(uint8_t), 1, fileout);
			fwrite(&(pixelmap_image[i][j].b), sizeof(uint8_t), 1, fileout);
		}
	}

	// free memory, close files

	for (int i = 0; i < sizeof_matrix_of_smej; i++) {
		free(matrix_of_smej[i]);
	}
	free(matrix_of_smej);
	for (int i = 0; i < height_input_image; i++) {
		free(integer_pixelmap_image[i]);
	}
	free(integer_pixelmap_image);
	for (int i = 0; i < height_input_image; i++) {
		free(pixelmap_image[i]);
	}
	free(pixelmap_image);
	fclose(fileinput);
	fclose(fileout);
}
