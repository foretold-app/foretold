const users = require('./users');
const { UsersData } = require('../../data');

describe('Users Resolvers', () => {
  describe('users()', () => {
    const root = {};
    const args = { id: 'id1', input: { name: 'name1' } };
    const context = { user: { agentId: 'agentId1' } };

    it('edits user', () => users.update(root, args, context).then((result) => {
      expect(new UsersData().updateOne).toHaveBeenCalledWith(
        { id: 'id1' },
        { name: 'name1' },
      );
      expect(result).toBe(true);
    }));
  });
});
