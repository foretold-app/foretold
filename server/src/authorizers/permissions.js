const { shield } = require('graphql-shield');

// const { isChannelAllowed } = require('./channels');

function getPermissions() {
  return shield({
    Query: {
      //
    },
  });
}

module.exports = {
  getPermissions,
};
