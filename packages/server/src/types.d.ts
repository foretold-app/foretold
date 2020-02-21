export namespace Definitions {
  export type ObjectID = string;
  export type ChannelMembershipRole = "ADMIN" | "VIEWER";
  export type float = number;
  export type Json = string;

  export type AgentID = ObjectID;
  export type ChannelBookmarkID = ObjectID;
  export type BotID = ObjectID;
  export type ChannelID = ObjectID;
  export type ChannelMembershipID = ObjectID;
  export type FeedItemID = ObjectID;
  export type GlobalSettingsID = ObjectID;
  export type InvitationID = ObjectID;
  export type MeasurableID = ObjectID;
  export type MeasurementID = ObjectID;
  export type NotebookID = ObjectID;
  export type NotificationID = ObjectID;
  export type NotificationStatusID = ObjectID;
  export type PreferenceID = ObjectID;
  export type SeriesID = ObjectID;
  export type TemplateID = ObjectID;
  export type TokenID = ObjectID;
  export type UserID = ObjectID;

  export interface Definition {
    id: ObjectID;
    createdAt: string;
    updatedAt: string;

    changed(name: string): boolean;

    previous(name: string): boolean;

    get(name: string): any;
  }

  export interface Channel extends Definition {
    id: ChannelID;
    isPublic: boolean;
    creatorId: AgentID;

    getAgents(): Definitions.Agent[];
  }

  export interface Bot extends Definition {
    id: BotID;
    name: string;
    agentId: AgentID;

    getAgent(): Definitions.Agent;
  }

  export interface Preference extends Definition {
    id: PreferenceID;
    agentId: AgentID;
    stopAllEmails: boolean;
    enableExperimentalFeatures: boolean;
  }

  export interface User extends Definition {
    id: UserID;
    name: string;
    auth0Id: string;
    agentId: AgentID;
    email?: string;
    picture?: string;
    isEmailVerified: boolean;
    descriptions?: string;
    auth0AccessToken?: string;

    getAgent(): Definitions.Agent;
  }

  export interface Measurable extends Definition {
    id: MeasurableID;
    name: Promise<string>;
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

    getCreator(): Definitions.Agent;

    getChannel(): Definitions.Channel;

    set(name: string, value: any): Promise<any>;
  }

  export interface Measurement extends Definition {
    id: MeasurementID;
    value: any;
    competitorType: string;
    measurableId: MeasurableID;
    agentId: AgentID;
    relevantAt: number;
    taggedMeasurementId: MeasurementID;
    description: string;
    valueText: string;

    getMeasurable(): Measurable;

    getAgent(): Agent;
  }

  export interface Agent extends Definition {
    id: AgentID;
    isAdmin: boolean;
    type: "BOT" | "USER";
    name: Promise<string>;
    measurementCount: number;

    getBot(): Definitions.Bot;

    getUser(): Definitions.User;
  }

  export interface Series extends Definition {
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
    inviterAgentId: AgentID;
    role: ChannelMembershipRole;
    methodCreatedBy:
      | "ADDED_IN_APP_BY_ADMIN"
      | "AGENT_JOINED_DIRECTLY"
      | "ADDED_BY_EMAIL_BY_ADMIN";
  }

  export interface Token extends Definition {
    id: TokenID;
    token: string;
    isActive: boolean;
  }

  export interface NotificationStatus extends Definition {
    id: NotificationStatusID;
    agentId: AgentID;
    notificationId: NotificationID;
  }

  export interface Template extends Definition {
    id: TemplateID;
  }

  export interface Notification extends Definition {
    id: NotificationID;
    envelope: {
      replacements: Object;
      to: string;
      body: string;
      subject: string;
    };
  }

  export interface Invitation extends Definition {
    id: InvitationID;
    email: string;
    channelId: ChannelID;
    inviterAgentId: AgentID;
    status: "AWAITING" | "ACCEPTED";
  }

  export interface FeedItem extends Definition {
    id: FeedItemID;
    channelId: ChannelID;
    agentId: AgentID;
    body: {
      generic?: { item: string; description: string };
      measurable?: { item: string; description: string; measurableId: string };
    };
  }

  export interface GlobalSetting extends Definition {
    id: GlobalSettingsID;
    entityGraph: null | object;
    botAgentId: null | Definitions.AgentID;
  }

  export interface AgentMeasurable extends Definition {
    id: MeasurableID;
    agentId: AgentID;
    measurableId: MeasurableID;
    primaryPointScore?: float;
    timeAverageScore?: float;
    predictionCountTotal: number;
  }

  export interface AgentChannel extends Definition {
    id: AgentID;
    agentId: AgentID;
    channelId: ChannelID;
    primaryPointScore: float;
    numberOfPredictions: number;
    numberOfQuestionsScored: number;
  }

  export interface Notebook extends Definition {
    id: NotebookID;
    ownerId: AgentID;
    channelId: ChannelID;
    body: string;
    name: string;
  }

  export interface Mutex extends Definition {
    name: string;
    agentId: AgentID;
  }

  export interface Vote extends Definition {
    agentId: AgentID;
    measurementId: MeasurementID;
    voteAmount: number;
  }

  export interface ChannelBookmark extends Definition {
    agentId: AgentID;
    channelId: ChannelID;
  }

  export type Creator = Definitions.User | Definitions.Bot;
}

export namespace Schema {
  export interface Context {
    // Authentication/authorization section
    user?: Definitions.User;
    bot?: Definitions.Bot;
    agent: Definitions.Agent;
    creator?: Definitions.User | Definitions.Bot;

    // settings
    botAgentId?: Definitions.AgentID;

    // After Middleware Interceptions
    userAsObject?: Definitions.User;
    preference?: Definitions.Preference;
    channel?: Definitions.Channel;
    channelMembership?: Definitions.ChannelMemberships;
    channelMembershipsAdmins?: Definitions.ChannelMemberships[];
    channelMembershipsRole?: Definitions.ChannelMembershipRole;
    notebook?: Definitions.Notebook;
    measurement?: Definitions.Measurement;
    measurable?: Definitions.Measurable;
    channelBookmark?: Definitions.ChannelBookmark;
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
    channelId?: Definitions.ChannelID;
  };

  export type withinPublicChannels = {
    as: string;
  };

  export type withinJoinedChannels = {
    as: string;
    agentId: Definitions.AgentID;
  };

  export type withinPublicAndJoinedChannels = {
    as: string;
    agentId: Definitions.AgentID;
  };

  export type attributes = {
    fields?: string[],
    isBookmarked?: {
      agentId: Definitions.AgentID,
    },
    bookmarksCount?: boolean,
  };

  export type order = { field: string; direction: string };
  export type orderList = order[];
  export type lock = boolean | {
    level: string,
  };

  export interface Transaction {
    LOCK: {
      UPDATE: string,
    },

    commit(): void;

    rollback(): void;
  }

  namespace DataSource {
    type compoundId = object;
    type id = string | compoundId;

    interface Data extends Object {
    }

    interface Options {
      agentId?: Definitions.AgentID;
      currentAgentId?: Definitions.AgentID;
      isAdmin?: boolean;

      attributes?: boolean | attributes;
      group?: boolean;
      lock?: lock;
      raw?: boolean;
      skipLocked?: boolean;
      transaction?: object;
    }

    interface Filter {
      id?: Definitions.ObjectID;
      creatorId?: Definitions.AgentID;
      seriesId?: Definitions.SeriesID;
      channelId?: Definitions.ChannelID;
      measurableId?: Definitions.MeasurableID;
      userId?: Definitions.UserID;
      agentId?: Definitions.AgentID;
      excludeChannelId?: Definitions.ChannelID;
      notTaggedByAgent?: Definitions.AgentID;
      notificationId?: Definitions.NotificationID;
      ownerId?: Definitions.AgentID;
      measurableIds?: Definitions.MeasurableID[];
      measurementId?: Definitions.MeasurementID;
      taggedMeasurementId?: Definitions.MeasurementID;
      measuredByAgentId?: Definitions.AgentID;

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
      needsToBePending?: boolean;
      needsResolutionResponse?: boolean;
      role?: string;

      // @todo: Object? Give definition!
      findInDateRange?: object;
      withinMeasurables?: withinMeasurables | null;
      withinPublicChannels?: withinPublicChannels | null;
      withinJoinedChannels?: withinJoinedChannels | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;

      states?: string[];
      isArchived?: string[];
      types?: string[];
    }

    interface Pagination {
      limit?: number;
      offset?: number;
      last?: number;
      first?: number;
      after?: string;
      before?: string;
    }

    interface Query {
      sort?: number;
      distinct?: boolean;
      col?: string;
    }

    interface Params {
      id?: Definitions.ObjectID;

      agentId?: Definitions.AgentID;
      measurableId?: Definitions.MeasurableID;
      measurementId?: Definitions.MeasurementID;
      seriesId?: Definitions.SeriesID;
      channelId?: Definitions.ChannelID;
      notebookId?: Definitions.NotebookID;

      auth0Id?: string;
      name?: string;
      competitorType?: string;
      email?: string;
      isEmailVerified?: boolean;
    }

    interface Response {
      data: any
    }

    interface ResponseList {
      data: any[];
      total: number
    }

    // @todo: To fix response types.
    interface Generic {
      createOne(data: Data, options: Options): Promise<Response>;

      getOne(params: Params, query: Query, options: Options): Promise<Response>;

      getCount(params: Params, query: Query, options: Options): Promise<number>;

      updateOne(params: Params, data: Data, options: Options): Promise<Response>;

      deleteOne(params: Params, options: Options): Promise<Response>;

      getAll(
        filter: Filter,
        pagination: Pagination,
        options: Options
      ): Promise<ResponseList>;

      getConnection(
        filter: Filter,
        pagination: Pagination,
        options: Options
      ): Promise<ResponseList>;

      upsertOne(
        params: Params,
        query: Query,
        data: Data,
        options: Options
      ): Promise<Response>;

      getTransaction(): Promise<Transaction>;

      commit(Transaction): Promise<Transaction>;

      rollback(Transaction): Promise<Transaction>;

      lock(): Promise<boolean>;

      updateMaterializedView(): Promise<boolean>;
    }
  }

  namespace Models {
    type compoundId = object;
    type id = string | compoundId;

    interface Data extends Object {
    }

    interface Restrictions {
      agentId?: Definitions.AgentID;
      channelId?: Definitions.ChannelID;
      channelIdAsId?: boolean;
      isAdmin?: boolean;
      measurableId?: boolean;
      userId?: Definitions.UserID;

      withinJoinedChannels?: withinJoinedChannels | null;
      withinMeasurables?: withinMeasurables | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;
      withinPublicChannels?: withinPublicChannels | null;
    }

    interface Options {
      attributes?: boolean | attributes;
      group?: boolean;
      lock?: lock;
      raw?: boolean;
      skipLocked?: boolean;
      transaction?: object;
    }

    interface Filter {
      agentId?: Definitions.AgentID;
      excludeChannelId?: Definitions.ChannelID;
      userId?: Definitions.UserID;
      channelId?: Definitions.ChannelID;
      measurableId?: Definitions.MeasurableID;
      notTaggedByAgent?: Definitions.AgentID;
      seriesId?: Definitions.SeriesID;
      creatorId?: Definitions.AgentID;
      ownerId?: Definitions.AgentID;
      measurableIds?: Definitions.MeasurableID[];
      measurementId?: Definitions.MeasurementID;
      taggedMeasurementId?: Definitions.MeasurementID;
      measuredByAgentId?: Definitions.AgentID;

      isArchived?: string[];
      types?: string[];
      sentAt?: string[];
      notificationId?: Definitions.NotificationID[];
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
      needsToBePending?: boolean;
      needsResolutionResponse?: boolean;
      role?: string;

      withinMeasurables?: withinMeasurables | null;
      withinPublicChannels?: withinPublicChannels | null;
      withinJoinedChannels?: withinJoinedChannels | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;

      getSpacedLimit?(): number | undefined;
    }

    interface Pagination {
      limit?: number;
      offset?: number;
      last?: number;
      first?: number;
      after?: string;
      before?: string;
      order?: orderList;

      getPagination(): { limit: number; offset: number };

      isOrderSet(): boolean;

      getOrder(): orderList;
    }

    interface Query {
      col?: string;
      distinct?: boolean;
      sort?: number;
    }

    interface Params {
      id?: Definitions.ObjectID;

      agentId?: Definitions.AgentID;
      measurableId?: Definitions.MeasurableID;
      measurementId?: Definitions.MeasurementID;
      seriesId?: Definitions.SeriesID;
      channelId?: Definitions.ChannelID;
      notebookId?: Definitions.NotebookID;

      auth0Id?: string;
      name?: string;
      competitorType?: string;
      email?: string;
      isEmailVerified?: boolean;
    }

    interface Response {
      data: any
    }

    interface ResponseList {
      data: any[];
      total: number
    }

    // @todo: To fix response types.
    interface Generic {
      deleteOne(
        params: Params,
        restrictions: Restrictions,
        options: Options
      ): Promise<Response>;

      updateOne(
        params: Params,
        data: Data,
        restrictions: Restrictions,
        options: Options
      ): Promise<Response>;

      createOne(data: Data, restrictions: Restrictions): Promise<Response>;

      getOne(
        params: Params,
        query: Query,
        restrictions: Restrictions,
        options: Options
      ): Promise<Response>;

      upsertOne(
        params: Params,
        query: Query,
        data: Data,
        restrictions: Restrictions,
        options: Options
      ): Promise<Response>;

      getCount(
        params: Params,
        query: Query,
        restrictions: Restrictions,
        options: Options
      ): Promise<number>;

      getAll(
        filter: Filter,
        pagination: Pagination,
        restrictions: Restrictions,
        options: Options
      ): Promise<ResponseList>;

      getAllWithConnections(
        filter: Filter,
        pagination: Pagination,
        restrictions: Restrictions,
        options: Options
      ): Promise<ResponseList>;

      updateAll(params: Params, data: Data, options: Options): Promise<boolean>;

      upsertOne(
        params: Params,
        query: Query,
        data: Data,
        restrictions: Restrictions,
        options: Options
      ): Promise<Response>;

      getTransaction(): Promise<Transaction>;

      commit(Transaction): Promise<Transaction>;

      rollback(Transaction): Promise<Transaction>;

      lock(): Promise<boolean>;

      updateMaterializedView(): Promise<boolean>;
    }
  }
}

export type Auth0UserInfoResponse = {
  sub: string;
  email?: string;
  email_verified?: boolean;
  nickname?: string;
  picture?: string;
  given_name?: string;
  family_name?: string;
  name?: string;
  gender?: string;
  locale?: string;
  updated_at?: string;
};

export type EmailConfig = {
  emailHostGenerator: string;
  emailHelp: string;
  unsubscribeLink: string;
  unsubscribeComment: string;
  subscribeEmail: string;
  subscribeLink: string;
  subscribeComment: string;

  gateways: SmtpGateway[];
};

export type SmtpGateway = {
  host: string;
  port: string;
  user: string;
  pass: string;
  from: string;
};
