const EventEmitter = require('events');

const index = require('./emitter');

describe('Emitter Async', () => {
  it('returns an instance', () => {
    expect(index).toBeInstanceOf(EventEmitter);
  });
});
