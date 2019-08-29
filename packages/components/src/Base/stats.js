import {Pdf, Cdf, ContinuousDistributionCombination} from "@foretold/cdf";

function cdfToPdf({xs, ys}){
    let cdf = new Cdf(xs, ys);
    let pdf = cdf.toPdf();
    return {xs: pdf.xs, ys: pdf.ys}
}

function mean(var1, var2){
    let cdf1= new Cdf(var1.xs, var1.ys);
    let cdf2 = new Cdf(var2.xs, var2.ys);
    let comb = new ContinuousDistributionCombination([cdf1, cdf2]);
    let newCdf = comb.combineYsWithMean(1000);
    return {xs: newCdf.xs, ys: newCdf.ys}
}

function findY(x, {xs, ys}){
    let cdf = new Cdf(xs, ys);
    let result = cdf.findY(x);
    return result
}

function findX(y, {xs, ys}){
    let cdf = new Cdf(xs, ys);
    let result = cdf.findX(y);
    return result
}

module.exports = {cdfToPdf, findY, findX, mean};