import { simulate } from './Simulator';

export var Sampler = {
  /**
   * @param low
   * @param high
   * @param n
   * @param _1
   * @returns {*}
   */
  sample({ params: [low, high] }, n, _1) {
    return simulate(`uniform(${low},${high})`, [], n);
  }
};

