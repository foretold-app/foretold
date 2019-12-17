# Server
This is a simple Node.js server for the foretold application. 
It uses graphql/apollo/sequelize.

# To Run
1. Change the **config/db-config.js** to match your credentials.
2. Run ``yarn install``
3. Run ``yarn db:create``
4. Run ``yarn db:migrate``
5. Run ``AUTH0_SECRET={SECRET} JWT_SECRET={SECRET} yarn dev`` 
(make sure to get the secret from Ozzie first).

# Software Architecture

### The Asynchronous Elements

| Components        |
| ----------------- |
| Node.js           |
| Express.js        |
| Appolo GraphQL    |
| - Middlewares     |
| - Authorizers     |
| - Resolvers       |
| Data Sources      |
| Abstract Models   |
| Sequelize Models  |
| PostgresQL        |

Each layer knows only interface of nested layer, not deeper then only one.
This means that a **Data Source** know interface of an **Abstract Model**, but
does not know interface of an **Sequelize Model**.

### The Synchronous Elements

| Components    | 
| ------------- |
| Actions       |
| Consumers     |
| Producers     |
| GitHub        |
| Emitter       |
| Events        |
| Jobs          |
| Listeners     |
| Mailer        |
| Schedule      |


### The Data

| Datums            | 
| ----------------- |
| Models            | 
| Enums             |
| Graphql Types     |
| Data Classes      |
| Models Classes    |
|  - Feed Items     |
|  - Notifications  |
|  - Templates      |

Todo:
- To move "models/classes" to somewhere else.
- To mote "lib/authentication" to somewhere else.

# Application Development

### Terms
- Brier Score - score of predictions.
- CDF - Cumulative distribution function.
- PDF - Probability density function.
- Server Side JWT - is used in API calls to **Identify** application objects.
- One-time Token - has an limit of life time.
- Long-term Token - has no life time limits.
- Auth0 JWT.
- Auth0 Access Token.
- Auth0.com - is an external system to identify users. 
- Competitive Measurement - "Agents" do it thought the client.
- Aggregation Measurement - result of "Bots" work.
- Unresolved Measurement.
- Graphql Terms:
  - Connection (edges, nodes).
  - Type.
  - Middleware.
  - Resolver.
  - Authorizer.

## Introduction

To understand better how the application works, which data flows are existed in
it, pay attention on this text. Please, start from **Terms**, and **Objects**.

Some solutions are made to give an ability in the future to grow the application and
prepare it to the work in multi-instance mode (DB - PostgreSQL, Data/Models Layers,
Async/Sync parts, Notifications Subsystem and so on), to be ready to scale the application
**by X, Y, Z** axes of scaling model. Where **X axis** scaling is horizontal duplication 
(scale by cloning), **Y axis** - functional decomposition (splitting different things),
**Z axis** - data partitioning (scale by splitting similar things).

Talking about Z axis. In the DB side the application uses "UUIDV4" as an ID of Objects. 
It is not
sortable since it is not a sequence, but it is usefully in splitting data by 
shards. Maybe in the future we are going to use "MongoDB Object ID" as an ID of objects
right on the PostreSQL side. But do not forget that to migrate old data we should
order it with "createdAt" field and only then to give to each object new ID (if it will
be needed).

Using UUID as an ID of the DB tables rows is a brilliant idea. Because each row
represents virtual objects. Hence each virtual object has its own an identification(ID). 
We do really like this aspect.

## Concepts

- We widely use "Types" for both sides. For the server as well as for the client
sides. See "Cursor" types for instance. Do not use ordinary "string" types,
it does not tell us anything, you wanted to say something certain. And all
the strings have a length. Do not forget about it.
- We do not use the cache. The time has not come.
- We like encapsulated flows.
- We do not deliver a business logic to the DB side. 
- We like to use abilities of full technology stack (Node.js, Postgres, React 
and so on).
- We do not like the comments in the code. Use code as comments.
- The better code is the code does not exist.
- We always remember about timezones. We store all time formats as UTC. 
  The client works with user's timezones.
- We hate a code duplication.

> **Mark Watney**: The problem is water. I have created 126 square meters of soil. 
But every cubic meter of soil requires 40 liters of water to be farmable. 
So I got to make a lot more water. Good thing is, I know the recipe. 
You take hydrogen, you add oxygen, you burn. Now, I have hundreds of liters 
of unused hydrazine at the MDV. If I run the hydrazine over an iridium 
catalyst, it’ll separate into N2 and H2. And then if I just direct the 
hydrogen into a small area and burn it. Luckily, in the history of humanity
nothing bad has ever happened from lighting hydrogen on fire. 
**NASA hates fire**. Because of the whole “fire makes everybody die in space” 
thing.

### Virtual Objects

Rules:

1. Always try to keep virtual objects as clear as you can, try to not use Links
like "agent_id", "channel_id" or so on. It will prevent you from many
future issues when you are going to make refactoring of the application.

2. To prevent hard link between "logic flows" you can add something
like "methodCreatedBy" of "ChannelMembership". So, for what do
you think I am doing this? The main reason is to separate Sync/Async flows
of the application. At the right hand you just want to store new membership
in the "ChannelMembership" table, on another hand and you do not want to hard link this 
parts of the application with async flow "when somebody is being added into a channel".
In this case just add new aspect of an object and then (later) use this
aspect in async flow, just make another system event like "New Channel 
Membership" is added. 

3. Each virtual object must have "createAt", "updateAt" timestamps. And if there is 
an ability to add this "updatedAt" too (on the next stage).
 
4. Virtual objects have the "associations". In this application they are stored in 
the "Models Definition" layer and in the DB side in the "constraints". 

- **Agent** (clear*) - is an abstraction of User, Bot objects.
- **User** (combined*) - represents a User of Application.
- **Bot** (combined*) - each User can create a Bot.
- **Channel** (combined*) - all data are separated by channels.
- **Measurable** (combined*) - **questions** about probability.
- **Measurement** (combined*) - **predictions**, results of measurements 
for questions.
- **Series** (combined*) - series of measurements.
- **Preference** (combined*) - each Agent has it own preferences 
or settings for a client.
- **Token** (combined*) - abstraction for tokens.
- **Template** (clear*) - abstraction for templates for any use-case.
- **Notification** (clear*) - part of Notification Subsystem.
- **Invitation** (combined*) - abstraction of Invitation Flow.
- **Notification Status** (link*) - links between Agents and Notifications.
- **Channel Membership** (link*) - links between Channels
and theirs Members (Agents).
- **Global Setting**.
- **Feed Item**.

Marks: 
- _clear*_ - it is an virtual object without any external links (like  "agent_id" or 
similar),
- _link*_ - an object to link another virtual objects, 
- _combined*_ - contains Link and Clear aspects of virtual objects.

### External System Integration

- Auth0.com - clients external authentication.
- Amazon AWS SES - email provider.
- Google Stack Drive - log management.

### Hooks

See: "packages/server/src/models/hooks.js" file. Try to keep all "sync" hooks 
in one place. For what do you want to ask?

The server side application architecture is being building with separated,
decomposed layers (Model Definitions, Models Abstractions, Data Layer.
Middleware Layer, Authorizers Layer, Async Flows Layer, GrqphQL
Layer ans its resolvers).

Models are just "data type definitions" which are similar with "grapqhl
types". "Sequelize" is a just lib which plays role of a "DB Driver".

So this is why we keep "async" part of the application here, and
the "associations" definitions too. It does not matter where the "associations"
are defined and placed (either DB or JS files). The main purpose
is to give to developers to read this base definitions without
duplications and keep in a mind only a small part of this.

### Plain Classes

Why do we use plain classes like "Data", "Params" and so on?

We use "Data", "Params", "Filter", "Options", "Pagination", "Query",
"Response All", "Restrictions" classes. 

- The main purpose is to protect any developer from widely counted interface errors.
- The second reason is that we want to increase readable qualities of the code.

### Logic Flows

1. Measurables States Flow. 
"Measurables" have "states". Some part of the app moves "Open" measurables
to "Judgement Pending" measurables. Another part moves from "Judgement Pending"
to "Judged" or "Agent" does it. 

2. Aggregation Measurements Flow. Bots Flow.
"Measurements" are being made by "Agents" though the client. They are called
"Competitive". If them make "Bots" these are "Aggregation" measurements. So each
time period some "Bots" make "Aggregations" of previous "Measurements".

3. Authentication Flow (to describe how the application identifies the users).
- What does "Authentication Primary" mean?
- What does "Authentication Secondary" mean? And what is the difference 
between these two entities?

4. Authorization Flow (to describe how queries go through an access control).

5. Logs Flow. This flow works with Google Stack Drive. Only used for
maintenance purpose.

6. User Info Updating Flow. This flow uses "access token" of "Auth0.com" and 
updated users info.

7. Notification Producing Flow.

8. Notification Consuming Flow.

9. Objects Co-creation Flow. 

10. Invitations Flow.

11. Mails Sender Flow.
How this works?

12. Feed Items Flow. 
How are they created?

13. Ken Library Flow. GitHub Integration Description.

14. GraphQL Errors Masking Caveats.
In production mode "Graphql Apollo Server" always masks all errors. This does
not correspond to "Graphql Apollo Server" this tied with "Graphql Protocol".

### Authentication Flow

1. Get "Server Side JWT" using (one of):
- "Auth0 JWT", "Auth0 Access Token".
- "One-time Token".
- "Long-term Token".
2. Get API responses with "Server Side JWT".
