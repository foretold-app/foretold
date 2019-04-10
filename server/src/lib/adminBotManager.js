const models = require("../models")

const USERS = {
    "ADMIN": "Admin Bot Coordinator"
}

const BOTS = {
    "RESOLUTION_BOT": "Resolution Bot"
}

//Not currently used, because get requests create on demand.
const setup = async function () {
    const user = await models.User.create({name: USERS.ADMIN, auth0Id: ""});
    await models.Bot.create({userId: user.id, name: BOTS.RESOLUTION_BOT});
}

const getUser = async function(symbol){
    const users = await models.User.findAll({
        where: {
            name: USERS[symbol],
          }
    })
    if (users.length == 0){
        const user = await models.User.create({name: USERS[symbol], auth0Id: ""});
        return user
    } else {
        return users[0];
    }
}

const getBot = async function(symbol){
    const bots = await models.Bot.findAll({
        where: {
            name: BOTS[symbol],
          }
    })
    if (bots.length == 0){
        const user = getUser("ADMIN");
        const bot = await models.Bot.create({userId: user.id, name: BOTS[symbol]});
        return bot
    } else {
        return bots[0];
    }
}

module.exports = {
  getUser,
  getBot,
}
