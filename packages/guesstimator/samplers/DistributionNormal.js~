import math from 'mathjs'
import {simulate} from './Simulator'
import {distributionUpToIntoNormal} from '../lib/distributionMath.js'

export var Sampler = {
  sample({params: [low, high]}, n, _1) {

    return simulate(distributionUpToIntoNormal(low, high), [], n)
  }
}
