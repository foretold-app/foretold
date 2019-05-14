FROM node:12.2.0

RUN env

COPY package.json /opt/app/
COPY package-lock.json /opt/app/
COPY yarn.lock /opt/app/
COPY lerna.json /opt/app/
COPY packages /opt/app/packages/
WORKDIR /opt/app

RUN npm install --loglevel=warn

EXPOSE ${PORT:-80}
