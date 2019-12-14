import _ from 'lodash';

import { parse } from './formatter/index';
import { samplerTypes } from './types';

//TODO(fix this class)
// @todo: How to fix this class? What is the issue?

class Guesstimator {
  /**
   * @param parsedError
   * @param parsedInput
   */
  constructor({ parsedError, parsedInput }) {
    this.parsedError = parsedError || {};
    this.parsedInput = parsedInput;
  }

  /**
   * @param unparsedInput
   * @returns {({}|{subType: number, type: number}|Guesstimator)[]}
   */
  static parse(unparsedInput) {
    const [parsedError, parsedInput] = parse(unparsedInput);
    const newItem = new this({ parsedError, parsedInput });
    return [parsedError, newItem];
  }

  /**
   * @todo: Use this function everywhere.
   * @returns {boolean}
   */
  hasParsingErrors() {
    return !_.isEmpty(this.parsedError);
  }

  /**
   * @public
   * @returns {*}
   */
  samplerType() {
    return samplerTypes.find(this.parsedInput.guesstimateType);
  }

  /**
   * @returns {boolean}
   */
  needsExternalInputs() {
    return this.parsedInput.guesstimateType === 'FUNCTION';
  }

  /**
   * @param n
   * @param externalInputs
   * @returns {Promise<{values: [], errors: [*]}>|*}
   */
  sample(n, externalInputs = {}) {
    if (!_.isEmpty(this.parsedError)) {
      return { errors: [this.parsedError], values: [] };
    }

    const samplerType = this.samplerType();
    return samplerType.sampler.sample(this.parsedInput, n, externalInputs);
  }
}

export { Guesstimator };
