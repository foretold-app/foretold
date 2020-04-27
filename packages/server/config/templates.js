const { TEMPLATE_NAME } = require('../src/enums');
const { TEMPLATE_TYPE } = require('../src/enums');

/**
 *
 * Do not forget that this file is used as a stub of DB table "Templates".
 * This is why if you had decided to return to DB you would have needed
 * to migrate this data on DB table.
 *
 * @todo: To rename an "envelopeTemplate" into something yet.
 * @todo: Copy to the DB (later).
 *
 */
module.exports = [
  {
    name: TEMPLATE_NAME.EMAIL_OUTER_TEMPLATE,
    type: TEMPLATE_TYPE.EMAIL_ENVELOPE,
    envelopeTemplate: {
      outerTemplate: '{{{ innerTemplate }}}'
        + '<hr /> '
        + '<a href="<% unsubscribe.link %>">Unsubscribe</a> |'
        + '<a href="<% subscribe.link %>">Subscribe</a>.',
    },
  },
  {
    name: TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED,
    type: TEMPLATE_TYPE.EMAIL,
    envelopeTemplate: {
      subject: 'Question "{{{ measurable.name }}}" has reached its '
        + 'expected resolution date.',
      body: 'Your question, '
        + '<a href="{{{ measurable.link }}}">{{{ measurable.name }}}</a>, '
        + 'has reached its expected resolution date. We recommend either '
        + 'resolving the question or moving the expected resolution date to '
        + 'another time. You can do that '
        + '<a href="{{{ measurable.link }}}">here</a>.',
    },
  },
  {
    name: TEMPLATE_NAME.MEASURABLE_STATE_IS_RESOLVED,
    type: TEMPLATE_TYPE.EMAIL,
    envelopeTemplate: {
      subject: 'Question "{{{ measurable.name }}}" was resolved',
      body: 'The question [{{{ measurable.name }}}] was resolved. Click <a'
        + ' href="{{{ measurable.link }}}">here</a> to see the result.',
    },
  },
  {
    name: TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY,
    type: TEMPLATE_TYPE.EMAIL,
    envelopeTemplate: {
      subject: 'You have been added to a community on Foretold',
      body: '<a href="{{{ inviterAgent.link }}}">{{{ inviterAgent.name'
        + ' }}}</a> has added you to the Foretold community, [<a href="{{{'
        + ' channel.link }}}">{{ channel.name }}</a>]',
    },
  },
  {
    name: TEMPLATE_NAME.MEMBER_JOINED_COMMUNITY_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '',
      description: '<split><agent /></split> joined the community',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_MEASUREMENT_PREDICTION_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '{{{ measurable.name }}}',
      description: '<split><agent /></split> made a prediction',
      measurableId: '{{{ measurable.id }}}',
      measurementId: '{{{ measurement.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_MEASUREMENT_COMMENT_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '{{{ measurable.name }}}',
      description: '<split><agent /></split> made a comment',
      measurableId: '{{{ measurable.id }}}',
      measurementId: '{{{ measurement.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_MEASUREMENT_RESOLUTION_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '{{{ measurable.name }}}',
      description: '<split><agent /></split> resolved the question with an'
        + ' answer',
      measurableId: '{{{ measurable.id }}}',
      measurementId: '{{{ measurement.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_MEASUREMENT_RESOLUTION_NOT_AVAILABLE_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '{{{ measurable.name }}}',
      description: '<split><agent /></split> has marked this question as '
        + 'unresolved',
      measurableId: '{{{ measurable.id }}}',
      measurementId: '{{{ measurement.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.MEASURABLE_REACHED_RESOLUTION_DATE_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '{{{ measurable.name }}}',
      description: '{{{ measurable.name }}} has reached it\'s expected'
        + ' resolution date',
      measurableId: '{{{ measurable.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_MEASURABLE_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '{{{ measurable.name }}}',
      description: '<split><agent /></split> made a question',
      measurableId: '{{{ measurable.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_CHANNEL_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '',
      description: '<split><agent /></split> has created a community',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_INVITATION,
    type: TEMPLATE_TYPE.EMAIL,
    envelopeTemplate: {
      subject: 'You have been invited to the Foretold community, '
        + '"{{{ channel.name }}}"',
      body: 'You have been invited to the Foretold community '
        + '<a href="{{{ channel.link }}}">{{{ channel.name }}}</a>.'
        + 'Sign up to <a href="https://foretold.io/login">Foretold</a> '
        + 'and verify your email address to accept the invitation. ',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_NOTEBOOK_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '',
      description: '<split><agent /></split> made a notebook',
      notebookId: '{{{ notebook.id }}}',
    },
  },
  {
    name: TEMPLATE_NAME.NEW_SERIES_FEED_ITEM,
    type: TEMPLATE_TYPE.FEED_ITEM,
    envelopeTemplate: {
      item: '',
      description: '<split><agent /></split> made a series',
      seriesId: '{{{ series.id }}}',
    },
  },
];
