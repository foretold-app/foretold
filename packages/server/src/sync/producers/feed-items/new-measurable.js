const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewMeasurable extends ProducerFeedItems {
  /**
   * @param {Defs.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);

    this.templateName = Producer.TEMPLATE_NAME.NEW_MEASURABLE_B_FEED_ITEM;
    this.FeedItem = Producer.FeedItemMeasurableB;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return !_.get(this.input, 'seriesId', null);
  }

  /**
   * @param {Defs.Agent} agent
   * @return {Promise.<{
   * agent: {name: string},
   * measurable: {name: string, id: string}
   * }>}
   * @protected
   */
  async _getReplacements(agent) {
    const agentName = (await _.get(agent, 'name', null)) || 'Somebody';
    const measurableName = (await _.get(this.input, 'name', null))
      || 'Question';

    const labelSubject = _.get(this.input, 'labelSubject', '');
    const labelProperty = _.get(this.input, 'labelProperty', '');
    const labelCustom = _.get(this.input, 'labelCustom', '');
    const labelStartAtDate = _.get(this.input, 'labelStartAtDate', '');
    const labelEndAtDate = _.get(this.input, 'labelEndAtDate', '');
    const labelConditionals = _.get(this.input, 'labelConditionals', []);

    return {
      agent: {
        name: agentName,
      },
      measurable: {
        id: this.input.id,
        name: measurableName,

        labelSubject,
        labelProperty,
        labelCustom,
        labelStartAtDate,
        labelEndAtDate,
        labelConditionals,
      },
    };
  }
}

module.exports = {
  NewMeasurable,
};
