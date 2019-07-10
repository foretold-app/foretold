const assert = require('assert');
const _ = require('lodash');

const { Consumer } = require('./consumer');
const { Pagination } = require('../../pagination');

class Emails extends Consumer {
  constructor() {
    super({});
  }

  async main() {
    try {
      const transaction = await this.data.notifications.getTransaction();

      const filter = { type: this.NOTIFICATION_TYPE.EMAIL };
      const pagination = new Pagination({ limit: 10 });
      const options = { transaction };
      const notifications = await this.data.notifications.getAll(filter, pagination, options);

    } catch (e) {
      console.log(`Emails Consumer`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  Emails,
};
