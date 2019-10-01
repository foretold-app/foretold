const crypto = require('crypto');

global.crypto = {
  getRandomValues: function(buffer) {
    crypto.randomFill(buffer, function(err, buf){
      if (err) throw err;
    });
  }
};

const ken = require('./dist');

module.exports = ken;
