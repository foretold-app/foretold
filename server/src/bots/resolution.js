const models = require("../models")
const Sequelize = require('sequelize')
const fetch = require("node-fetch");
const {setup, getBot} = require("../lib/adminBotManager");
import _ from "lodash"
const Op = Sequelize.Op

class Resolution {
  async run() {
    const resolutionBot = await getBot("RESOLUTION_BOT");
    const resolutionAgent = await resolutionBot.getAgent();
    const measurables = await models.Measurable.needsResolutionResponse();
    for (const m of measurables){
        await m.processResolution(resolutionAgent.id)
    }
  }
}

let resolution = new Resolution();
resolution.run();