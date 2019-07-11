const _ = require('lodash');
const pug = require('pug');

const { emailConfig } = require('./email-config');
const { transporter } = require('./transporter');
const { SmtpGateways } = require('./smtp-gateways');

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
    } = {}
  ) {
    this.authToken = authToken;
    this.replacements = replacements;
    this.template = template;
    this.to = to;
    this.subject = subject;
    this.html = '';

    this.config = emailConfig;
    this.gateway = new SmtpGateways(emailConfig).getDefault();

    this.transporter = transporter(this.gateway);
    this.from = _.get(this.gateway, 'from');
  }

  /**
   * @returns {Promise<* | never>}
   */
  main() {
    this.buildTemplate();
    return this.sendMail();
  }

  /**
   * @returns {void}
   */
  buildTemplate() {
    this.html = pug.compile(this.template)(this.replacements);
  }

  /**
   * @tested
   * @param {string} [target]
   * @param {string} [subject]
   * @param {string} [contentHtml]
   * @return {Promise<*>}
   */
  sendMail(
    target = this.to,
    subject = this.subject,
    contentHtml = this.html,
  ) {
    console.log('______sendMail____');

    const mailOptions = {
      subject,
      to: target,
      html: contentHtml,
      from: this.from,
      generateTextFromHTML: true,
      list: this._ListHeaders(),
    };

    console.log(
      `mailOptions.body: ***********\n` +
      JSON.stringify(mailOptions, null, '\n') +
      `\n***********`
    );

    return new Promise((resolve, reject) => {
      this.transporter.sendMail(mailOptions, (error, response) => {
        this.transporter.close();
        if (error) {
          console.error(`send email error: ${JSON.stringify(error)}`);
          return reject(error);
        }
        console.log(`send email result: ${JSON.stringify(response)}`);
        resolve(response);
      });
    });
  }

  /**
   * @protected
   * @docs https://nodemailer.com/message/list-headers/
   * @returns {{help: string, unsubscribe: {comment: string, url: string}, subscribe: *[]}}
   */
  _ListHeaders() {
    return {
      // List-Help: <mailto:admin@example.com?subject=help>
      help: this._compileStr(this.config.emailHelp),

      // List-Unsubscribe: <http://example.com> (Comment)
      unsubscribe: {
        url: this._compileStr(this.config.unsubscribeLink),
        comment: this._compileStr(this.config.unsubscribeComment),
      },

      // List-Subscribe: <mailto:admin@example.com?subject=subscribe>
      // List-Subscribe: <http://example.com> (Subscribe)
      subscribe: [
        this._compileStr(this.config.subscribeEmail),
        {
          url: this._compileStr(this.config.subscribeLink),
          comment: this._compileStr(this.config.subscribeComment)
        }
      ],
    }
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
