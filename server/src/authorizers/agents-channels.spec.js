const { isAdminRule } = require('./agents-channels');

describe('AgentsChannels Authorizers', () => {

  describe('isAdminRule()', () => {
    it('returns true when admin role in context is exists', () => {
      const root = {};
      const args = {};
      const context = { agentsChannelsRoles: ['admin'] };
      const info = {};
      return isAdminRule(root, args, context, info).then((result) => {
        expect(result).toBe(true);
      })
    });
    it('returns false when admin role in context is not exists', () => {
      const root = {};
      const args = {};
      const context = { agentsChannelsRoles: ['viewer'] };
      const info = {};
      return isAdminRule(root, args, context, info).then((result) => {
        expect(result).toBe(false);
      })
    });
  });

});
