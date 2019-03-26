const channels = require('./channels');

const { Rule } = require('graphql-shield/dist/rules');

describe('authorizers index', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  describe('index', () => {
    it('index', () => {
      expect(channels).toBeInstanceOf(Object);
      expect(channels.authorize).toBeInstanceOf(Function);
      expect(channels.isChannelAllowed).toBeInstanceOf(Rule);
    });
  });

  describe('authorize', () => {
    it('authorize', () => {
      expect(channels.authorize()).toBe(false);
      expect(channels.authorize({ isPublic: true })).toBe(true);
      expect(channels.authorize({ isPublic: false })).toBe(false);
      expect(channels.authorize({ isPublic: false })).toBe(false);
      expect(channels.authorize({ isPublic: false }, {})).toBe(true);
    });
  });

  describe('isChannelAllowedRule', () => {
    const root = {};
    const args = {};
    const context = {
      channel: { id: 'id1' },
      agentChannel: { agentId: 'agentId2' },
    };
    const info = {};
    beforeAll(() => {
      jest.spyOn(channels, 'authorize').mockReturnValue(true);
    });
    it('isChannelAllowedRule', () => {
      return channels.isChannelAllowedRule(root, args, context, info).then((res) => {
        expect(channels.authorize).toHaveBeenCalledWith(
          context.channel,
          context.agentChannel,
        );
        expect(res).toBe(true);
      });
    });
  });

});
