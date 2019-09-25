FROM node:12.6.0

RUN env
ENV NODE_ENV=development

COPY package.json /opt/app/
COPY yarn.lock /opt/app/
WORKDIR /opt/app

RUN apt-get update
RUN apt-get install -y nano mc curl git
RUN apt-get install -y ocaml
RUN yarn

COPY lib/ /opt/app/lib
COPY src/ /opt/app/src
COPY stories/ /opt/app/stories
COPY .babelrc /opt/app/
COPY .merlin /opt/app/
COPY .netlifyk /opt/app/
COPY .uglifyrc /opt/app/
COPY _redirects /opt/app/
COPY bsconfig.json /opt/app/
COPY graphql_schema.json /opt/app/
COPY webpack.config.js /opt/app/

RUN yarn build-for-server

EXPOSE ${PORT:-80}
