import math from 'mathjs'
import {simulate} from './Simulator'
import {distributionUpToIntoLognormal} from '../lib/distributionMath.js'

export var Sampler = {
  sample({params: [low, high]}, n, _1) {
    return simulate(distributionUpToIntoLognormal(low, high), [], n)
  }
}
