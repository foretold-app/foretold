# Foretold Application

## Run Docker Development Environment

- Create file "docker.compose.override.yml" from 
  "docker.compose.override.example.yml" and set up env vars.
- Run `docker-compose up -d --build`
- Run `docker-compose ps` there should be containers in the Up state.
- Run `docker-compose exec app yarn server/db:create` to create DB
- Run `docker-compose exec app yarn server/db:migrate` to migrate DB schema
- Run `docker-compose logs --tail 100 -f app` to see application logs.

Noq you could change the code and the application will be reloaded. The DB saves
its state into ".persistence" folder. Do not remove this folder if you want
to save DB state.

After work
- Run `docker-compose down` to remove all stuff.
- Run `rm -rf ./.persistence` to remove application volumes.
