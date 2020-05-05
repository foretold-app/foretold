FROM node:12.6.0

RUN env

COPY package-travis.json /opt/app/package.json
COPY yarn.lock /opt/app/
COPY lerna.json /opt/app/
COPY ws-fixer.sh /opt/app/
COPY packages /opt/app/packages/
WORKDIR /opt/app

# yarn install === lerna bootstrap (!)
RUN yarn install --loglevel=warn --unsafe-perm
RUN yarn lerna bootstrap --ignore-scripts
# RUN yarn packages/clean
RUN yarn packages/build

EXPOSE ${PORT:-80}
