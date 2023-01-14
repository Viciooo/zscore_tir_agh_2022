# Zscore anomaly detection written in C

## Project description
The goal was to implement an on-the-fly algorithm to compute standard deviation of streamed training dataset and later use that parameter to detect anomalies in streamed test dataset.

To obtain standard deviation on memory-constrained device we are using Welford's online algorithm
```math 
\bar x_n = \frac{(n-1) \, \bar x_{n-1} + x_n}{n} = \bar x_{n-1} + \frac{x_n - \bar x_{n-1}}{n}
```

The following formulas can be used to update the mean and (estimated) variance of the sequence, for an additional element $x_n$ Here, $$\bar x_n = \frac{1}{n} \sum_{i=1}^{n} x_i$$ denotes the sample mean of the first $n$ samples $(x_1,\dots,x_n)$, $$s^2_n = \frac{1}{n - 1} \sum_{i=1}^{n} (x_i - \bar{x}_n)^2$$ their unbiased sample variance.

$$\bar x_n = \frac{(n-1) \, \bar x_{n-1} + x_n}{n} = \bar x_{n-1} + \frac{x_n - \bar x_{n-1}}{n}$$

$$s^2_n = \frac{n-2}{n-1} \, s^2_{n-1} + \frac{(x_n - \bar x_{n-1})^2}{n} = s^2_{n-1} + \frac{(x_n - \bar x_{n-1})^2}{n} - \frac{s^2_{n-1}}{n-1}, \quad n>1$$

These formulas suffer from numerical instability, as they repeatedly subtract a small number from a big number which scales with $n$. A better quantity for updating is the sum of squares of differences from the current mean, $$\sum_{i=1}^n (x_i - \bar x_n)^2$$ here denoted $M_{2,n}$:

$$M_{2,n} = M_{2,n-1} + (x_n - \bar x_{n-1})(x_n - \bar x_n)$$

$$s^2_n = \frac{M_{2,n}}{n-1}$$

Implementation:
```c
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
```
$$s_n = \sqrt{s^2_n} = \sqrt{\frac{M_{2,n}}{n-1}}$$
```c
standard_deviation.x = sqrt(online_m2.x / (TRAIN_DATA_SIZE - 1));
standard_deviation.y = sqrt(online_m2.x / (TRAIN_DATA_SIZE - 1));
standard_deviation.z = sqrt(online_m2.z / (TRAIN_DATA_SIZE - 1));
```

Anomaly detection:
```c
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
```
[Code](./src)

[Data](./data)

## Contributors

* [Piotr Witek](https://github.com/Viciooo)
* [Bartłomiej Chwast](https://github.com/bchwast)
* [Piotr Kowalczyk](https://github.com/pkowalczyk1)
* [Mikołaj Wielgos](https://github.com/wlgs)
