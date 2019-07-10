const  { Consumer } = require('./consumer');

class Emails extends Consumer {
  constructor(measurable = {}) {
    super({});

    this.measurable = measurable;
  }

  async main() {
    return true;
  }
}

module.exports = {
  Emails,
};
