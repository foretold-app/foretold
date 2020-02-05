const sync = require('./sync');
const server = require('./server');

function run() {
  sync.run();
  server.run();
}

module.exports = {
  run,
};
