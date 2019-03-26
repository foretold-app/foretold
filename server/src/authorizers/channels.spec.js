const channels = require('./channels');

describe('tests channel authorizers', () => {

  describe('tests constructors', () => {
    it('should return constructors', () => {
      expect(channels).toBeInstanceOf(Object);
      expect(channels.authorize).toBeInstanceOf(Function);
    });
  });

  describe('tests authorize function', () => {
    it('should return false if there is no input', () => {
      expect(channels.authorize()).toBe(false);
    });
    it('should return true if channel is public', () => {
      expect(channels.authorize({ isPublic: true })).toBe(true);
    });
    it('should return false if there is no agent-channel input object', () => {
      expect(channels.authorize({ isPublic: false })).toBe(false);
    });
    it('should return true if all is ok', () => {
      expect(channels.authorize({ isPublic: false }, {})).toBe(true);
    });
  });

  describe('tests isChannelAllowedRule function', () => {
    const root = {};
    const args = {};
    const context = {
      channel: { id: 'id1' },
      agentChannel: { agentId: 'agentId2' },
    };
    const info = {};
    beforeEach(() => {
      jest.spyOn(channels, 'authorize').mockReturnValue(true);
    });
    it('should call authorize function', () => {
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
