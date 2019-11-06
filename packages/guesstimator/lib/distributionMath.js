import math from 'mathjs'

// The function: distributionUpToIntoLognormal
// assumes a centered 90% confidence interval, e.g. the left endpoint
// marks 0.05% on the CDF, the right 0.95%.

export function distributionUpToIntoLognormal(low, high){

    let logHigh = math.log(high)
    let logLow = math.log(low)

    let mean = (math.mean(logHigh, logLow))
    let stdev = ((logHigh-logLow) / (2*1.645))

    return `lognormal(${mean},${stdev})`
}
