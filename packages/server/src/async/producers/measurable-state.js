const  { Producer } = require('./producer');

const { EmailEnvelope } = require('../../models/classes/notifications');

class MeasurableState extends Producer {
  constructor(measurable = {}) {
    super({});

    this.measurable = measurable;
  }

  async main() {
    try {
      const creator = await this.measurable.getCreator();
      console.log(`Creator ID = "${creator.id}"`);



    } catch (e) {
      console.log(`MeasurableState`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  MeasurableState,
};
