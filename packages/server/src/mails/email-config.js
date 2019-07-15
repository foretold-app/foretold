/** @type {EmailConfig} */
const emailConfig = {
  emailHostGenerator: process.env.EMAIL_HOST_GENERATOR || 'foretold.io',
  emailHelp: process.env.EMAIL_HELP || 'admin@foretold.io?subject=help',
  unsubscribeLink: process.env.UNSUBSCRIBE_LINK || 'https://foretold.io/unsubscribe?token={{token}}',
  unsubscribeComment: process.env.UNSUBSCRIBE_COMMENT || 'Unsubscribe',
  subscribeEmail: process.env.SUBSCRIBE_EMAIL || 'admin@foretold.io?subject=subscribe',
  subscribeLink: process.env.SUBSCRIBE_LINK || 'https://foretold.io/subscribe?token={{token}}',
  subscribeComment: process.env.SUBSCRIBE_COMMENT || 'Subscribe',

  gateways: [
    {
      name: "Default",
      host: process.env.SMTP_HOST || "smtp.host.com",
      port: process.env.SMTP_PORT || "465",
      user: process.env.SMTP_USER || "user@host.com",
      pass: process.env.SMTP_PASS || "<none>",
      from: process.env.SMTP_FROM || "User <user@host.com>",
      default: true
    }
  ],
};

module.exports = {
  emailConfig,
};
