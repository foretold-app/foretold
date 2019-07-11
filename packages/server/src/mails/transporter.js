const nodemailer = require('nodemailer');

/**
 * @tested
 * @param {SmtpGateway} confIn
 * @return {object}
 */
function transportFactory(confIn) {
  console.log('Transporter is ready for production version');

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
 * @param {SmtpGateway} confIn
 * @return {{close(), sendMail(*, *): void}}
 */
function transportFakeFactory(confIn) {
  console.log('Transporter is ready for development version');

  return new class {
    sendMail(options) {
    }

    close() {
    }
  };
}

const transporter = process.env.NODE_ENV === "development"
  ? transportFakeFactory
  : transportFactory;

module.exports = {
  transporter,
};
