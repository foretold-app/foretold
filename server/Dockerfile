FROM node:10-alpine

RUN env

COPY package.json /opt/app/
COPY package-lock.json /opt/app/
WORKDIR /opt/app

RUN apk add --no-cache build-base python && \
  npm install --loglevel=warn

COPY src/ /opt/app/lib/src
COPY config/ /opt/app/lib/config
COPY seeders/ /opt/app/lib/seeders
COPY migrations/ /opt/app/lib/migrations

RUN ln -s /opt/app/lib/config /opt/app/config
RUN ln -s /opt/app/lib/migrations /opt/app/migrations
RUN ln -s /opt/app/lib/seeders /opt/app/seeders

EXPOSE ${PORT:-80}
