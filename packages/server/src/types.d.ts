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

  /**
   * @todo: Why "DataSource" and "Models" both have the same
   * @todo: interfaces "Filter", "Params", "Query", "Pagination"?
   * @todo: It is confusing.
   * @todo: Should we separate these terms in the code similar?
   * @todo: Either we should remove "duplicated" interfaces?
   */

  namespace DataSource {
    type compoundId = object;
    type id = string | compoundId;

    interface Data extends Object {
    }

    interface DataOptions {
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

    interface DataFilter {
      agentId?: Definitions.AgentID;
      channelId?: Definitions.ChannelID;
      creatorId?: Definitions.AgentID;
      excludeChannelId?: Definitions.ChannelID;
      id?: Definitions.ObjectID;
      measurableId?: Definitions.MeasurableID;
      measurableIds?: Definitions.MeasurableID[];
      measuredByAgentId?: Definitions.AgentID;
      measurementId?: Definitions.MeasurementID;
      notTaggedByAgent?: Definitions.AgentID;
      notificationId?: Definitions.NotificationID;
      ownerId?: Definitions.AgentID;
      seriesId?: Definitions.SeriesID;
      taggedMeasurementId?: Definitions.MeasurementID;
      userId?: Definitions.UserID;

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

      getSpacedLimit?(): number | undefined;
    }

    interface DataPagination {
      limit?: number;
      offset?: number;
      last?: number;
      first?: number;
      after?: string;
      before?: string;
    }

    interface DataQuery {
      sort?: number;
      distinct?: boolean;
      col?: string;
    }

    interface DataParams {
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

    interface DataResponse {
      data: any
    }

    interface DataResponseList {
      data: any[];
      total: number
    }

    // @todo: To fix response types.
    interface DataGeneric {
      createOne(data: Data, options: DataOptions): Promise<DataResponse>;

      getOne(params: DataParams, query: DataQuery, options: DataOptions): Promise<DataResponse>;

      getCount(params: DataParams, query: DataQuery, options: DataOptions): Promise<number>;

      updateOne(params: DataParams, data: Data, options: DataOptions): Promise<DataResponse>;

      deleteOne(params: DataParams, options: DataOptions): Promise<DataResponse>;

      getAll(
        filter: DataFilter,
        pagination: DataPagination,
        options: DataOptions
      ): Promise<DataResponseList>;

      getConnection(
        filter: DataFilter,
        pagination: DataPagination,
        options: DataOptions
      ): Promise<DataResponseList>;

      upsertOne(
        params: DataParams,
        query: DataQuery,
        data: Data,
        options: DataOptions
      ): Promise<DataResponse>;

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

    interface ModelRestrictions {
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

    interface ModelOptions {
      attributes?: boolean | attributes;
      group?: boolean;
      lock?: lock;
      raw?: boolean;
      skipLocked?: boolean;
      transaction?: object;
    }

    interface ModelFilter {
      agentId?: Definitions.AgentID;
      channelId?: Definitions.ChannelID;
      creatorId?: Definitions.AgentID;
      excludeChannelId?: Definitions.ChannelID;
      id?: Definitions.ObjectID;
      measurableId?: Definitions.MeasurableID;
      measurableIds?: Definitions.MeasurableID[];
      measuredByAgentId?: Definitions.AgentID;
      measurementId?: Definitions.MeasurementID;
      notTaggedByAgent?: Definitions.AgentID;
      ownerId?: Definitions.AgentID;
      seriesId?: Definitions.SeriesID;
      taggedMeasurementId?: Definitions.MeasurementID;
      userId?: Definitions.UserID;

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

      findInDateRange?: object;
      withinMeasurables?: withinMeasurables | null;
      withinPublicChannels?: withinPublicChannels | null;
      withinJoinedChannels?: withinJoinedChannels | null;
      withinPublicAndJoinedChannels?: withinPublicAndJoinedChannels | null;

      getSpacedLimit?(): number | undefined;
    }

    interface ModelPagination {
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

    interface ModelQuery {
      col?: string;
      distinct?: boolean;
      sort?: number;
    }

    interface ModelParams {
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

    interface ModelResponse {
      data: any
    }

    interface ModelResponseList {
      data: any[];
      total: number
    }

    // @todo: To fix response types.
    interface ModelGeneric {
      deleteOne(
        params: ModelParams,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponse>;

      updateOne(
        params: ModelParams,
        data: Data,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponse>;

      createOne(data: Data, restrictions: ModelRestrictions): Promise<ModelResponse>;

      getOne(
        params: ModelParams,
        query: ModelQuery,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponse>;

      upsertOne(
        params: ModelParams,
        query: ModelQuery,
        data: Data,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponse>;

      getCount(
        params: ModelParams,
        query: ModelQuery,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<number>;

      getAll(
        filter: ModelFilter,
        pagination: ModelPagination,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponseList>;

      getAllWithConnections(
        filter: ModelFilter,
        pagination: ModelPagination,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponseList>;

      updateAll(params: ModelParams, data: Data, options: ModelOptions): Promise<boolean>;

      upsertOne(
        params: ModelParams,
        query: ModelQuery,
        data: Data,
        restrictions: ModelRestrictions,
        options: ModelOptions
      ): Promise<ModelResponse>;

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
