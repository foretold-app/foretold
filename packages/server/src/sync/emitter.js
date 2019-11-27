const EventEmitter = require('events');

class EventEmitterAsync extends EventEmitter {
  /**
   * @public
   * @param name
   * @param args
   * @returns {Promise<boolean>}
   */
  async emitAsync(name, ...args) {
    const listeners = this.listeners(name);
    for (let i = 0, max = listeners.length; i < max; i++) {
      await listeners[i](...args);
    }
    return true;
  }
}

const emitter = new EventEmitterAsync();

module.exports = emitter;
