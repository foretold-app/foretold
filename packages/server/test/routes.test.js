const request = require('supertest');

xdescribe('GET /', () => {
  it('should render properly', async () => {
    await request(app).get('/').expect(200);
  });
});

