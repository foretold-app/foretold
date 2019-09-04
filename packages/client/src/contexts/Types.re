type valueType = [ | `DATE | `FLOAT | `PERCENTAGE];

type agentTypeEntity = [ | `USER | `BOT];

type channelMembershipRole = [ | `ADMIN | `VIEWER];

type myMembershipRole = [ | `ADMIN | `VIEWER | `NONE];

type competitorType = [
  | `AGGREGATION
  | `COMPETITIVE
  | `OBJECTIVE
  | `UNRESOLVED
  | `COMMENT
];

type measurableState = [
  | `OPEN
  | `JUDGEMENT_PENDING
  | `JUDGED
  | `CLOSED_AS_UNRESOLVED
];

type isArchived = [ | `TRUE | `FALSE];

type permission = [
  | `BOT_CREATE
  | `BOT_UPDATE
  | `CHANNEL_CREATE
  | `CHANNEL_MEMBERSHIP_CREATE
  | `CHANNEL_MEMBERSHIP_DELETE
  | `CHANNEL_MEMBERSHIP_ROLE_UPDATE
  | `CHANNEL_UPDATE
  | `JOIN_CHANNEL
  | `LEAVE_CHANNEL
  | `MEASURABLE_ARCHIVE
  | `MEASURABLE_CREATE
  | `MEASURABLE_UNARCHIVE
  | `MEASURABLE_UPDATE
  | `MEASUREMENT_CREATE
  | `SERIES_CREATE
  | `USER_UPDATE
];

type permissions = {allow: list(permission)};

type preference = {
  id: string,
  stopAllEmails: option(bool),
  enableExperimentalFeatures: bool,
};

type user = {
  id: string,
  auth0Id: option(string),
  agent: option(agent),
  name: string,
  agentId: string,
  email: option(string),
  picture: option(string),
  description: option(string),
  score: option(float),
  bots: option(array(bot)),
}
and bot = {
  competitorType,
  description: option(string),
  id: string,
  name: option(string),
  token: option(string),
  agent: option(agent),
  permissions: option(permissions),
}
and agentType =
  | Bot(bot)
  | User(user)
and agent = {
  id: string,
  name: option(string),
  measurementCount: option(int),
  agentType: option(agentType),
  channels: Js.Array.t(channel),
  channelMemberships: option(Js.Array.t(channelMembership)),
  preference: option(preference),
}
and channel = {
  id: string,
  name: string,
  description: option(string),
  isArchived: bool,
  isPublic: bool,
  isCurated: bool,
  membershipCount: option(int),
  creator: option(agent),
  myRole: option(myMembershipRole),
  channelMemberships: option(Js.Array.t(channelMembership)),
  openedMeasurablesCount: option(int),
  permissions: option(permissions),
}
and channelMembership = {
  channel: option(channel),
  role: channelMembershipRole,
  agent: option(agent),
  permissions: option(permissions),
}
and measurable = {
  id: string,
  name: string,
  valueType,
  channel: option(channel),
  channelId: string,
  isArchived: option(bool),
  resolutionEndpoint: option(string),
  resolutionEndpointResponse: option(float),
  measurementCount: option(int),
  measurerCount: option(int),
  state: option(measurableState),
  labelSubject: option(string),
  labelProperty: option(string),
  labelOnDate: option(MomentRe.Moment.t),
  labelCustom: option(string),
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(agent),
  measurements: option(list(measurement)),
  series: option(series),
  iAmOwner: option(bool),
  min: option(float),
  max: option(float),
  permissions: option(permissions),
  outcome: option(measurement),
  previousAggregate: option(measurement),
}
and measurementScoreSet = {
  primaryPointScore: option(float),
  nonMarketLogScore: option(float),
  prediction: measurement,
  outcome: option(measurement),
  previousAggregate: option(measurement),
}
and measurement = {
  id: string,
  description: option(string),
  value: Belt.Result.t(MeasurementValue.t, string),
  competitorType,
  taggedMeasurementId: option(string),
  createdAt: option(MomentRe.Moment.t),
  relevantAt: option(MomentRe.Moment.t),
  measurableId: option(string),
  valueText: option(string),
  measurable: option(measurable),
  agent: option(agent),
  measurementScoreSet: option(measurementScoreSet),
}
and agentMeasurable = {
  id: string,
  measurable,
  agent,
  primaryPointScore: float,
  predictionCountTotal: int,
  createdAt: MomentRe.Moment.t,
  competitiveMeasurement: option(measurement),
  aggregationMeasurement: option(measurement),
  objectiveMeasurement: option(measurement),
}
and agentChannel = {
  id: string,
  agentId: string,
  channelId: string,
  primaryPointScore: float,
  numberOfPredictions: int,
  numberOfQuestionsScored: int,
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  agent,
  channel,
}
and leaderboardItem = {
  id: string,
  measurable: option(measurable),
  agent: option(agent),
  pointScore: option(float),
  createdAt: option(MomentRe.Moment.t),
  predictionCountTotal: option(int),
  numberOfQuestionsScored: option(int),
  competitiveMeasurement: option(measurement),
  aggregationMeasurement: option(measurement),
  objectiveMeasurement: option(measurement),
}
and series = {
  id: string,
  description: option(string),
  name: option(string),
  creator: option(agent),
  channel: option(channel),
  measurableCount: option(int),
}
and feedItem = {
  id: string,
  channelId: string,
  body: FeedItemBody.t,
  channel,
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
}
and pageInfo = {
  hasNextPage: bool,
  hasPreviousPage: bool,
  endCursor: option(string),
  startCursor: option(string),
}
and connection('a) = {
  pageInfo,
  total: option(int),
  edges: array('a),
}
and globalSetting = {
  id: string,
  entityGraph: option(Js.Json.t),
};