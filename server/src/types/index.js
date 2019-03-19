const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("../models");
const { capitalizeFirstLetter } = require('../helpers');

const { competitor } = require('./competitor');
const { valueType } = require('./value-type');

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
 * @param fields
 * @return {{[p: string]: *}}
 */
function filterr(fields) {
  let newFields = { ...fields };
  if (!!newFields.competitorType) {
    newFields.competitorType = { type: competitor }
  }
  if (!!newFields.valueType) {
    newFields.valueType = { type: valueType }
  }
  return newFields;
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

// @todo: move in separate file
const channel = new graphql.GraphQLObjectType({
  name: 'Channel',
  fields: {
    id: { type: graphql.GraphQLString },
    name: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    isArchived: { type: graphql.GraphQLString },
    isPublic: { type: graphql.GraphQLString },
    creator: {
      type: agentType,
      resolve: resolver(models.Agent),
    },
    agents: {
      type: graphql.GraphQLList(agentType),
      resolve: resolver(models.Agent),
    },
  }
});

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

  // @todo: tmp
  filterr,
};
