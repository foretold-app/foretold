class ResponseAll {
  constructor (data, total) {
    this.data = data;
    this.total = total;
  }

  getDate() {
    return this.data;
  }

  getTotal() {
    return this.total;
  }
}

module.exports = {
  ResponseAll,
};
