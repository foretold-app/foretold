# Foretold Server
This is a simple Node.js server for the foretold application. It uses graphql/apollo/sequelize.

# To Run
1. Change the config/config.json to match your credentials.
2. Run ``npm install``
3. Run ``npm run db:create``
4. Run ``npm run db:migrate``
5. Run ``AUTH0_SECRET={SECRET} npm run dev`` (Make sure to get the secret from Ozzie first)

# Run Docker Development Environment
1. Create file docker.compose.override.yml from docker.compose.override.example.yml and set up env vars and ports.
2. Run ``docker-compose up -d --build``
3. Run ``docker-compose ps`` there should be containers in the Up state.
4. Run ``docker-compose logs --tail 100 -f app`` to see application logs.
