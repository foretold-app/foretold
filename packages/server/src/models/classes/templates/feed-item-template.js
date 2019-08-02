class FeedItemTemplate {
  constructor(options = {}) {
    this.item = options.item;
    this.description = options.description;
  }

  getItem() {
    return this.item;
  }

  getDescription() {
    return this.description;
  }
}

module.exports = {
  FeedItemTemplate,
};
