const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("../models");
const { capitalizeFirstLetter } = require('../helpers');

const { filterr } = require('./filterr');
const { channelInput } = require('./channel-input');
const { channel } = require('./channel');

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
      type: hasMany ? graphql.GraphQLNonNull(new graphql.GraphQLList(type)) : type,
      resolve: resolver(model[capitalizeFirstLetter(r.as)])
    };
  });

  return all;
}

const userType = makeObjectType(models.User);
const measurableType = makeObjectType(models.Measurable);
const measurementType = makeObjectType(models.Measurement);
const botType = makeObjectType(models.Bot);
const agentType = makeObjectType(models.Agent);
const seriesType = makeObjectType(models.Series);

const index = {
  Users: userType,
  Agents: agentType,
  Bots: botType,
  Measurables: measurableType,
  measurables: measurableType,
  Series: seriesType,
  Measurements: measurementType,
  Channels: channel,
};

module.exports = {
  ...index,
  index,

  userType,
  agentType,
  botType,
  measurableType,
  seriesType,
  measurementType,

  channel,
  channelInput,
};
