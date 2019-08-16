# Foretold Server

## Run Docker Development Environment

- Create file docker.compose.override.yml from docker.compose.override.example.yml and set up env vars and ports.
- Run `docker-compose up -d --build`
- Run `docker-compose ps` there should be containers in the Up state.
- Run `docker-compose exec app npm run server/db:create` to create DB
- Run `docker-compose exec app npm run server/db:migrate` to migrate DB schema
- Run `docker-compose logs --tail 100 -f app` to see application logs.
- Set up packages/server/config.json
