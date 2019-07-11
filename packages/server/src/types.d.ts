export namespace Models {

  export type ObjectID = string;

  export interface Model {
    id: ObjectID;
    createdAt: string;
    updatedAt: string;
  }

  export interface Channel extends Model {
    id: ObjectID;
    isPublic: boolean;
  }

  export interface Bot extends Model {
    id: ObjectID;
    name: string;

    getAgent(): Models.Agent;
  }

  export interface Preference extends Model {
    id: ObjectID;
    emails: boolean;
  }

  export interface User extends Model {
    id: ObjectID;
    name: string;
    auth0Id: string;
    agentId: ObjectID;

    getAgent(): Models.Agent;
  }

  export interface Measurable extends Model {
    id: ObjectID;
    state: string;

    getCreationNotification(creator: Models.Creator): any;

    getUpdateNotifications(creator: Models.Creator): any;
  }

  export interface Measurement extends Model {
    id: ObjectID;

    getCreationNotification(creator: Models.Creator): any;
  }

  export interface Agent extends Model {
    id: ObjectID;
    isAdmin: boolean;
    type: 'BOT' | 'USER';
    name: string;
    measurementCount: number;

    getBot(): Models.Bot;

    getUser(): Models.User;
  }

  export interface Series extends Model {
    id: ObjectID;
  }

  export type ChannelMembershipRole = "ADMIN" | "VIEWER";

  export interface ChannelMemberships {
    agentId: ObjectID;
    channelId: ObjectID;
    role: ChannelMembershipRole;
  }

  export type Creator = Models.User | Models.Bot;

  export interface Token {
    token: string;
    isActive: boolean;
  }
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
      sentAt?: string | null,
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
      userId?: Models.ObjectID,
      channelId?: Models.ObjectID,
      measurableId?: Models.ObjectID,
      measuredByAgentId?: Models.ObjectID,
    };
    type options = {
      transaction?: object,
      lock?: boolean,
      skipLocked?: boolean,
    };
    type filter = {
      excludeChannelId?: Models.ObjectID,
      userId?: Models.ObjectID,
      isArchived?: string[],
      types?: string[],
      sentAt?: string[],
      notificationId?: string[],
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
