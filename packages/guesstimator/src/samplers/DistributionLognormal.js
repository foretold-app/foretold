import { simulate } from './Simulator';
import { distributionUpToIntoLognormal } from '../lib/distributionMath.js'

export var Sampler = {
  /**
   * @param low
   * @param high
   * @param n
   * @param _1
   * @returns {*}
   */
  sample({ params: [low, high] }, n, _1) {
    return simulate(distributionUpToIntoLognormal(low, high), [], n)
  }
};
