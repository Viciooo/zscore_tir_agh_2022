# Useful

## Notes

* czytanie z pliku danych
* ustalenie kiedy learning się kończy
* liczenie odchylenia standardowe
* on the fly liczysz

## Online variance algorithm

```python
def online_variance(data):
    n = 0
    mean = 0.0
    M2 = 0.0

    for x in data:
        n += 1
        delta = x - mean
        mean += delta/n
        M2 += delta*(x - mean)

    if n < 2:
        return float('nan')
    else:
        return M2 / (n - 1)
```
