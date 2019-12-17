# CDF

This library helps convert samples to cumulative distribution functions and 
probability density functions, and then perform various functions on them. 
It uses the [pdfast](https://www.npmjs.com/package/pdfast) library to perform 
**Kernel Density Estimation** using triangular kernel.

## Samples

Manipulate sets of samples representing a stochastic variable.

Options:

- **min**: min value. This does not use pdfast's min, but rather removes all 
of the samples below it. - max: max value. This does not use pdfast's max, 
but rather removes all of the samples above it. This means that the resulting 
PDF or CDF may have some mass outside this range, due to KDE.
- **size**: number of points to represent the pdf or cdf. Defaults to 50.
- **width**: determine how many points to the left and right does an element affect, 
similar to bandwidth in kernel density estimation. Defaults to 2.

```javascript
let mySamples = new Samples([3, 4, 5, 6, 7, 8, 8, 8, 10, 11]);
let pdf = mySamples.toPdf({ min: 3, max: 8, width: 10, size: 1000 });
let pdfXs = pdf.xs;
let pdfYs = pdf.ys;
let cdf = mySamples.toCdf({ min: 3, max: 8, width: 10, size: 1000 });

// Get the 2nd percentile.
mySamples.getPercentile(2);

// Get the 95th percentile.
mySamples.getPercentile(95);

// Create a new Samples object, with the filter implemented.
mySamples.filter({ min: 3, max: 8 });
```
