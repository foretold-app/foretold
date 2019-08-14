export namespace Models {

  export type ObjectID = string;
  export type ChannelMembershipRole = "ADMIN" | "VIEWER";

  export interface Model {
    id: ObjectID;
    createdAt: string;
    updatedAt: string;

    changed(name: string): boolean;

    previous(name: string): boolean;

    get(name: string): any;
  }

  export interface Channel extends Model {
    isPublic: boolean;

    getAgents(): Models.Agent[];
  }

  export interface Bot extends Model {
    name: string;

    getAgent(): Models.Agent;
  }

  export interface Preference extends Model {
    agentId: ObjectID;
    stopAllEmails: boolean;
    enableExperimentalFeatures: boolean;
  }

  export interface User extends Model {
    name: string;
    auth0Id: string;
    agentId: ObjectID;
    email?: string;
    picture?: string;
    isEmailVerified: boolean;
    descriptions?: string;

    getAgent(): Models.Agent;
  }

  export interface Measurable extends Model {
    name: string;
    labelCustom: string;
    valueType: string;
    expectedResolutionDate: string;
    creatorId: ObjectID;
    resolutionEndpoint: string;
    stateUpdatedAt: string;
    labelSubject: string;
    labelOnDate: string;
    labelProperty: string;
    seriesId: string;
    channelId: string;
    isArchived: boolean;
    state: string;
    min: number;
    max: number;

    getCreationNotification(creator: Models.Creator): any;

    getUpdateNotifications(creator: Models.Creator): any;

    getCreator(): Models.Agent;

    getChannel(): Models.Channel;
  }

  export interface Measurement extends Model {
    value: any;
    competitorType: string;
    measurableId: ObjectID;
    agentId: ObjectID;
    relevantAt: string;
    taggedMeasurementId: ObjectID;
    description: string;
    valueText: string;

    getCreationNotification(creator: Models.Creator): any;

    getMeasurable(): Measurable;
  }

  export interface Agent extends Model {
    isAdmin: boolean;
    type: 'BOT' | 'USER';
    name: string;
    measurementCount: number;

    getBot(): Models.Bot;

    getUser(): Models.User;
  }

  export interface Series extends Model {
  }

  export interface ChannelMemberships {
    agentId: ObjectID;
    channelId: ObjectID;
    role: ChannelMembershipRole;
    methodCreatedBy:
      'ADDED_IN_APP_BY_ADMIN' |
      'AGENT_JOINED_DIRECTLY' |
      'ADDED_BY_EMAIL_BY_ADMIN';
  }

  export interface Token extends Model {
    token: string;
    isActive: boolean;
  }

  export interface AgentNotification extends Model {
  }

  export interface Template extends Model {
  }

  export interface Notification extends Model {
  }

  export interface Invitation extends Model {
    agentId: ObjectID;
    channelId: ObjectID;
    inviterAgentId: ObjectID;
    status: 'AWAITING' | 'ACCEPTED';
  }

  export interface FeedItem extends Model {
    channelId: ObjectID;
    agentId: ObjectID;
    body: {
      generic?: { item: string, description: string },
      measurable?: { item: string, description: string, measurableId: string },
    };
  }

  export interface GlobalSetting extends Model {
    entityGraph: null | object;
  }

  export type Creator = Models.User | Models.Bot;
}

export namespace Schema {
  export interface Context {
    // Authentication/authorization section
    // @todo: add prefix
    user?: Models.User;
    bot?: Models.Bot;
    agent: Models.Agent;

    // After Middleware Interceptions
    userAsObject?: Models.User;
    preference?: Models.Preference;
    creator?: Models.User | Models.Bot;
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
  namespace DataSourceLayer {
    type compoundId = object;
    type id = string | compoundId;
    type data = object;
    type options = {
      isAdmin?: boolean,
      agentId?: Models.ObjectID,
      measuredByAgentId?: Models.ObjectID,
      channelMemberId?: Models.ObjectID,
      transaction?: object,
      lock?: boolean,
      skipLocked?: boolean,
    };
    type filter = {
      id?: Models.ObjectID,
      creatorId?: Models.ObjectID,
      seriesId?: Models.ObjectID,
      channelId?: Models.ObjectID,
      measurableId?: Models.ObjectID,
      agentId?: Models.ObjectID,
      excludeChannelId?: Models.ObjectID,
      competitorType?: string,
      findInDateRange?: object,
      notTaggedByAgent?: Models.ObjectID,
      states?: string[],
      isArchived?: string[],
      types?: string[],
      type?: string,
      notificationId?: Models.ObjectID,
      channelMemberId?: Models.ObjectID,
      sentAt?: string | null,
      attemptCounterMax?: number,
    };
    type pagination = {
      limit?: number,
      offset?: number,
      last?: number,
      first?: number,
      after?: string,
      before?: string,
    };
    type query = object;
    type params = {
      id?: Models.ObjectID,
      agentId?: Models.ObjectID,
      auth0Id?: string;
      name?: string;
    };
    type response = { data: any };
    type responseList = { data: any[], total: number };

    interface DataSource {
      createOne(
        data: data,
        options: options,
      ): response;

      getOne(
        params: params,
        query: query,
        options: options,
      ): response;

      updateOne(
        params: params,
        data: data,
        options: options,
      ): response;

      deleteOne(
        params: params,
        options: options,
      ): response;

      getAll(
        filter: filter,
        pagination: pagination,
        options: options,
      ): responseList;

      upsertOne(
        params: params,
        query: query,
        data: data,
        options: options,
      ): response;
    }
  }

  namespace AbstractModelsLayer {
    type compoundId = object;
    type id = string | compoundId;

    type data = object;
    type restrictions = {
      agentId?: Models.ObjectID,
      isAdmin?: boolean,
      channelIdAsId?: boolean,
      userId?: Models.ObjectID,
      channelId?: Models.ObjectID,
      measurableId?: Models.ObjectID,
      measuredByAgentId?: Models.ObjectID,
      channelMemberId?: Models.ObjectID,
    };
    type options = {
      transaction?: object,
      lock?: boolean,
      skipLocked?: boolean,
    };
    type filter = {
      agentId?: Models.ObjectID,
      excludeChannelId?: Models.ObjectID,
      channelMemberId?: Models.ObjectID,
      userId?: Models.ObjectID,
      channelId?: Models.ObjectID,
      isArchived?: string[],
      types?: string[],
      sentAt?: string[],
      notificationId?: string[],
      attemptCounterMax?: number,
    };
    type pagination = {
      limit?: number,
      offset?: number,
      last?: number,
      first?: number,
      after?: string,
      before?: string,
    };
    type query = object;
    type params = object;
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
