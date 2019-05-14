const API_URL = process.env.API_URL;
const BOT_TOKEN = process.env.BOT_TOKEN;
const CDF_COMBINE_SIZE = process.env.CDF_COMBINE_SIZE || 1000;
const BOT_AGENT_ID = process.env.BOT_AGENT_ID;
const AGGR_SCHEDULE = process.env.AGGR_SCHEDULE || '0 * * * *';

if (!API_URL) throw new ReferenceError('API_URL is not defined');
if (!BOT_AGENT_ID) throw new ReferenceError('BOT_AGENT_ID is not defined');

module.exports = {
  API_URL,
  BOT_TOKEN,
  CDF_COMBINE_SIZE,
  BOT_AGENT_ID,
  AGGR_SCHEDULE,
};
