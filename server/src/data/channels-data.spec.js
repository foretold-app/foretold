jest.mock('../models');

const models = require('../models');
const { ChannelsData } = require('./channels-data');
const { AgentsChannelsData } = require('./agents-channels-data');

describe('ChannelsData', () => {
  afterEach(() => {
    jest.clearAllMocks();
    jest.restoreAllMocks();
  });

  it('class should be constructor', () => {
    expect(ChannelsData).toBeInstanceOf(Function);
  });

  const instance = new ChannelsData();

  describe('createOne - branch A', () => {
    beforeAll(() => {
      jest.spyOn(models.Channel, 'findOne').mockReturnValue(Promise.resolve(true));
      jest.spyOn(models.Channel, 'create').mockReturnValue(Promise.resolve(true));
      jest.spyOn(AgentsChannelsData.prototype, 'createOne').mockReturnValue(Promise.resolve(true));
    });
    it('createOne', () => {
      const user = {agentId: 'agentId1'};
      const input = { name: 'Input Name 1'};
      return instance.createOne(user, input).then((result) => {
        expect(models.Channel.findOne).toHaveBeenCalledWith({where: { name: input.name }});
        expect(models.Channel.create).toHaveBeenCalledTimes(0);
        expect(AgentsChannelsData.prototype.createOne).toHaveBeenCalledTimes(0);
        expect(result).toBe(true);
      });
    });
  });

});

