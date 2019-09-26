const common = require('./common');

describe('Common Resolvers', () => {
  describe('iAmOwner() when creatorId passed through root', () => {
    const root = { creatorId: 'creatorId1' };
    const args = {};
    const context = { agent: { id: 'creatorId1' } };
    const info = {};
    it('returns yes', () => {
      return common.iAmOwner(root, args, context, info).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

  describe('iAmOwner() when agentId passed through root', () => {
    const root = { agentId: 'agentId2' };
    const args = {};
    const context = { agent: { id: 'agentId2' } };
    const info = {};
    it('returns yes', () => {
      return common.iAmOwner(root, args, context, info).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

  describe('iAmOwner() when nothing is passed', () => {
    const root = {};
    const args = {};
    const context = { agent: { id: 'agentId3' } };
    const info = {};
    it('returns false', () => {
      return common.iAmOwner(root, args, context, info).then((result) => {
        expect(result).toBe(false);
      });
    });
  });

  describe('isMe() when agentId is passed thought root', () => {
    const root = { agentId: 'agentId4' };
    const args = {};
    const context = { agent: { id: 'agentId4' } };
    const info = {};
    it('returns true', () => {
      return common.isMe(root, args, context, info).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

  describe('isMe() when id is passed thought root', () => {
    const root = { id: 'id' };
    const args = {};
    const context = { agent: { id: 'id' } };
    const info = {};
    it('returns true', () => {
      return common.isMe(root, args, context, info).then((result) => {
        expect(result).toBe(true);
      });
    });
  });

  describe('isMe() when id is passed thought root but not the same', () => {
    const root = { id: 'id5' };
    const args = {};
    const context = { user: { agentId: 'id5' } };
    const info = {};
    it('returns false', () => {
      return common.isMe(root, args, context, info).then((result) => {
        expect(result).toBe(false);
      });
    });
  });

  describe('isMe() when nothing is passed', () => {
    const root = {};
    const args = {};
    const context = { user: { agentId: 'id6' } };
    const info = {};
    it('returns false', () => {
      return common.isMe(root, args, context, info).then((result) => {
        expect(result).toBe(false);
      });
    });
  });
});
