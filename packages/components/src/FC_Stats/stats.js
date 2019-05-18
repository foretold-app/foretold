import {Pdf, Cdf, CdfCombination} from "@foretold/cdf";


function cdfToPdf({xs, ys}){
    let cdf = new Cdf(xs, ys);
    let pdf = cdf.toPdf();
    return {xs: pdf.xs, ys: pdf.ys}
}

function findY({xs, ys}, x){
    let cdf = new Cdf(xs, ys);
    let result = cdf.findY(x);
    return result
}

function findX({xs, ys}, y){
    let cdf = new Cdf(xs, ys);
    let result = cdf.findX(y);
    return result
}

module.exports = {cdfToPdf, findY, findX};