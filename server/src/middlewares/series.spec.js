const { serie } = require('./series');
const data = require('../data');

describe('Series Middleware', () => {

  describe('serie()', () => {
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
      return serie(root, args, context, info).then((result) => {
        expect(data.seriesData.getOne).toHaveBeenCalledWith('serieId1');
        expect(result).toBe(undefined);
        expect(context.serie).toBe(serieObj);
        expect(context.channelId).toBe(serieObj.channelId);
      });
    });

  });

});
