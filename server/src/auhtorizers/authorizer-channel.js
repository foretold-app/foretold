/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
export const authorizerChannel = next => (root, args, context, info) => {
  console.log('Channel authorizer');
  if (args.channelId) {
    console.log('Channel authorizer', args.channelId);
  } else {
    // throw new Error('Channel ID is required');
  }
  return next(root, args, context, info);
};

module.exports = {
  authorizerChannel,
};
