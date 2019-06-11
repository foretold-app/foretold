export namespace Models {

  export interface Model {
    id: string;
    createdAt: string;
    updatedAt: string;
  }

  export interface Channel extends Model {
    id: string;
    isPublic: boolean;
  }

  export interface Bot extends Model {
    id: string;
    name: string;
    getAgent(): Models.Agent;
  }

  export interface User extends Model {
    id: string;
    name: string;
    auth0Id: string;
    agentId: string;
    getAgent(): Models.Agent;
  }

  export interface Measurable extends Model {
    id: string;
    state: string;
    getCreationNotification(creator: Models.Creator): any;
    getUpdateNotifications(creator: Models.Creator): any;
  }

  export interface Measurement extends Model {
    id: string;
    getCreationNotification(creator: Models.Creator): any;
  }

  export interface Agent extends Model {
    id: string;
    isAdmin: boolean;
    type: 'BOT' | 'USER';
    name: string;
    measurementCount: number;
    getBot(): Models.Bot;
    getUser(): Models.User;
  }

  export interface Series extends Model {
    id: string;
  }

  export type ChannelMembershipRole = "ADMIN" | "VIEWER";

  export interface ChannelMemberships {
    agentId: string;
    channelId: string;
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
      agentId?: string,
      measuredByAgentId?: string,
    };
    type filter = {
      creatorId?: string,
      seriesId?: string,
      channelId?: string,
      measurableId?: string,
      agentId?: string,
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
      agentId?: string,
      isAdmin?: boolean,
      userId?: string,
      channelId?: boolean,
      measurableId?: boolean,
      measuredByAgentId?: string,
    };
    type filter = object;
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
