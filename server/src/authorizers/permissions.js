const { shield, allow } = require('graphql-shield');

const users = require('./users');

function getPermissions() {
  return shield({
    Query: {
      '*': allow,
      user: users.isAuthenticated,
    },
    Mutation: {
      '*': users.isAuthenticated,
    }
  });
}

module.exports = {
  getPermissions,
};
