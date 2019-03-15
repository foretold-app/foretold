import request from 'supertest';

xdescribe('GET /', () => {
  it('should render properly', async () => {
    await request(app).get('/').expect(200);
  });
});

