const { getDB } = require('./db');
const { init } = require('./init');
const { addHooks } = require('./hooks');

const db = getDB();
init(db);
addHooks(db);

module.exports = db;
