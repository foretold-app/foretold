const { API } = require('./api');

class AggregationBot {
  constructor() {
    this.api = new API(AggregationBot.token);
  }

  async main() {
    await this.api.measurables();
    return true;
  }
}

AggregationBot.token = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1NTU2NzU2MDcsImlzcyI6IkZvcmV0b2xkIiwic3ViIjoiYThhODhmYzItYjQ4NC00OTY2LWE3ODItZGFlM2MzZDg3MjRlIn0.Y1WsWH59R9b9w4KzJp8M6wE99_rJduxXpKe379R3RXk';

module.exports = {
  AggregationBot,
};
