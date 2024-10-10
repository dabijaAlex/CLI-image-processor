#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
int main() {
    int n = 0, ok = 1, n2 = 0, i = 0, j = 0, k = 0, l = 0, idx = 0;
    const int nr_de_la_mine = 100;
    const int al_doilea_nr_de_la_mn = 10;
    int auxiliar = 0;
    struct val_dp_index {
        int N, M, size;
    }v[nr_de_la_mine];
    char nume_comanda[al_doilea_nr_de_la_mn], path[nr_de_la_mine];
    int ****img_col = malloc(nr_de_la_mine*sizeof(int ***));
    float ***flt_c = malloc(nr_de_la_mine*sizeof(float **));
    scanf("%s", nume_comanda);
    if (nume_comanda[0] == 'e')
        ok = 0;
    while (ok) {
        if (nume_comanda[0] == 'e') {
            ok = 0;
            break;
        } else if (nume_comanda[0] == 'l') {
            scanf("%d%d", &v[n].N, &v[n].M);
            scanf("%s", path);
            img_col[n] = alocare(v[n].N, v[n].M);
            if (img_col[n] == NULL) {
                free(img_col[n]);
                return -1;
            }
            read_from_bmp(img_col[n], v[n].N, v[n].M, path);
            n = n+1;
        } else if (nume_comanda[0] == 's') {
            scanf("%d", &idx);
            scanf("%s", path);
            write_to_bmp(img_col[idx], v[idx].N, v[idx].M, path);
        } else if (strcmp("ah", nume_comanda) == 0) {
            scanf("%d", &idx);
            img_col[idx] = flip_horizontal(img_col[idx], v[idx].N, v[idx].M);
        } else if (strcmp("ar", nume_comanda) == 0) {
            scanf("%d", &idx);
            auxiliar = v[idx].M;
            v[idx].M = v[idx].N;
            v[idx].N = auxiliar;
            img_col[idx] = rotate_left(img_col[idx], v[idx].M, v[idx].N);
        } else if (strcmp("ac", nume_comanda) == 0) {
            scanf("%d", &idx);
            int x = 0, y = 0, w = 0, h = 0;
            scanf("%d%d%d%d", &x, &y, &w, &h);
            img_col[idx] = crop(img_col[idx], v[idx].N, v[idx].M, x, y, h, w);
            v[idx].M = w;
            v[idx].N = h;
        } else if (strcmp("ae", nume_comanda) == 0) {
            int rows = 0, cols = 0, R = 0, B = 0, G = 0;
            scanf("%d", &idx);
            scanf("%d%d%d%d%d", &rows, &cols, &R, &G, &B);
            img_col[idx] = extend(img_col[idx], v[idx].N, v[idx].M, rows, cols, R, G, B);
            v[idx].N = rows*2+v[idx].N;
            v[idx].M = cols*2+v[idx].M;
        } else if (strcmp("ap", nume_comanda) == 0) {
            int id_s = 0, id_d = 0, x = 0, y = 0;
            scanf("%d%d%d%d", &id_d, &id_s, &x, &y);
            img_col[id_d] = paste(img_col[id_d], v[id_d].N, v[id_d].M, img_col[id_s], v[id_s].N, v[id_s].M, x, y);
        } else if (strcmp("cf", nume_comanda) == 0) {
            int size = 0;
            scanf("%d", &size);
            flt_c[n2] = malloc(size*sizeof(float*));
            if (!flt_c[n2]) {
                free(flt_c[n2]);
                return -1;
            }
            for (i = 0; i < size; i++) {
                flt_c[n2][i] = malloc(size*sizeof(float));
                if (flt_c[n2][i] == NULL) {
                    for (j = 0; j < i; j++)
                        free(flt_c[n2][j]);
                    free(flt_c[n2]);
                    return -1;
                }
            }
            for (i = 0; i < size; i++)
                for (j = 0; j < size; j++)
                    scanf("%f", &flt_c[n2][i][j]);
            v[n2].size = size;
            n2++;
        } else if (strcmp("af", nume_comanda) == 0) {
            int id_i = 0, id_f = 0;
            scanf("%d%d", &id_i, &id_f);
            img_col[id_i] = apply_filter(img_col[id_i], v[id_i].N, v[id_i].M, flt_c[id_f], v[id_f].size);
        } else if (strcmp("df", nume_comanda) == 0) {
            int index_filter = 0;
            scanf("%d", &index_filter);
            for (k = index_filter; k < (n2-1); k++) {
                for (i = 0; i < v[k].size; i++)
                    free(flt_c[k][i]);
                free(flt_c[k]);
                flt_c[k] = malloc(v[k+1].size*sizeof(float*));
                if (flt_c[k] == NULL) {
                    free(flt_c[k]);
                    return -1;
                }
                for (i = 0; i < v[k+1].size; i++) {
                    flt_c[k][i] = malloc(v[k+1].size*sizeof(float));
                    if (flt_c[k][i] == NULL) {
                        for (j = 0; j < i; j++)
                            free(flt_c[k][j]);
                        free(flt_c[k]);
                        return -1;
                    }
                }
                v[k].size = v[k+1].size;
                for (i = 0; i < v[k].size; i++)
                    for (j = 0; j < v[k].size; j++)
                        flt_c[k][i][j] = flt_c[k+1][i][j];
            }
            for (i = 0; i < v[n2-1].size; i++)
                    free(flt_c[n2-1][i]);
                free(flt_c[n2-1]);
            n2--;
        } else if (strcmp("di", nume_comanda) == 0) {
            int index_img = 0;
            scanf("%d", &index_img);
            for (l = index_img; l < n-1; l++) {
                dezalocare(img_col[l], v[l].N, v[l].M);
                img_col[l] = alocare(v[l+1].N, v[l+1].M);
                v[l].N = v[l+1].N;
                v[l].M = v[l+1].M;
                for (k = 0; k < 3; k++)
                    for (i = 0; i < v[l].N; i++)
                        for (j = 0; j < v[l].M; j++)
                            img_col[l][i][j][k] = img_col[l+1][i][j][k];
            }
            dezalocare(img_col[n-1], v[n-1].N, v[n-1].M);
            n--;
        }
        scanf("%s", nume_comanda);
    }
    for (i = 0; i < n; i++)
        dezalocare(img_col[i], v[i].N, v[i].M);
    free(img_col);
    for (k = 0; k < n2; k++) {
        for (i = 0; i < v[k].size; i++)
            free(flt_c[k][i]);
        free(flt_c[k]);
    }
    free(flt_c);
    return 0;
}
