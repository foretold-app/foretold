const models = require('../src/models');

module.exports = {
  up: async function () {
    const user = await models.User.findOne({
      where: { name: 'Service Account' },
    });
    await models.Channel.findOrCreate({
      where: { name: 'unlisted' },
      defaults: {
        name: 'unlisted',
        agentId: user.agentId,
        creatorId: user.agentId,
        isArchived: false,
        isPublic: false,
      },
    });
  },

  down: async function () {
    await models.Channel.destroy({
      where: { name: 'unlisted' },
    });
  }
};
