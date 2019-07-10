const  { Producer } = require('./producer');

class MeasurableState extends Producer {
  constructor(measurable = {}) {
    super({});

    this.measurable = measurable;
  }

  async main() {
    return true;
  }
}

module.exports = {
  MeasurableState,
};
