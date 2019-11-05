import math from 'mathjs'
import {simulate} from './Simulator'
import {distributionUpToIntoLognormal} from '../lib/distributionMath.js'

export var Sampler = {
  sample({params: [low, high]}, n, _1) {
    // This assumes a centered 90% confidence interval, e.g. the left endpoint
    // marks 0.05% on the CDF, the right 0.95%.
    return simulate(distributionUpToIntoLognormal(low, high), [], n)
  }
}
