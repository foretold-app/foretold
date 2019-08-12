const API_URL = process.env.API_URL;
const BOT_TOKEN = process.env.BOT_TOKEN;
const CDF_COMBINE_SIZE = process.env.CDF_COMBINE_SIZE || 1000;
const AGGR_SCHEDULE = process.env.AGGR_SCHEDULE || '0 * * * *';

if (!API_URL) throw new ReferenceError('API_URL is not defined');

module.exports = {
  API_URL,
  BOT_TOKEN,
  CDF_COMBINE_SIZE,
  AGGR_SCHEDULE,
};
