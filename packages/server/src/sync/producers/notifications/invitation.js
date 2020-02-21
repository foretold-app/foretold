const assert = require('assert');
const _ = require('lodash');

const { getChannelLink } = require('../../../../config/client-urls');
const { getAgentLink } = require('../../../../config/client-urls');

const { Producer } = require('../producer');
const { ProducerNotifications } = require('./producer-notifications');
const logger = require('../../../lib/log');

const log = logger.module('sync/producers/notifications/invitation');

class Invitation extends ProducerNotifications {
  /**
   * @param {Defs.Invitation} invitation
   */
  constructor(invitation) {
    super({});

    assert(_.isObject(invitation),
      'Invitation should be an object.');
    assert(_.get(invitation, 'email'),
      'Invitation should have an email.');
    assert(_.get(invitation, 'inviterAgentId'),
      'Invitation should have an inviter agent id.');
    assert(_.get(invitation, 'channelId'),
      'Invitation should have a channel id.');

    this.invitation = invitation;
    this.templateName = Producer.TEMPLATE_NAME.NEW_INVITATION;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      /** @type {Defs.Channel} */
      const channel = await this.channels.getOne({
        id: this.invitation.channelId,
      });
      /** @type {Defs.Agent} */
      const inviter = await this.agents.getOne({
        id: this.invitation.inviterAgentId,
      });

      assert(!!_.get(channel, 'id'), 'Channel ID is required.');
      assert(!!_.get(inviter, 'id'), 'Inviter ID is required.');

      const replacements = await this._getReplacements(
        inviter,
        channel,
      );

      const email = this.invitation.email;

      /** @type {Defs.Notification} */
      const notification = await this._queueEmail(replacements, email);
      await this._assignNotification(null, notification);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      log.trace(this.constructor.name, e.message, e);
      return false;
    }
  }

  /**
   * @protected
   * @param {Defs.Agent} inviter
   * @param {Defs.Channel} channel
   * @return {object}
   */
  async _getReplacements(inviter, channel) {
    return {
      inviterAgent: {
        name: (await _.get(inviter, 'name', null)) || 'Somebody',
        link: getAgentLink(inviter),
      },
      channel: {
        name: _.get(channel, 'name', null) || 'Channel',
        link: getChannelLink(channel),
      },
    };
  }
}

module.exports = {
  Invitation,
};
