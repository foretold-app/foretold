const { channel } = require('./channels');
const data = require('../data');

describe('channela', () => {

  describe('channel ', () => {
    const channelObj = {};
    beforeAll(() => {
      jest.spyOn(data.channelsData, 'getOne').mockReturnValue(Promise.resolve(channelObj));
    });

    it('A', () => {
      const root = {};
      const args = { channelId: 'channelId1' };
      const context = {};
      const info = {};
      return channel(root, args, context, info).then((result) => {
        expect(data.channelsData.getOne).toHaveBeenCalledWith('channelId1');
        expect(result).toBe(undefined);
        expect(context.channel).toBe(channelObj);
      });
    });

    it('B', () => {
      const root = { channelId: 'channelId1' };
      const args = {};
      const context = {};
      const info = {};
      return channel(root, args, context, info).then((result) => {
        expect(data.channelsData.getOne).toHaveBeenCalledWith('channelId1');
        expect(result).toBe(undefined);
        expect(context.channel).toBe(channelObj);
      });
    });

    it('C', () => {
      const root = {};
      const args = {};
      const context = { channelId: 'channelId1' };
      const info = {};
      return channel(root, args, context, info).then((result) => {
        expect(data.channelsData.getOne).toHaveBeenCalledWith('channelId1');
        expect(result).toBe(undefined);
        expect(context.channel).toBe(channelObj);
      });
    });

  });

});

