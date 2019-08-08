const {
  NOTIFICATION_ERROR_REASON,
} = require('../../models/enums/notification-error-reason');

class CustomError extends Error {
  constructor(message) {
    super(message);
    this.type = NOTIFICATION_ERROR_REASON.INTERNAL_ERROR;
    this.weight = 1;
  }
}

class InternalError extends CustomError {
  constructor(message) {
    super(message);
    this.type = NOTIFICATION_ERROR_REASON.INTERNAL_ERROR;
    this.weight = 1;
  }
}

class ExternalError extends CustomError {
  constructor(message) {
    super(message);
    this.type = NOTIFICATION_ERROR_REASON.EXTERNAL_ERROR;
    this.weight = 1;
  }
}

class PreferencesError extends CustomError {
  constructor(message) {
    super(message);
    this.type = NOTIFICATION_ERROR_REASON.PREFERENCES_ERROR;
    this.weight = 3;
  }
}

class EmailAddressError extends CustomError {
  constructor(message) {
    super(message);
    this.type = NOTIFICATION_ERROR_REASON.EMAIL_ADDRESS_ERROR;
    this.weight = 3;
  }
}

function assert(predicate, msg, ErrorClass = InternalError) {
  if (predicate === false) {
    throw new ErrorClass(msg);
  }
}

module.exports = {
  assert,
  errs: {
    CustomError,
    InternalError,
    ExternalError,
    PreferencesError,
    EmailAddressError,
  },
  NOTIFICATION_ERROR_REASON,
};
