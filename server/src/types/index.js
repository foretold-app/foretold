const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("../models");
const { capitalizeFirstLetter } = require('../helpers');

const { filterr } = require('./filterr');

const agents = require('./agents');
const channels = require('./channels');
const channelMemberships = require('./channel-memberhips');

/**
 * @deprecated
 * @param model
 * @return {GraphQLObjectType}
 */
function makeObjectType(model) {
  return new graphql.GraphQLObjectType({
    name: model.name,
    description: model.name,
    fields: () =>
      _.assign(
        filterr(attributeFields(model)),
        generateReferences(model),
      )
  });
}

/**
 * @deprecated
 * @param model
 */
function generateReferences(model) {
  let all = {};
  const associations = _.toArray(model.associations);

  associations.map(r => {
    const hasMany = r.associationType === "HasMany";
    const toMany = r.associationType === "BelongsToMany";
    const otherTableName = r.target.tableName;
    const type = index[otherTableName];

    all[r.as] = {
      type: (hasMany || toMany) ? graphql.GraphQLNonNull(new graphql.GraphQLList(type)) : type,
      resolve: resolver(model[capitalizeFirstLetter(r.as)])
    };
  });

  return all;
}

const userType = makeObjectType(models.User);
const measurableType = makeObjectType(models.Measurable);
const measurementType = makeObjectType(models.Measurement);
const botType = makeObjectType(models.Bot);
const seriesType = makeObjectType(models.Series);

const index = {
  Users: userType,
  Agents: agents.agent,
  Bots: botType,
  Measurables: measurableType,
  measurables: measurableType,
  Series: seriesType,
  Measurements: measurementType,
  Channels: channels.channel,
};

module.exports = {
  ...index,
  index,

  userType,
  botType,
  measurableType,
  seriesType,
  measurementType,

  agents,
  channels,
  channelMemberships,
};
