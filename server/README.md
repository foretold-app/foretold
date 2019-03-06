# Foretold Server
This is a simple Node.js server for the foretold application. It uses graphql/apollo/sequelize.

# To Run
1. Change the config/config.json to match your credentials copying from config.example.json.
2. Run ``npm install``
3. Run ``npm run db:create``
4. Run ``npm run db:migrate``
5. Run ``AUTH0_SECRET={SECRET} npm run dev`` (Make sure to get the secret from Ozzie first)
