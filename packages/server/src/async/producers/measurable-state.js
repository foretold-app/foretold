const  { Producer } = require('./producer');


class MeasurableState extends Producer {
  constructor(measurable = {}) {
    super({});
    this.measurable = measurable;
    this.templateName = this.TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED;
  }

  async main() {
    try {
      const creator = await this.measurable.getCreator();
      console.log(`Creator ID = "${creator.id}"`);

      const template = await this.getTemplate();
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
