const { AuthenticationError } = require('apollo-server');

const jwt = require('jsonwebtoken');

const config = require('../../config/config');

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
  decodeAuth0Jwt(token) {
    try {
      return this.jwt.verify(token, this.AUTH0_SECRET);
    } catch (err) {
      throw new Jwt.JwtAuth0Invalid();
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
      throw new Jwt.JwtInvalid();
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

Jwt.JwtAuth0Invalid = class JwtAuth0Invalid extends AuthenticationError {
  constructor() {
    super('JWT of Auth0.com is invalid');
  }
};

Jwt.JwtInvalid = class JwtInvalid extends AuthenticationError {
  constructor() {
    super('JWT is invalid');
  }
};

module.exports = {
  Jwt,
};
