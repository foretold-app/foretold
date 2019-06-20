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
    user?: Models.User;
    bot?: Models.Bot;
    agent: Models.Agent;
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
    };
    type filter = {
      creatorId?: Models.ObjectID,
      seriesId?: Models.ObjectID,
      channelId?: Models.ObjectID,
      measurableId?: Models.ObjectID,
      agentId?: Models.ObjectID,
      excludeChannelId?: Models.ObjectID,
      competitorType?: string,
      findInDateRange?: object,
      notTaggedByAgent?: string,
      states?: string[],
      isArchived?: string[],
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
    type filter = {
      excludeChannelId?: Models.ObjectID,
      isArchived?: string[],
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
      ): response;

      updateOne(
        params: params,
        data: data,
        restrictions: restrictions,
      ): response;

      createOne(
        data: data,
        restrictions: restrictions,
      ): response;

      getOne(
        params: params,
        query: query,
        restrictions: restrictions,
      ): response;

      getAll(
        filter: filter,
        pagination: pagination,
        restrictions: restrictions,
      ): responseList;

      updateAll(
        params: params,
        data: data,
      ): boolean;
    }
  }
}
