const _ = require('lodash');
const moment = require('moment');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewMeasurable extends ProducerFeedItems {
  /**
   * @param {Defs.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);

    this.measurable = measurable;
    this.templateName = Producer.TEMPLATE_NAME.NEW_MEASURABLE_B_FEED_ITEM;
    this.FeedItem = Producer.FeedItemMeasurableB;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return !_.get(this.measurable, 'seriesId', null);
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
    const agentName = (await _.get(agent, 'name', null))
      || 'Somebody';
    const measurableName = (await _.get(this.measurable, 'name', null))
      || 'Question';

    return {
      agent: {
        name: agentName,
      },
      measurable: {
        id: this.measurable.id,
        name: measurableName,
      },
    };
  }

  /**
   * @return {Promise.<object>}
   * @protected
   */
  async _getInputs() {
    const labelSubject = _.get(this.measurable, 'labelSubject', null);
    const labelProperty = _.get(this.measurable, 'labelProperty', null);
    const labelCustom = _.get(this.measurable, 'labelCustom', null);
    const labelStartAtDate$ = _.get(this.measurable, 'labelStartAtDate', null);
    const labelEndAtDate$ = _.get(this.measurable, 'labelEndAtDate', null);
    const labelConditionals = _.get(this.measurable, 'labelConditionals', null);

    const labelStartAtDate = moment(labelStartAtDate$).toISOString();
    const labelEndAtDate = moment(labelEndAtDate$).toISOString();

    return {
      labelSubject,
      labelProperty,
      labelCustom,
      labelStartAtDate,
      labelEndAtDate,
      labelConditionals,
    };
  }
}

module.exports = {
  NewMeasurable,
};
