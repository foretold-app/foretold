const { isAuthenticatedRule } = require('./agents');

describe('Agents Authorizers', () => {

  describe('isAuthenticatedRule()', () => {
    it('returns true when user model exists within context', () => {
      const root = {};
      const args = {};
      const context = { agent: { id: 'id1' } };
      const info = {};
      return isAuthenticatedRule(root, args, context, info).then((result) => {
        expect(result).toBe(true);
      })
    });
    it('returns false when user model does not exist within context', () => {
      const root = {};
      const args = {};
      const context = {};
      const info = {};
      return isAuthenticatedRule(root, args, context, info).then((result) => {
        expect(result).toBe(false);
      })
    });
  });

});
