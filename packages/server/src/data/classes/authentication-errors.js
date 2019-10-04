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

module.exports = {
  NoUserIdError,
  NotAuthenticatedError,
  TokenIsInvalidError,
  NoAgentIdError,
};
