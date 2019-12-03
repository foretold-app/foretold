/* eslint-disable max-classes-per-file */
const { AuthenticationError } = require('apollo-server');


class NoUserIdError extends AuthenticationError {
  constructor() {
    super('No User Id');
  }
}

class NotAuthenticatedError extends AuthenticationError {
  constructor() {
    super('Not authenticated');
  }
}

class TokenIsInvalidError extends AuthenticationError {
  constructor() {
    super('Token is invalid');
  }
}

class NoAgentIdError extends AuthenticationError {
  constructor() {
    super('No Agent Id');
  }
}

/* eslint-enable max-classes-per-file */

module.exports = {
  NoUserIdError,
  NotAuthenticatedError,
  TokenIsInvalidError,
  NoAgentIdError,
};
