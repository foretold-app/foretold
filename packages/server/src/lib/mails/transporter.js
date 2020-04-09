const nodemailer = require('nodemailer');
const logger = require('../log');

const log = logger.module('lib/github');

/**
 * @tested
 * @param {SmtpGateway} confIn
 * @return {object}
 */
function transportFactory(confIn) {
  log.trace('Transporter is ready for production version');

  return nodemailer.createTransport({
    host: confIn.host,
    port: confIn.port,
    secure: true,
    auth: {
      user: confIn.user,
      pass: confIn.pass,
    },
    tls: {
      rejectUnauthorized: false,
    },
  });
}

/**
 * @tested
 * @param {SmtpGateway} _confIn
 * @return {{close(), sendMail(*, *): void}}
 */
function transportFakeFactory(_confIn) {
  log.trace('Transporter is ready for development version');

  return new class {
    sendMail(_options) {
    }

    close() {
    }
  }();
}

const transporter = process.env.NODE_ENV === 'development'
  ? transportFakeFactory
  : transportFactory;

module.exports = {
  transporter,
};
