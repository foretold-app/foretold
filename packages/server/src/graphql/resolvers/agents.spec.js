const agents = require('./agents');
const data = require('../../data');

describe('Agents Resolvers', () => {

  beforeEach(() => {
    jest.spyOn(data.agents, 'getOne').mockReturnValue(
      Promise.resolve(true),
    );
  });

  describe('one() when id passed through arguments', () => {
    const root = {};
    const args = { id: 'id1' };
    it('returns an agent by id', () => {
      return agents.one(root, args).then((result) => {
        expect(data.agents.getOne).toHaveBeenCalledWith({ id: 'id1' });
        expect(result).toBe(true);
      });
    });
  });

  describe('one() when agentId passed through root', () => {
    const root = { agentId: 'agentId1' };
    const args = {};
    it('returns an agent by agentId', () => {
      return agents.one(root, args).then((result) => {
        expect(data.agents.getOne).toHaveBeenCalledWith({ id: 'agentId1' });
        expect(result).toBe(true);
      });
    });
  });

});
