module Queries = {
  module Agent = Foretold__GraphQL__GetAgent;
  module Agents = Foretold__GraphQL__GetAgents;
  module Measurable = Foretold__GraphQL__GetMeasurable;
  module Measurables = Foretold__GraphQL__GetMeasurables;
  module MeasurableWithMeasurements = Foretold__GraphQL__GetMeasurableWithMeasurements;
  module Series = Foretold__GraphQL__GetSeries;
  module User = Foretold__GraphQL__GetUser;
  module SeriesCollection = Foretold__GraphQL__GetSeriesCollection;
  module Channels = Foretold__GraphQL__ChannelsGet;
  module Channel = Foretold__GraphQL__ChannelGet;
  module ChannelMemberships = Foretold__GraphQL__ChannelGetMemberships;
};

module Mutations = {
  module MeasurementCreate = Foretold__GraphQL__CreateMeasurementMutation;
  module MeasurableArchive = Foretold__GraphQL__MeasurableArchive;
  module MeasurableUnarchive = Foretold__GraphQL__MeasurableUnarchive;
  module ChannelCreate = Foretold__GraphQL__ChannelCreate;
  module ChannelJoin = Foretold__GraphQL__ChannelJoin;
  module ChannelLeave = Foretold__GraphQL__ChannelLeave;
  module ChannelUpdate = Foretold__GraphQL__ChannelUpdate;
  module ChannelMembershipRoleUpdate = Foretold__GraphQL__ChannelMembershipRoleUpdate;
};