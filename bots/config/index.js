const API_URL = process.env.API_URL;
const BOT_TOKEN = process.env.BOT_TOKEN;

if (!API_URL) throw new ReferenceError('API_URL is not defined');

module.exports = {
  API_URL,
  BOT_TOKEN,
};
