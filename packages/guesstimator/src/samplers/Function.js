import { simulate } from './Simulator';

export var Sampler = {
  /**
   * @param text
   * @param n
   * @param inputs
   * @returns {*}
   */
  sample({ text }, n, inputs) {
    return simulate(text, inputs, n);
  }
};
