FROM node:11.13.0-alpine

RUN env

COPY package*.json /opt/app/
WORKDIR /opt/app

RUN apk add --no-cache build-base python mc nano && \
  npm install --loglevel=warn

COPY api/ /opt/app/api
COPY async/ /opt/app/async
COPY bots/ /opt/app/bots
COPY config/ /opt/app/config
COPY index.js /opt/app

EXPOSE ${PORT:-80}
