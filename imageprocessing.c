#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"

void dezalocare(int ***x, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(x[i][j]);
        }
        free(x[i]);
    }
    free(x);
}
int ***alocare(int N, int M) {
    int c = 0, h = 0;
    int ***x = (int ***)malloc(N * sizeof(int **));
    if (!x)
        return NULL;
    for (int i = 0; i < N; i++) {
        x[i] = (int **)malloc(M * sizeof(int *));
        if (x[i] == NULL) {
            for (c = 0; c < i; c++)
                free(x[c]);
            free(x);
            return NULL;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            x[i][j] = (int *)malloc(3 * sizeof(int));
            if (x[i][j] == NULL) {
                for (c = 0; c < j; c++) {
                    free(x[i][c]);
                }
                free(x[i]);
                if (i != 0) {
                    for (c = 0; c < i; c++) {
                        for (h = 0; h < M; h++)
                            free(x[c][h]);
                        free(x[c]);
                    }
                }
                free(x);
                return NULL;
            }
        }
    }
    return x;
}
int ***flip_horizontal(int ***image, int N, int M) {
    long k = 0, i = 0, j = 0;
    int aux = 0;
    for (k = 0; k < 3; k++) {
        for (i = 0; i < N; i++)
            for (j = 0; j <(M/2); j++) {
                    aux = image[i][j][k];
                    image[i][j][k] = image[i][M-j-1][k];
                    image[i][M-j-1][k] = aux;
                }
    }
    return image;
    return NULL;
}
int ***rotate_left(int ***image, int N, int M) {
    int i = 0, j  = 0, k = 0;
    int ***b = alocare(M, N);
    if (b == NULL)
        return NULL;
    for (k = 0; k < 3; k++)
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++)
                    b[M-1-j][i][k] = image[i][j][k];
    dezalocare(image, N, M);
    image = alocare(M, N);
    if (image == NULL)
        return NULL;
    for (k = 0; k < 3; k++)
        for (i = 0; i < M; i++)
            for (j = 0; j < N; j++)
                image[i][j][k] = b[i][j][k];
    dezalocare(b, M, N);
    return image;
    return NULL;
}
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int i = 0, j = 0, k = 0, new_M = w, new_N = h;
    int ***b = alocare(new_N, new_M);
    if (b == NULL) {
        free(b);
        return NULL;
    }
    for (k = 0; k < 3; k++)
        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++)
                b[i][j][k] = image[i+y][j+x][k];
    dezalocare(image, N, M);
    image = alocare(new_N, new_M);
    if (image == NULL)
        return NULL;
    for (k = 0; k < 3; k++)
        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++)
                image[i][j][k] = b[i][j][k];
    dezalocare(b, new_N, new_M);
    return image;
    return NULL;
}
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int new_M = 0 , new_N = 0;
    new_N = N + 2 * rows;
    new_M = M + 2 * cols;
    int k = 0, i = 0, j = 0;
    int ***b = alocare(new_N, new_M);
    if (b == NULL) {
        free(b);
        return NULL;
    }
    for (k = 0; k < 3; k++)
        for (i = 0; i < new_N; i++)
            for (j = 0; j < new_M; j++) {
                if (k == 0)
                    b[i][j][k] = new_R;
                if (k == 1)
                    b[i][j][k] = new_G;
                if (k == 2)
                    b[i][j][k] = new_B;
            }
    for (k = 0; k < 3; k++)
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++)
                b[i+rows][j+cols][k] = image[i][j][k];
    dezalocare(image, N, M);
    image = alocare(new_N, new_M);
    if (image == NULL) {
        free(image);
        return NULL;
    }
    for (k = 0; k < 3; k++)
        for (i = 0; i < new_N; i++)
            for (j = 0; j < new_M; j++)
                image[i][j][k] = b[i][j][k];
    dezalocare(b, new_N, new_M);
    return image;
    return NULL;
}
int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
    int i = 0, j = 0, k = 0;
    for (i = y; i < N_dst; i++)
        for (j = x; j < M_dst; j++)
            for (k = 0; k < 3; k++)
                if ((j -x < M_src)&&(i - y < N_src))
                    image_dst[i][j][k] = image_src[i-y][j-x][k];
    return image_dst;
    return NULL;
}
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    float termen = 0, suma = 0;
    int k = 0, i = 0, j = 0, i2 = 0, j2 = 0;
    int ***new_image = alocare(N, M);
    const int MAX_char = 255;
    for (k = 0; k < 3; k++)
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++) {
                suma = 0;
                for (i2 = 0; i2 < filter_size; i2++) {
                    for (j2 = 0; j2 < filter_size; j2++) {
                        if ((i-(filter_size/2)+i2 < 0) || (j-(filter_size/2)+j2 < 0))
                            termen = 0;
                        else
                        if ((j-(filter_size/2)+j2 >= M) || (i-(filter_size/2)+i2 >= N))
                            termen = 0;
                        else
                            termen = filter[i2][j2]*(float)image[i-(filter_size/2)+i2][j-(filter_size/2)+j2][k];
                        suma = suma + termen;
                    }
                    new_image[i][j][k] = (int)suma;
                    if (suma < 0)
                        new_image[i][j][k] = 0;
                    if (suma > (float)MAX_char)
                        new_image[i][j][k] = MAX_char;
                }
            }
    dezalocare(image, N, M);
    return new_image;
    return NULL;
}
