let stringOfcompetitorType = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

type user = {
  id: string,
  name: string,
};

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

type agent = {
  id: string,
  measurementCount: option(int),
  bot: option(bot),
  user: option(user),
};

type agents = array(agent);

type valueType = [ | `DATE | `FLOAT | `PERCENTAGE];

type creator = {
  id: string,
  name: option(string),
};

type measurable = {
  id: string,
  name: string,
  valueType,
  isLocked: bool,
  description: option(string),
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  lockedAt: option(MomentRe.Moment.t),
  creator: option(creator),
};

type measurables = array(measurable);