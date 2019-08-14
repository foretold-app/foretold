const GitHub = require('github-api');

class API extends GitHub {
  async getListOfHooks() {
    const repo = await super.getRepo('foretold-app', 'ken-data');
    const list = await repo.listHooks();
    return list;
  }
}

const api = new API({
  token: '0e869ed656bb78c73a085c1442bc6db75d110160'
});

module.exports = {
  api,
  API,
};
