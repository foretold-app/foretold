const { BotModel } = require('./bot-model');

describe('Bot Model', () => {
  it('class should be a constructor', () => {
    expect(BotModel).toBeInstanceOf(Function);
  });
});
