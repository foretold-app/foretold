const agents = require('./agents');
const { AgentsData } = require('../../data');

describe('Agents Resolvers', () => {

  describe('one() when id passed through arguments', () => {
    const root = {};
    const args = { id: 'id1' };
    it('returns an agent by id', () => {
      return agents.one(root, args).then((result) => {
        expect(new AgentsData().getOne).toHaveBeenCalledWith({ id: 'id1' });
        expect(result).toBe(true);
      });
    });
  });

  describe('one() when agentId passed through root', () => {
    const root = { agentId: 'agentId1' };
    const args = {};
    it('returns an agent by agentId', () => {
      return agents.one(root, args).then((result) => {
        expect(new AgentsData().getOne).toHaveBeenCalledWith({ id: 'agentId1' });
        expect(result).toBe(true);
      });
    });
  });
});
