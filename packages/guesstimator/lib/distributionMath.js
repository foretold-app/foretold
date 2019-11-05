import math from 'mathjs'

export function distributionUpToIntoLognormal(low, high){

    let logHigh = math.log(high)
    let logLow = math.log(low)

    let mean = (math.mean(logHigh, logLow)).toFixed(3)
    let stdev = ((logHigh-logLow) / (2*1.645)).toFixed(3)

    return `lognormal(${mean},${stdev})`
}
