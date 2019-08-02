const { Producer } = require('../producer');

class ProducerFeedItems extends Producer {

  constructor(options = {}) {
    super(options);
  }

}

module.exports = {
  ProducerFeedItems,
};
