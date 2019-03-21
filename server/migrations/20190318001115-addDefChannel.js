const models = require('../src/models');

module.exports = {
  up: async function () {
    const user = await models.User.findOne({
      where: { name: 'Service Account' },
      include: [models.User.Agent]
    });
    let channel = await models.Channel.findOne({
      where: { name: 'unlisted' },
    });

    if (!channel) {
      channel = await models.Channel.create({
        name: 'unlisted',
        creatorId: user.agentId,
        isArchived: false,
        isPublic: false,
      });

      await models.Channel.Agents.set(channel, user.Agent);
    }
  },

  down: async function () {
    await models.Channel.destroy({
      where: { name: 'unlisted' },
    });
  }
};
