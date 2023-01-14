#define _GNU_SOURCE
#define BUF_SIZE 255
#define TRAIN_DATA_SIZE 30
#define TEST_DATA_SIZE 1000
#define ANOMALY_DISTANCE_FACTOR 3

//#define VERBOSE

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct POINT {
    double x;
    double y;
    double z;
};

struct POINT train_data[TRAIN_DATA_SIZE];
struct POINT test_data[TEST_DATA_SIZE];

struct POINT online_mean = {0.0, 0.0, 0.0};
struct POINT online_m2 = {0.0, 0.0, 0.0};
struct POINT standard_deviation = {0.0, 0.0, 0.0};
int current_count = 0;
int anomalies_found = 0;

void read_data(const char* file_path, struct POINT* arr) {
    FILE *filePointer;

    char buffer[BUF_SIZE]; /* not ISO 90 compatible */
    filePointer = fopen(file_path, "r");

    int points_added = 0;
    while (fgets(buffer, BUF_SIZE, filePointer)) {
        char *pt;
        pt = strtok(buffer, ",");
        double x = atof(pt);
        pt = strtok(NULL, ",");
        double y = atof(pt);
        pt = strtok(NULL, ",");
        double z = atof(pt);

        struct POINT point = {x, y, z};
        arr[points_added] = point;

#ifdef VERBOSE
    printf("read -> ");
    printf("x: %f y: %f z: %f\n",
            train_data[points_added].x,
            train_data[points_added].y,
            train_data[points_added].z);
 #endif
        points_added++;
    }

    fclose(filePointer);
}


void update_mean_and_m2(struct POINT new_point) {
    current_count++;
    struct POINT delta = {new_point.x - online_mean.x, new_point.y - online_mean.y, new_point.z - online_mean.z};
    
    online_mean.x += delta.x / current_count;
    online_mean.y += delta.y / current_count;
    online_mean.z += delta.z / current_count;
    
    online_m2.x += delta.x * (new_point.x - online_mean.x);
    online_m2.y += delta.y * (new_point.y - online_mean.y);
    online_m2.z += delta.z * (new_point.z - online_mean.z);
}

/*
    return value:
        0 - not an anomaly
        1 - anomaly on x axis
        2 - anomaly on y axis
        3 - anomaly on x + y axis
        4 - anomaly on z axis
        5 - anomaly on x + z axis
        7 - anomaly on x + y + z axis
*/
int is_anomaly(struct POINT new_point, unsigned outlier_factor){
    int res = 0;

    if (fabs(new_point.x - online_mean.x) > outlier_factor * standard_deviation.x)
        res += 1;
    if (fabs(new_point.y - online_mean.y) > outlier_factor * standard_deviation.y)
        res += 2;
    if (fabs(new_point.z - online_mean.z) > outlier_factor * standard_deviation.z)
        res += 4;

    if (res > 0) {
        printf("anomaly found -> x: %f y: %f, z: %f -> anomaly type: %d\n",
               new_point.x,
               new_point.y,
               new_point.z,
               res);
        anomalies_found++;
    }
    return res;
}


int main(void) {
    read_data("../data/train.csv", train_data);
    read_data("../data/test.csv", test_data);

    for (int i = 0; i < TRAIN_DATA_SIZE; ++i) {
        update_mean_and_m2(train_data[i]);
#ifdef VERBOSE
        if (i > 0) {
            printf("variance iter:%d -> ", i + 1);
            printf("x: %f y: %f z: %f\n",
                   online_m2.x / i,
                   online_m2.y / i,
                   online_m2.z / i);
        }
#endif
    }
    printf("final mean -> ");
    printf("x: %f y: %f z: %f\n",
           online_mean.x,
           online_mean.y,
           online_mean.z);
    printf("final variance -> ");
    printf("x: %f y: %f z: %f\n",
        online_m2.x / (TRAIN_DATA_SIZE - 1),
        online_m2.y / (TRAIN_DATA_SIZE - 1),
        online_m2.z / (TRAIN_DATA_SIZE - 1));

    standard_deviation.x = sqrt(online_m2.x / (TRAIN_DATA_SIZE - 1));
    standard_deviation.y = sqrt(online_m2.x / (TRAIN_DATA_SIZE - 1));
    standard_deviation.z = sqrt(online_m2.z / (TRAIN_DATA_SIZE - 1));
    printf("final sd -> ");
    printf("x: %f y: %f z: %f\n",
           standard_deviation.x,
           standard_deviation.y,
           standard_deviation.z);

    for (int i = 0; i< TEST_DATA_SIZE; ++i) {
        int anomaly = is_anomaly(test_data[i], ANOMALY_DISTANCE_FACTOR);
#ifdef VERBOSE
        printf("id%d anomaly check -> %d\n", i, anomaly);
#endif
    }
    printf("Total anomalies -> %d\n", anomalies_found);

    return 0;
}