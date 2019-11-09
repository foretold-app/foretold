import _ from 'lodash';

import { parse } from './formatter/index';
import { samplerTypes } from './types';

//TODO(fix this class)

class Guesstimator {
  constructor({ parsedError, parsedInput }) {
    this.parsedError = parsedError || {};
    this.parsedInput = parsedInput;
  }

  static parse(unparsedInput) {
    const [parsedError, parsedInput] = parse(unparsedInput);
    const newItem = new this({ parsedError, parsedInput });
    return [parsedError, newItem];
  }

  hasParsingErrors() {
    return !_.isEmpty(this.parsedError);
  }

  samplerType() {
    return samplerTypes.find(this.parsedInput.guesstimateType);
  }

  needsExternalInputs() {
    return (this.parsedInput.guesstimateType === 'FUNCTION');
  }

  sample(n, externalInputs = {}) {
    if (!_.isEmpty(this.parsedError)) {
      return Promise.resolve({ errors: [this.parsedError], values: [] });
    }

    const samplerType = this.samplerType();
    return samplerType.sampler.sample(this.parsedInput, n, externalInputs);
  }
}

export { Guesstimator };
