const { channelExistsValidation } = require('./channels');

const { authenticationInputValidation } = require('./authentications');
const { competitiveMeasurementCanBeAddedToOpenMeasurable } = require(
  './measurements',
);
const { measurableNameValidation } = require('./measurables');
const { measurementValueTypeValidation } = require('./measurements');
const { measurementValueValidation } = require('./measurements');

/**
 * Do not try to use DRY principle here.
 * Just read each section as it is.
 */
const validators = {

  Query: {
    authentication: async (resolve, root, args, context, info) => {
      await authenticationInputValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },
  Mutation: {
    measurementCreate: async (resolve, root, args, context, info) => {
      await measurementValueValidation(root, args, context, info);
      await measurementValueTypeValidation(root, args, context, info);
      await competitiveMeasurementCanBeAddedToOpenMeasurable(
        root, args, context, info,
      );
      return resolve(root, args, context, info);
    },

    measurableCreate: async (resolve, root, args, context, info) => {
      await measurableNameValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelBookmarkCreate: async (resolve, root, args, context, info) => {
      await channelExistsValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelBookmarkDelete: async (resolve, root, args, context, info) => {
      await channelExistsValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },
};

module.exports = {
  validators,
};
