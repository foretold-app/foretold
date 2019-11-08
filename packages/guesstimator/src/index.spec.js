const _ = require('lodash');
const { Guesstimator } = require('./index.js');

describe('Guesstimator', () => {
  describe('.parse', () => {
    it('works with a simple function', () => {
      const input = { text: '=34' };
      const [error, item] = Guesstimator.parse(input);
      expect(_.isEmpty(error))
        .toBe(true);

      const parsedInput = item.parsedInput;
      expect(parsedInput.guesstimateType)
        .toBe('FUNCTION');
      expect(parsedInput.text)
        .toBe('34');
    });
  });

  describe('#samplerType', () => {
    it('has many samplerTypes', () => {
      const input = { text: '=34' };
      const [error, item] = Guesstimator.parse(input);
      expect(item.samplerType().referenceName)
        .toBe('FUNCTION');
    });
  });
});
