class ResponseAll {
  constructor (data, total) {
    this.data = data;
    this.total = total;
  }

  getData() {
    return this.data;
  }

  getTotal() {
    return this.total;
  }
}

module.exports = {
  ResponseAll,
};
