const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("../models");
const { capitalizeFirstLetter } = require('../helpers');

const { filterr } = require('./filterr');

const agents = require('./agents');
const channels = require('./channels');
const channelMemberships = require('./channel-memberhips');
const measurables = require('./measurables');
const measurements = require('./measurements');
const series = require('./series');
const users = require('./users');

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

const botType = makeObjectType(models.Bot);

const index = {
  // @todo: Simplify it
  Bots: botType,

  Users: users.user,
  Series: series.series,
  Measurements: measurements.measurement,
  Channels: channels.channel,
  Agents: agents.agent,
  Measurables: measurables.measurable,
};

module.exports = {
  botType,

  agents,
  channels,
  channelMemberships,
  measurables,
  measurements,
  series,
  users,
};
