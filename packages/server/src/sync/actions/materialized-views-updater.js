const logger = require('../../lib/log');

const { ChannelAgentsData } = require('../../data');
const { AgentChannelsData } = require('../../data');
const { AgentMeasurablesData } = require('../../data');

class MaterializedViewsUpdater {
  constructor() {
    this.channelAgentsData = new ChannelAgentsData();
    this.agentChannelsData = new AgentChannelsData();
    this.agentMeasurablesData = new AgentMeasurablesData();
    this.log = logger.module('sync/actions/materialized-views-updater');
  }

  /**
   * @returns {Promise<boolean>}
   */
  async main() {
    try {
      await this.channelAgentsData.updateMaterializedView();
      await this.agentChannelsData.updateMaterializedView();
      await this.agentMeasurablesData.updateMaterializedView();
    } catch (e) {
      this.log.error(e);
    }
    return true;
  }
}

module.exports = {
  MaterializedViewsUpdater,
};
