"use strict";

var _lodash = require("lodash");

var _lodash2 = _interopRequireDefault(_lodash);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

const models = require("../models");
const Sequelize = require('sequelize');
const fetch = require("node-fetch");
const { setup, getBot } = require("../lib/adminBotManager");

const Op = Sequelize.Op;

class Resolution {
  async run() {
    const resolutionBot = await getBot("RESOLUTION_BOT");
    const resolutionAgent = await resolutionBot.getAgent();
    const measurables = await models.Measurable.needsResolutionResponse();
    for (const m of measurables) {
      await m.processResolution(resolutionAgent.id);
    }
  }
}

let resolution = new Resolution();
resolution.run();
//# sourceMappingURL=resolution.js.map