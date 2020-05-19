const { channelExistsValidation } = require('./channels');
const { notebookExistsValidation } = require('./notebooks');
const { authenticationInputValidation } = require('./authentications');
const { competitiveMeasurementCanBeAddedToOpenMeasurable } = require(
  './measurements',
);
const { measurableNameValidation } = require('./measurables');
const { measurableOnlyOneTitleValidation } = require('./measurables');
const { measurementValueTypeValidation } = require('./measurements');
const { measurementValueValidation } = require('./measurements');
const { agentExistsValidation } = require('./agents');
const { channelMembershipExistsValidation } = require('./channel-memberships');
const { measurableConditionalOnValidation } = require('./measurables');
const { measurableLabelsValidation } = require('./measurables');

/**
 * @todo: To move these validators into "validators.js" file.
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
      await measurableOnlyOneTitleValidation(root, args, context, info);
      await measurableNameValidation(root, args, context, info);
      await measurableLabelsValidation(root, args, context, info);
      await measurableConditionalOnValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    measurableUpdate: async (resolve, root, args, context, info) => {
      await measurableOnlyOneTitleValidation(root, args, context, info);
      await measurableNameValidation(root, args, context, info);
      await measurableLabelsValidation(root, args, context, info);
      await measurableConditionalOnValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelBookmarkToggle: async (resolve, root, args, context, info) => {
      await channelExistsValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelMembershipVerify: async (resolve, root, args, context, info) => {
      await channelExistsValidation(root, args, context, info);
      await agentExistsValidation(root, args, context, info);
      await channelMembershipExistsValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    channelMembershipUnverify: async (resolve, root, args, context, info) => {
      await channelExistsValidation(root, args, context, info);
      await agentExistsValidation(root, args, context, info);
      await channelMembershipExistsValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },

    notebookBookmarkToggle: async (resolve, root, args, context, info) => {
      await notebookExistsValidation(root, args, context, info);
      return resolve(root, args, context, info);
    },
  },
};

module.exports = {
  validators,
};
