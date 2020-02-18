const { Samples } = require('./lib/samples');

let mySamples = new Samples([3, 4, 5, 6, 7, 7, 7, 7, 7, 8, 8, 8, 10, 11]);
mySamples.sort();
let pdf = mySamples.toPdf({ min: 3, max: 8, width: 10, size: 1000 });
let cdf = mySamples.toCdf({ min: 3, max: 8, width: 10, size: 50 });

// Get the 2nd percentile.
// console.log(mySamples.getPercentile(2));

// Get the 95th percentile.
// console.log(mySamples.getPercentile(95));

// Create a new Samples object, with the filter implemented.
// console.log(mySamples.filter({ min: 3, max: 8 }));

console.log("CDF");

// console.log(cdf);
console.log(cdf.findX(0.95));
console.log(cdf.findX(0.50));
console.log(cdf.findX(0.05));

console.log("PDF");

// console.log(pdf);
console.log(pdf.findX(0.95));
console.log(pdf.findX(0.50));
console.log(pdf.findX(0.05));