const { samplerTypes } = require('./types.js');

describe('Types', () => {

  describe('.samplerTypes', () => {
    it('has many samplerTypes', () => {
      expect(samplerTypes.all.length)
        .toBeGreaterThan(4);
    });
  });

});
