const channels = require('./channels');

describe('channel authorizers module', () => {

  describe('constructors', () => {
    it('return constructors', () => {
      expect(channels).toBeInstanceOf(Object);
      expect(channels.authorize).toBeInstanceOf(Function);
    });
  });

  describe('authorize()', () => {
    it('returns false if there is no input', () => {
      expect(channels.authorize()).toBe(false);
    });
    it('returns true if channel is public', () => {
      expect(channels.authorize({ isPublic: true })).toBe(true);
    });
    it('returns false if there is no agent-channel input object', () => {
      expect(channels.authorize({ isPublic: false })).toBe(false);
    });
    it('returns true if all is ok', () => {
      expect(channels.authorize({ isPublic: false }, {})).toBe(true);
    });
  });

  describe('isChannelAllowedRule()', () => {
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
    it('calls authorize()', () => {
      return channels.isChannelAllowedRule(root, args, context, info)
        .then((res) => {
          expect(channels.authorize).toHaveBeenCalledWith(
            context.channel,
            context.agentChannel,
          );
          expect(res).toBe(true);
        });
    });
  });

});
