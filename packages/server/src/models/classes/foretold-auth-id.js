class ForetoldAuthId {
  constructor(email = 'no@email.com') {
    this.email = email;
  }

  toString() {
    return `foretold|${this.email}`;
  }
}

module.exports = {
  ForetoldAuthId,
};
