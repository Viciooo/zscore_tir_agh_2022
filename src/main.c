#define _GNU_SOURCE
#define BUF_SIZE 255
#define TRAIN_DATA_SIZE 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct POINT {
    float x;
    float y;
    float z;
};

struct POINT train_data[TRAIN_DATA_SIZE];

int count_lines(FILE *file) {
    char buf[BUF_SIZE];
    int counter = 0;
    for (;;) {
        size_t res = fread(buf, 1, BUF_SIZE, file);
        if (ferror(file))
            return -1;

        int i;
        for (i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }

    return counter;
}

int read_data() {
    FILE *filePointer;

    char buffer[BUF_SIZE]; /* not ISO 90 compatible */
    filePointer = fopen("../data/train.csv", "r");

    int points_added = 0;
    while (fgets(buffer, BUF_SIZE, filePointer)) {
        char *pt;
        pt = strtok(buffer, ",");
        float x = atof(pt);
        pt = strtok(NULL, ",");
        float y = atof(pt);
        pt = strtok(NULL, ",");
        float z = atof(pt);

        struct POINT point = {x, y, z};
        train_data[points_added] = point;

        printf("x: %f y: %f z: %f\n",
               train_data[points_added].x,
               train_data[points_added].y,
               train_data[points_added].z);
        points_added++;

    }

    fclose(filePointer);
    return 0;
}


int main(void) {
    read_data();
    return 0;
}