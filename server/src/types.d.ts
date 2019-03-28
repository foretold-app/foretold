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
}

export namespace Schema {
  export interface Context {
    user: Models.User;
  }
  export interface ChannelsInput {
    name: string;
    description: string;
    isArchived: boolean;
    isPublic: boolean;
  }
}
