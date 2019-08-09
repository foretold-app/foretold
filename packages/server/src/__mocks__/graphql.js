function GraphQLSchema(x) {
  if (!(this instanceof GraphQLSchema)) return new GraphQLSchema(x);
}

function GraphQLObjectType(x) {
  if (!(this instanceof GraphQLObjectType)) return new GraphQLObjectType(x);
}

function GraphQLString(x) {
  if (!(this instanceof GraphQLString)) return new GraphQLString(x);
}

function GraphQLList(x) {
  if (!(this instanceof GraphQLList)) return new GraphQLList(x);
}

function GraphQLNonNull(x) {
  if (!(this instanceof GraphQLNonNull)) return new GraphQLNonNull(x);
}

function GraphQLEnumType(x) {
  if (!(this instanceof GraphQLEnumType)) return new GraphQLEnumType(x);
}

function GraphQLInputObjectType(x) {
  if (!(this instanceof GraphQLInputObjectType)) return new GraphQLInputObjectType(x);
}

function GraphQLScalarType(x) {
  if (!(this instanceof GraphQLScalarType)) return new GraphQLScalarType(x);
}

module.exports = {
  GraphQLSchema,
  GraphQLObjectType,
  GraphQLString,
  GraphQLList,
  GraphQLNonNull,
  GraphQLEnumType,
  GraphQLInputObjectType,
  GraphQLScalarType,
  parse: () => {
  },
};
