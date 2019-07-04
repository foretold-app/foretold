const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../models');

// Input
const measurementValueInputFloatCdf = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInputFloatCdf',
  fields: () => ({
    xs: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLFloat)) },
    ys: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLFloat)) },
  }),
});

const measurementValueInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementValueInput',
  fields: () => ({
    floatCdf: { type: measurementValueInputFloatCdf },
    floatPoint: { type: graphql.GraphQLFloat },
    percentage: { type: graphql.GraphQLFloat },
    binary: { type: graphql.GraphQLBoolean },
  }),
});

const measurementCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementCreateInput',
  fields: () => ({
    value: { type: measurementValueInput },
    valueText: { type: graphql.GraphQLString },
    competitorType: { type: require('./competitor-type').competitorType },
    measurableId: { type: graphql.GraphQLString },
    agentId: { type: graphql.GraphQLString },
    description: { type: graphql.GraphQLString },
    taggedMeasurementId: { type: graphql.GraphQLString },
    relevantAt: { type: DateType.default },
  }),
});

const measurementsInDateRangeInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurementsInDateRangeInput',
  fields: () => ({
    startDate: { type: DateType.default },
    endDate: { type: DateType.default },
    spacedLimit: { type: graphql.GraphQLInt },
  }),
});

// Output
const measurementValueFloatCdf = new graphql.GraphQLObjectType({
  name: 'MeasurementValueFloatCdf',
  fields: () => ({
    xs: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLNonNull(graphql.GraphQLFloat))) },
    ys: { type: graphql.GraphQLNonNull(graphql.GraphQLList(graphql.GraphQLNonNull(graphql.GraphQLFloat))) },
  }),
});

const measurementValue = new graphql.GraphQLObjectType({
  name: 'MeasurementValue',
  fields: () => ({
    floatCdf: { type: measurementValueFloatCdf },
    floatPoint: { type: graphql.GraphQLFloat },
    percentage: { type: graphql.GraphQLFloat },
    binary: { type: graphql.GraphQLBoolean },
  }),
});

const measurement = new graphql.GraphQLObjectType({
  name: 'Measurement',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    value: { type: graphql.GraphQLNonNull(measurementValue) },
    competitorType: { type: require('./competitor-type').competitorType },
    description: { type: graphql.GraphQLString },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLString },
    relevantAt: { type: DateType.default },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    taggedMeasurementId: { type: graphql.GraphQLString },
    iAmOwner: require('./common').iAmOwner,
    valueText: { type: graphql.GraphQLString },

    Measurable: {
      type: require('./measurables').measurable,
      resolve: resolver(models.Measurement.Measurable),
    },

    Agent: {
      type: require('./agents').agent,
      resolve: resolver(models.Measurement.Agent),
    },

    TaggedMeasurement: {
      type: measurement,
      resolve: resolver(models.Measurement.TaggedMeasurement),
    },

    TaggedBy: {
      type: graphql.GraphQLNonNull(graphql.GraphQLList(measurement)),
      resolve: resolver(models.Measurement.TaggedBy),
    },
  })
});

const measurementsEdge = new graphql.GraphQLObjectType({
  name: 'MeasurementsEdge',
  fields: () => ({
    node: { type: require('./measurements').measurement },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const measurementsConnection = new graphql.GraphQLObjectType({
  name: 'MeasurementsConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./measurements').measurementsEdge) },
  }),
});

module.exports = {
  measurement,
  measurementCreateInput,
  measurementsEdge,
  measurementsConnection,
  measurementsInDateRangeInput,
};
