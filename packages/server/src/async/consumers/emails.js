const assert = require('assert');
const _ = require('lodash');

const { Consumer } = require('./consumer');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');

class Emails extends Consumer {
  constructor() {
    super({});
  }

  async main() {
    try {
      const transaction = await this.data.notifications.getTransaction();

      const filter = new Filter({ type: this.NOTIFICATION_TYPE.EMAIL });
      const pagination = new Pagination({ limit: 10 });
      const options = new Options({ transaction });
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
