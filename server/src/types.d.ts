export namespace Models {
  export interface User {
    id: string;
    name: string;
    auth0Id: string;
    agentId: string;
  }

  export interface Channel {
    id: string;
    isPublic: boolean;
  }

  export interface Bot {
    id: string;
  }

  export interface Measurable {
    id: string;
  }

  export interface Measurement {
    id: string;
  }

  export interface Agent {
    id: string;
  }

  export interface Series {
    id: string;
  }

  export type ChannelMembershipRole = "ADMIN" | "VIEWER";

  export interface ChannelMemberships {
    agentId: string;
    channelId: string;
    role: ChannelMembershipRole;
  }
}

export namespace Schema {
  export interface Context {
    user: Models.User;
    channel?: Models.Channel;
    channelMembership?: Models.ChannelMemberships;
    channelMembershipsRole?: Models.ChannelMembershipRole;
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
    type restrictions = object;
    type filter = object;
    type pagination = object;
    type query = object;
    type params = object;

    interface DataSource {
      createOne(
        data,
        restrictions,
      ): any;

      getOne(
        params,
        query,
        restrictions,
      ): any;

      updateOne(
        params,
        data,
        restrictions,
      ): any;

      deleteOne(
        params,
        restrictions,
      ): any | null;

      getAll(
        filter,
        pagination,
        restrictions,
      ): any[];
    }
  }

  namespace AbstractModelsLayer {
    type compoundId = object;
    type id = string | compoundId;

    type data = object;
    type restrictions = object;
    type filter = object;
    type pagination = object;
    type query = object;
    type params = object;

    interface AbstractModel {
      deleteOne(
        params: params,
        restrictions: restrictions,
      ): any;

      updateOne(
        params: params,
        data: data,
        restrictions: restrictions,
      ): any;

      createOne(
        data: data,
        restrictions: restrictions,
      ): any;

      getOne(
        params: params,
        query: query,
        restrictions: restrictions,
      ): any;

      getAll(
        filter: filter,
        pagination: pagination,
        restrictions: restrictions,
      ): any[];
    }
  }
}
