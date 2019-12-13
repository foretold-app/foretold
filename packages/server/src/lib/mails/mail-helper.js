const _ = require('lodash');
const Mustache = require('mustache');

const { emailConfig } = require('./email-config');
const { transporter } = require('./transporter');
const { SmtpGateways } = require('./smtp-gateways');
const logger = require('../../lib/log');

const log = logger.module('lib/mails/mail-helper');

class MailHelper {
  /**
   * @param {string} [authToken]
   * @param {object} [replacements]
   * @param {string} [template]
   * @param {string} [to]
   * @param {string} [subject]
   */
  constructor(
    {
      authToken = '',
      replacements = {},
      template = '',
      to = '',
      subject = '',
    } = {},
  ) {
    this.authToken = authToken;
    this.replacements = replacements;
    this.template = template;
    this.to = to;
    this.subject = subject;

    this.config = emailConfig;
    this.gateway = new SmtpGateways(emailConfig).getDefault();

    this.transporter = transporter(this.gateway);
    this.from = _.get(this.gateway, 'from');
  }

  /**
   * @tested
   * @param {string} [target]
   * @param {string} [subject]
   * @param {string} [contentHtml]
   * @return {Promise<*>}
   */
  main(
    target = this.to,
    subject = this.subject,
    contentHtml = this._getHtml(),
  ) {
    log.trace('______sendMail____');

    const mailOptions = {
      subject,
      to: target,
      html: contentHtml,
      from: this.from,
      generateTextFromHTML: true,
      list: this._listHeaders(),
    };

    log.trace('mailOptions.body', JSON.stringify(mailOptions));

    return new Promise((resolve, reject) => {
      this.transporter.sendMail(mailOptions, (error, response) => {
        this.transporter.close();
        if (error) {
          console.error(`send email error: ${JSON.stringify(error)}`);
          return reject(error);
        }
        log.trace(`send email result: ${JSON.stringify(response)}`);
        return resolve(response);
      });
    });
  }

  /**
   * @returns {string}
   * @private
   */
  _getHtml() {
    return Mustache.render(this.template, this._getReplacements());
  }

  /**
   * @returns {{
   *  subscribe: {link: string},
   *  unsubscribe: {link: string}
   * }}
   * @private
   */
  _getReplacements() {
    return {
      ...this.replacements,
      subscribe: {
        link: this._getSubscribeLink(),
      },
      unsubscribe: {
        link: this._getUnsubscribeLink(),
      },
    };
  }

  /**
   * @protected
   * @docs https://nodemailer.com/message/list-headers/
   * @returns {{
   *  help: string,
   *  unsubscribe: {comment: string, url: string},
   *  subscribe: {url: string, comment: string}[]
   * }}
   */
  _listHeaders() {
    return {
      // List-Help: <mailto:admin@example.com?subject=help>
      help: this._compileStr(this.config.emailHelp),

      // List-Unsubscribe: <http://example.com> (Comment)
      unsubscribe: {
        url: this._getUnsubscribeLink(),
        comment: this._compileStr(this.config.unsubscribeComment),
      },

      // List-Subscribe: <mailto:admin@example.com?subject=subscribe>
      // List-Subscribe: <http://example.com> (Subscribe)
      subscribe: [
        // this._compileStr(this.config.subscribeEmail),
        {
          url: this._getSubscribeLink(),
          comment: this._compileStr(this.config.subscribeComment),
        },
      ],
    };
  }

  /**
   * @returns {string}
   * @private
   */
  _getSubscribeLink() {
    return this._compileStr(this.config.subscribeLink);
  }

  /**
   * @returns {string}
   * @private
   */
  _getUnsubscribeLink() {
    return this._compileStr(this.config.unsubscribeLink);
  }

  /**
   * @protected
   * @param {string} [str]
   * @returns {string}
   */
  _compileStr(str = '') {
    return this._setAuthToken(str);
  }

  /**
   * @protected
   * @param {string} [str]
   * @returns {string}
   */
  _setAuthToken(str = '') {
    return str.replace('{{token}}', this.authToken);
  }
}

module.exports = {
  MailHelper,
};
