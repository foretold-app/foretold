FROM node:10-alpine

RUN env

COPY package.json /opt/app/
COPY package-lock.json /opt/app/
WORKDIR /opt/app

RUN apk add --no-cache build-base python && \
  npm install --loglevel=warn

COPY src/ /opt/app/src
COPY config/ /opt/app/config
COPY seeders/ /opt/app/seeders
COPY migrations/ /opt/app/migrations
COPY .sequelizerc /opt/app/

EXPOSE ${PORT:-80}
