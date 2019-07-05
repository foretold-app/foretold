const jwt = require('jsonwebtoken');

const config = require('../config');

class Jwt {
  constructor() {
    this.jwt = jwt;

    this.AUTH0_SECRET = config.AUTH0_SECRET;
    this.JWT_SECRET = config.JWT_SECRET;
    this.JWT_ISSUER = config.JWT_ISSUER;
    this.JWT_EXPIN = config.JWT_EXPIN;
  }

  /**
   * @public
   * @param {string} token
   * @return {boolean}
   */
  validate(token = '') {
    const pattern = /^[A-Za-z0-9-_=]+\.[A-Za-z0-9-_=]+\.?[A-Za-z0-9-_.+/=]*$/;
    return pattern.test(token);
  }

  /**
   * @param {string} token
   * @return {boolean}
   */
  decodeAuth0JwtToken(token) {
    try {
      return this.jwt.verify(token, this.AUTH0_SECRET);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {string} token
   * @return {boolean}
   */
  decodeJwtToken(token) {
    try {
      return this.jwt.verify(token, this.JWT_SECRET);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {object} [payload]
   * @param {string} subject
   * @param {string | null} [expiresIn]
   * @return {string}
   */
  encodeJWT(payload = {}, subject, expiresIn = this.JWT_EXPIN) {
    const options = {
      subject,
      issuer: this.JWT_ISSUER,
    };
    if (expiresIn) options.expiresIn = expiresIn;
    return this.jwt.sign(payload, this.JWT_SECRET, options);
  }
}

module.exports = {
  Jwt,
};
