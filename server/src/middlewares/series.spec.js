const { series } = require('./series');
const data = require('../data');

describe('Series Middleware', () => {

  describe('series()', () => {
    const serieObj = { channelId: 'channelId1' };
    beforeEach(() => {
      jest.spyOn(data.seriesData, 'getOne').mockReturnValue(
        Promise.resolve(serieObj),
      );
    });

    it('when arguments are passed', () => {
      const root = {};
      const args = { id: 'serieId1' };
      const context = {};
      const info = {};
      return series(root, args, context, info).then((result) => {
        expect(data.seriesData.getOne).toHaveBeenCalledWith('serieId1');
        expect(result).toBe(undefined);
        expect(context.series).toBe(serieObj);
        expect(context.channelId).toBe(serieObj.channelId);
      });
    });

  });

});
