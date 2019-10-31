export namespace Models {
  export type ObjectID = string;
  export type ChannelMembershipRole = "ADMIN" | "VIEWER";
  export type float = number;
  export type Json = string;

  export type AgentID = ObjectID;
  export type ChannelID = ObjectID;
  export type UserID = ObjectID;
  export type BotID = ObjectID;
  export type MeasurableID = ObjectID;
  export type MeasurementID = ObjectID;
  export type NotificationID = ObjectID;
  export type TemplateID = ObjectID;
  export type GlobalSettingsID = ObjectID;
  export type FeedItemID = ObjectID;
  export type InvitationID = ObjectID;
  export type NotificationStatusID = ObjectID;
  export type SeriesID = ObjectID;
  export type PreferenceID = ObjectID;
  export type TokenID = ObjectID;
  export type ChannelMembershipID = ObjectID;
  export type NotebookID = ObjectID;

  export interface Model {
    id: ObjectID;
    createdAt: string;
    updatedAt: string;

    changed(name: string): boolean;

    previous(name: string): boolean;

    get(name: string): any;
  }

  export interface Channel extends Model {
    id: ChannelID;
    isPublic: boolean;
    creatorId: AgentID;

    getAgents(): Models.Agent[];
  }

  export interface Bot extends Model {
    id: BotID;
    name: string;

    getAgent(): Models.Agent;
  }

  export interface Preference extends Model {
    id: PreferenceID;
    agentId: AgentID;
    stopAllEmails: boolean;
    enableExperimentalFeatures: boolean;
  }

  export interface User extends Model {
    id: UserID;
    name: string;
    auth0Id: string;
    agentId: AgentID;
    email?: string;
    picture?: string;
    isEmailVerified: boolean;
    descriptions?: string;
    auth0AccessToken?: string;

    getAgent(): Models.Agent;
  }

  export interface Measurable extends Model {
    id: MeasurableID;
    name: string;
    labelCustom: string;
    valueType: string;
    expectedResolutionDate: string;
    creatorId: AgentID;
    resolutionEndpoint: string;
    stateUpdatedAt: string;
    labelSubject: string;
    labelOnDate: string;
    labelProperty: string;
    seriesId: SeriesID;
    channelId: ChannelID;
    isArchived: boolean;
    state: string;
    min: number;
    max: number;
    resolutionEndpointResponse: Function;

    getCreator(): Models.Agent;

    getChannel(): Models.Channel;

    judged(): void;
  }

  export interface Measurement extends Model {
    id: MeasurementID;
    value: any;
    competitorType: string;
    measurableId: MeasurableID;
    agentId: AgentID;
    relevantAt: string;
    taggedMeasurementId: MeasurementID;
    description: string;
    valueText: string;

    getMeasurable(): Measurable;

    getAgent(): Agent;
  }

  export interface Agent extends Model {
    id: AgentID;
    isAdmin: boolean;
    type: "BOT" | "USER";
    name: string;
    measurementCount: number;

    getBot(): Models.Bot;

    getUser(): Models.User;
  }

  export interface Series extends Model {
    id: SeriesID;
    subjects: string[];
    properties: string[];
    dates: string[];
    creatorId: ObjectID;
    channelId: ObjectID;
  }

  export interface ChannelMemberships {
    id: ChannelMembershipID;
    agentId: AgentID;
    channelId: ChannelID;
    role: ChannelMembershipRole;
    methodCreatedBy:
      | "ADDED_IN_APP_BY_ADMIN"
      | "AGENT_JOINED_DIRECTLY"
      | "ADDED_BY_EMAIL_BY_ADMIN";
  }

  export interface Token extends Model {
    id: TokenID;
    token: string;
    isActive: boolean;
  }

  export interface NotificationStatus extends Model {
    id: NotificationStatusID;
    agentId: AgentID;
    notificationId: NotificationID;
  }

  export interface Template extends Model {
    id: TemplateID;
  }

  export interface Notification extends Model {
    id: NotificationID;
    envelope: {
      replacements: Object;
      to: string;
      body: string;
      subject: string;
    }
  }

  export interface Invitation extends Model {
    id: InvitationID;
    email: string;
    channelId: ChannelID;
    inviterAgentId: AgentID;
    status: "AWAITING" | "ACCEPTED";
  }

  export interface FeedItem extends Model {
    id: FeedItemID;
    channelId: ChannelID;
    agentId: AgentID;
    body: {
      generic?: { item: string; description: string };
      measurable?: { item: string; description: string; measurableId: string };
    };
  }

  export interface GlobalSetting extends Model {
    id: GlobalSettingsID;
    entityGraph: null | object;
    botAgentId: null | Models.AgentID;
  }

  export interface AgentMeasurable extends Model {
    id: MeasurableID;
    agentId: AgentID;
    measurableId: MeasurableID;
    primaryPointScore?: float;
    predictionCountTotal: number;
  }

  export interface AgentChannel extends Model {
    id: AgentID;
    agentId: AgentID;
    channelId: ChannelID;
    primaryPointScore: float;
    numberOfPredictions: number;
    numberOfQuestionsScored: number;
  }

  export interface Notebook extends Model {
    id: NotebookID;
    ownerId: AgentID;
    channelId: ChannelID;
    body: string;
    name: string;
  }

  export type Creator = Models.User | Models.Bot;
}

export namespace Schema {
  export interface Context {
    // Authentication/authorization section
    user?: Models.User;
    bot?: Models.Bot;
    agent: Models.Agent;
    creator?: Models.User | Models.Bot;

    // settings
    botAgentId?: Models.AgentID;

    // After Middleware Interceptions
    userAsObject?: Models.User;
    preference?: Models.Preference;
    channel?: Models.Channel;
    channelMembership?: Models.ChannelMemberships;
    channelMembershipsAdmins?: Models.ChannelMemberships[];
    channelMembershipsRole?: Models.ChannelMembershipRole;
    resultOrLatestMeasurementForAgentId?: string;
  }

  export interface ChannelsInput {
    name: string;
    description: string;
    isArchived: boolean;
    isPublic: boolean;
  }
}

export namespace Layers {
  export type withinMeasurables = {
    as: string;
    states?: string[];
    channelId?: Models.ChannelID;
  };

  export type withinPublicChannels = {
    as: string;
  };

  export type withinJoinedChannels = {
    as: string;
    agentId: Models.AgentID;
  };

  export type withinPublicAndJoinedChannels = {
    as: string;
    agentId: Models.AgentID;
  };

  export type order = {field: string, direction: string};
  export type orderList = order[];

  namespace DataSourceLayer {
    type compoundId = object;
    type id = string | compoundId;
    type data = object;
    type options = {
      isAdmin?: boolean;
      agentId?: Models.AgentID;
      measuredByAgentId?: Models.AgentID;
      transaction?: object;
      lock?: boolean;
      skipLocked?: boolean;
      currentAgentId?: Models.AgentID;
    };
    type filter = {
      id?: Models.ObjectID;
      creatorId?: Models.AgentID;
      seriesId?: Models.SeriesID;
      channelId?: Models.ChannelID;
      measurableId?: Models.MeasurableID;
      userId?: Models.UserID;
      agentId?: Models.AgentID;
      excludeChannelId?: Models.ChannelID;
      notTaggedByAgent?: Models.AgentID;
      notificationId?: Models.NotificationID;
      ownerId?: Models.AgentID;

      competitorType?: string;
      type?: string;
      attemptCounterMax?: number;
      sentAt?: string | null;
      minPredictionCountTotal?: number | null;
      minNumberOfPredictions?: number | null;
      minNumberOfQuestionsScored?: number | null;
      email?: string;
      status?: string;
      isEmailVerified?: boolean[];
      notAuth0AccessToken?: boolean;
      isNotEmailVerified?: boolean;

      findInDateRange?: object; // @todo: Object? Give definition!
      withinMeasurables?: withinMeasurables | null;
      withinPublicChannels?: withinPublicChannels | null;
      withinJoinedChannels?: withinJoinedChannels | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;

      states?: string[];
      isArchived?: string[];
      types?: string[];
    };
    type pagination = {
      limit?: number;
      offset?: number;
      last?: number;
      first?: number;
      after?: string;
      before?: string;
    };
    type query = {
      sort?: number;
      distinct?: boolean;
      col?: string;
    };
    type params = {
      id?: Models.ObjectID;

      agentId?: Models.AgentID;
      measurableId?: Models.MeasurableID;
      seriesId?: Models.SeriesID;
      channelId?: Models.ChannelID;

      auth0Id?: string;
      name?: string;
      competitorType?: string;
      email?: string;
      isEmailVerified?: boolean;
    };
    type response = { data: any };
    type responseList = { data: any[]; total: number };

    interface DataSource {
      createOne(data: data, options: options): response;

      getOne(params: params, query: query, options: options): response;

      updateOne(params: params, data: data, options: options): response;

      deleteOne(params: params, options: options): response;

      getAll(
        filter: filter,
        pagination: pagination,
        options: options
      ): responseList;

      upsertOne(
        params: params,
        query: query,
        data: data,
        options: options
      ): response;
    }
  }

  namespace AbstractModelsLayer {
    type compoundId = object;
    type id = string | compoundId;

    type data = object;
    type restrictions = {
      agentId?: Models.AgentID;
      userId?: Models.UserID;
      channelId?: Models.ChannelID;
      measuredByAgentId?: Models.AgentID;

      isAdmin?: boolean;
      channelIdAsId?: boolean;
      measurableId?: boolean;

      withinMeasurables?: withinMeasurables | null;
      withinPublicChannels?: withinPublicChannels | null;
      withinJoinedChannels?: withinJoinedChannels | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;
    };
    type options = {
      transaction?: object;
      lock?: boolean;
      skipLocked?: boolean;
    };
    type filter = {
      agentId?: Models.AgentID;
      excludeChannelId?: Models.ChannelID;
      userId?: Models.UserID;
      channelId?: Models.ChannelID;
      measurableId?: Models.MeasurableID;
      notTaggedByAgent?: Models.AgentID;
      seriesId?: Models.SeriesID;
      creatorId?: Models.AgentID;
      ownerId?: Models.AgentID;

      isArchived?: string[];
      types?: string[];
      sentAt?: string[];
      notificationId?: string[];
      competitorType?: string[];
      states?: string[];
      attemptCounterMax?: number;
      minPredictionCountTotal?: number | null;
      minNumberOfPredictions?: number | null;
      minNumberOfQuestionsScored?: number | null;
      email?: string;
      status?: string;
      isEmailVerified?: boolean[];
      notAuth0AccessToken?: boolean;
      isNotEmailVerified?: boolean;

      withinMeasurables?: withinMeasurables | null;
      withinPublicChannels?: withinPublicChannels | null;
      withinJoinedChannels?: withinJoinedChannels | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;

      getSpacedLimit?(): number | undefined;
    };
    type pagination = {
      limit?: number;
      offset?: number;
      last?: number,
      first?: number,
      after?: string,
      before?: string,
      order?: orderList;

      getPagination(total: number): { limit: number, offset: number };
      getPagination2(): { limit: number, offset: number };
      isOrderSet(): boolean;
    };
    type query = {
      sort?: number,
      distinct?: boolean,
      col?: string,
    };
    type params = {
      id?: Models.ObjectID
      agentId?: Models.AgentID
      name?: string,
      auth0Id?: string,
      seriesId?: Models.SeriesID,
      isEmailVerified?: boolean,
    };
    type response = { data: any };
    type responseList = { data: any[], total: number };

    interface AbstractModel {
      deleteOne(
        params: params,
        restrictions: restrictions,
        options: options,
      ): response;

      updateOne(
        params: params,
        data: data,
        restrictions: restrictions,
        options: options,
      ): response;

      createOne(
        data: data,
        restrictions: restrictions,
      ): response;

      getOne(
        params: params,
        query: query,
        restrictions: restrictions,
        options: options,
      ): response;

      getAll(
        filter: filter,
        pagination: pagination,
        restrictions: restrictions,
        options: options,
      ): responseList;

      updateAll(
        params: params,
        data: data,
        options: options,
      ): boolean;

      upsertOne(
        params: params,
        query: query,
        data: data,
        restrictions: restrictions,
        options: options,
      ): response;
    }
  }
}

export type Auth0UserInfoResponse = {
  sub: string,
  email?: string,
  email_verified?: boolean,
  nickname?: string,
  picture?: string,
  given_name?: string,
  family_name?: string,
  name?: string,
  gender?: string,
  locale?: string,
  updated_at?: string,
}

export type EmailConfig = {
  emailHostGenerator: string,
  emailHelp: string,
  unsubscribeLink: string,
  unsubscribeComment: string,
  subscribeEmail: string,
  subscribeLink: string,
  subscribeComment: string,

  gateways: SmtpGateway[],
}

export type SmtpGateway = {
  host: string,
  port: string,
  user: string,
  pass: string,
  from: string,
}
