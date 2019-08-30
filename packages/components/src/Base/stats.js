import {Pdf, Cdf, ContinuousDistribution, ContinuousDistributionCombination} from "@foretold/cdf";

function cdfToPdf({xs, ys}){
    let cdf = new Cdf(xs, ys);
    let pdf = cdf.toPdf();
    return {xs: pdf.xs, ys: pdf.ys}
}

function mean(vars){
    let cdfs = vars.map(r => new Cdf(r.xs, r.ys));
    let comb = new ContinuousDistributionCombination(cdfs);
    let newCdf = comb.combineYsWithMean(10000);
    return {xs: newCdf.xs, ys: newCdf.ys}
}

function divideBy(vars){
    let cdfs = vars.map(r => (new Cdf(r.xs, r.ys)).toPdf());
    let comb = new ContinuousDistributionCombination(cdfs);
    let newPdf = comb.combineYsWithFn(10000, r => Math.log2(r[0] / r[1]) * r[2]);
    let newCdf = (new Pdf(newPdf.xs, newPdf.ys)).toCdf();
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

function integral({xs, ys}){
    let distribution = new ContinuousDistribution(xs, ys);
    return distribution.integral();
}

module.exports = {cdfToPdf, findY, findX, mean, divideBy, integral};