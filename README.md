# Foretold Application

## Run Docker Development Environment

- Create file "docker.compose.override.yml" from 
  "docker.compose.override.example.yml".
- Run `docker-compose up -d --build`.
- Run `docker-compose up -d --scale app=3` or `docker-compose scale app=3`.
- Run `docker-compose ps` there should be containers in the Up state.
- Run `docker-compose exec app yarn server/db:create` to create DB.
- Run `docker-compose exec app yarn server/db:migrate` to migrate DB schema.
- Run `docker-compose restart app`.
- Run `docker-compose logs --tail 100 -f app` to see application logs.
- Run `docker-compose logs --tail 100 -f` to see all logs.
- Open "http://localhost/graphql" to see the graphql browser. 
  Do not block "cookies".
- Open "http://localhost" to see the application.
- Now you could change some configuration in "docker.compose.override.yml".
  Then run `docker-compose up -d` to update the application.

Noq you could change the code and the application will be reloaded. The DB saves
its state into ".persistence" folder. Do not remove this folder if you want
to save DB state.

After work when you want to remove all:
- Run `docker-compose down` to remove all stuff.
- Run `rm -rf ./.persistence` to remove application volumes.

Notes:
- These command line instructions are for Linux based OS. 

# Development
- Use "idea-colors.icls" to colorize ReasonML code.
- Use ReasonML plugin for IDEs.
- Use "Rainbow Brackets" plugin for IDEs.
