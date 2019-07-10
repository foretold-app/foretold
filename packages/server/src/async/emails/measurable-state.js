const  { Emails } = require('./emails');

class MeasurableState extends Emails {
  constructor(measurable = {}) {
    const options = {};
    super(options);

    this.measurable = measurable;
  }

  async main() {
    return true;
  }
}

module.exports = {
  MeasurableState,
};
