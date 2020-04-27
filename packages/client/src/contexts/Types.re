type cursor = Js.Json.t;
type agentId = string;
type channelId = string;
type notebookId = string;
type voteId = string;
type measurementId = string;
type measurableId = string;
type string0to255 = string;
type string0to16K = string;
type pageParams = {id: string};

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
type fieldChannels = [ | `membersCount | `isCurated | `name];
type fieldAgentMeasurables = [ | `primaryPointScore];
type fieldNotebooks = [ | `isBookmarked];
type fieldMeasurables = [ | `refreshedAt | `stateOrder];
type direction = [ | `ASC | `DESC];

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
  | `GLOBAL_SETTING_UPDATE
  | `INVITATION_CREATE
  | `PREFERENCE_UPDATE
  | `SUBSCRIBE
  | `UNSUBSCRIBE
  | `MEASUREMENT_VOTE
  | `CHANNEL_BOOKMARK_TOGGLE
  | `NOTEBOOK_BOOKMARK_TOGGLE
  | `NOTEBOOK_DELETE
  | `NOTEBOOK_UPDATE
  | `CHANNEL_MEMBERSHIP_UNVERIFY
  | `CHANNEL_MEMBERSHIP_VERIFY
];

type marketType = [ | `MARKET | `NON_MARKET];

type finalComparisonMeasurement = [
  | `LAST_AGGREGATE_MEASUREMENT
  | `LAST_OBJECTIVE_MEASUREMENT
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
  bots: option(array(bot)),
  isEmailVerified: option(bool),
  createdAt: option(MomentRe.Moment.t),
}

and bot = {
  competitorType,
  id: string,
  name: option(string),
  description: option(string),
  picture: option(string),
  token: option(string),
  agent: option(agent),
  permissions: option(permissions),
  owner: option(agent),
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
  isMe: bool,
}

and channel = {
  id: string,
  name: string,
  description: option(string),
  isArchived: bool,
  isPublic: bool,
  isCurated: bool,
  requireVerification: bool,
  membershipCount: option(int),
  creator: option(agent),
  myRole: option(myMembershipRole),
  notebooksCount: option(int),
  channelMemberships: option(Js.Array.t(channelMembership)),
  openedMeasurablesCount: option(int),
  permissions: option(permissions),
  knowledgeGraph: option(string),
  isBookmarked: option(bool),
  bookmarksCount: option(int),
}

and channelMembership = {
  channel: option(channel),
  role: channelMembershipRole,
  isVerified: option(bool),
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
  recentMeasurement: option(measurement),
}

and measurementScoreSet = {
  primaryPointScore: option(float),
  nonMarketLogScore: option(float),
}

and measurement = {
  id: string,
  description: option(string),
  value: Belt.Result.t(MeasurementValue.t, string),
  competitorType,
  taggedMeasurementId: option(string),
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
  relevantAt: option(MomentRe.Moment.t),
  measurableId: option(string),
  valueText: option(string),
  measurable: option(measurable),
  agent: option(agent),
  measurementScoreSet: option(measurementScoreSet),
  totalVoteAmount: option(int),
  permissions: option(permissions),
  vote: option(vote),
}

and vote = {
  id: string,
  voteAmount: int,
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
  agent: option(agent),
}

and timeAverageScore = {
  score: float,
  timeActivityRatio: float,
  marketType,
  finalComparisonMeasurement,
}

and agentMeasurable = {
  id: string,
  measurable,
  agent,
  primaryPointScore: option(float),
  timeAverageScore: option(timeAverageScore),
  predictionCountTotal: int,
  createdAt: MomentRe.Moment.t,
  competitiveMeasurement: option(measurement),
  aggregationMeasurement: option(measurement),
  objectiveMeasurement: option(measurement),
  totalVotesReceived: option(int),
}

and agentChannel = {
  id: string,
  agentId: string,
  channelId: string,
  primaryPointScore: option(float),
  numberOfPredictions: int,
  numberOfQuestionsScored: int,
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  totalVotesReceived: option(int),
  agent,
  channel,
}

and leaderboardItem = {
  id: string,
  measurable: option(measurable),
  agent: option(agent),
  channel: option(channel),
  pointScore: option(float),
  timeAverageScore: option(timeAverageScore),
  createdAt: option(MomentRe.Moment.t),
  predictionCountTotal: option(int),
  numberOfQuestionsScored: option(int),
  competitiveMeasurement: option(measurement),
  aggregationMeasurement: option(measurement),
  objectiveMeasurement: option(measurement),
  totalVotesReceived: option(int),
}

and series = {
  id: string,
  description: option(string),
  name: option(string),
  creator: option(agent),
  channel: option(channel),
  measurableCount: option(int),
  channelId: string,
  subjects: option(array(option(string))),
  properties: option(array(option(string))),
  dates: option(array(option(Js.Json.t))),
}

and feedItem = {
  id: string,
  channelId: string,
  body: FeedItemBody.body,
  channel,
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
  agent,
}

and pageInfo = {
  hasNextPage: bool,
  hasPreviousPage: bool,
  endCursor: option(cursor),
  startCursor: option(cursor),
}

and connection('a) = {
  pageInfo,
  total: option(int),
  edges: array('a),
}

and connectionInputType('a) =
  (
    ~first: Js.Json.t=?,
    ~last: Js.Json.t=?,
    ~after: cursor=?,
    ~before: cursor=?,
    unit
  ) =>
  'a

and globalSetting = {
  id: string,
  entityGraph: option(BsKen.Graph_T.T.t),
}

and notebook = {
  id: notebookId,
  name: string,
  body: string,
  ownerId: agentId,
  channelId,
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
  owner: agent,
  channel,
  isBookmarked: option(bool),
  bookmarksCount: option(int),
  permissions: option(permissions),
};