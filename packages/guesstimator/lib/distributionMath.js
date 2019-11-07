import math from 'mathjs'

// The left endpoint marks 0.05% on the CDF, the right 0.95%.
export function distributionUpToIntoLognormal(low, high){
    let logHigh = math.log(high)
    let logLow = math.log(low)

    let mean = (math.mean(logHigh, logLow))
    let stdev = ((logHigh-logLow) / (2*1.645))

    return `lognormal(${mean},${stdev})`
}

// The left endpoint marks 0.05% on the CDF, the right 0.95%.
export function distributionUpToIntoNormal(low, high){
    let mean = math.mean(high, low)
    let stdev = (high - mean) / 1.645
    return `normal(${mean},${stdev})`
}