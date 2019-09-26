const { ChannelMembershipsData } = require('./channel-memberships-data');

describe('Data Layer - Channel Memberships Data', () => {
  it('class should be a constructor', () => {
    expect(ChannelMembershipsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelMembershipsData();

  describe('getAllChannelIds()', () => {
    const options = {};
    beforeEach(() => {
      jest.spyOn(instance, 'getAll2').mockReturnValue(Promise.resolve([
        { channelId: 'channelId1' },
      ]));
    });
    it('calls getAll()', () => {
      return instance.getAllChannelIds(options).then((result) => {
        expect(instance.getAll2).toHaveBeenCalledWith(options);
        expect(result).toEqual(['channelId1']);
      });
    });
  });

});
