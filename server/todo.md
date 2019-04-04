# Ideas of things to do.

### Hide user/bot abstraction in API.
We have two agent types in the database, because both need different fields. But we generally just use the agentId on all other attributes. I think it could be cleaner if we cleaned this up in the graphQL layer, and didn't make the user or bot IDs accessible at that level, so they are completely hidden by the backend. I imagine that a sum type would be quite nice, but have heard that those can be tricky with GraphQL, so may not be the best.

### Wrap all console logging coloring into a library. 
```js
 console.log(`\x1b[33m Rule (isChannelPublic) ` +
    `channelId "${_.get(context, 'channel.id')}", ` +
    `isChannelPublic "${JSON.stringify(isChannelPublic)}"` +
    ` \x1b[0m`);
```

### Handle "Value" in graphQL Layer.
Right now the measurement ``values`` are Json objects that get saved straight to the database. This is kinda messy. I think it would probably be better if they could be fully represented in the GraphQL layer. Happy to explain more.

### Mutations have corresponding inputs.
Right now the ``channelUpdate`` mutation accepts a ``ChannelInput``, but most other mutations don't use an input, but instead a bunch of parameters. These should be modified to all use similar inputs. This is true for:
- measurementCreate
- measurableCreate
- seriesCreate
- measurableUpdate
- channelMembershipCreate
- channelMembershipRoleUpdate
- channelMembershipDelete

### Channel Permissions
- All channels need at least one admin. The admin should not be able to remove themselves, unless there is one other admin.
- Change permissions:
  - Authenticated users shouldn't be able to modify channel members.
  - Authenticated users shouldn't be able to edit public channels.
  - Authenticated users shouldn't be able to change roles.

## GraphQL Schema Changes
- User auth0Ids should only be accessible by the logged in user. 
- Users / Bots should only be visible as properties of Agents in the graphQL layer. Perhaps an ``agent`` would have ``userAttributes`` or similar, but the userId would be hidden. Names and descriptions should only exist on agents, as they are redundant to the other two.
- Measurable should have a subtype for ``MeasurableLabel`` or similar, which wraps all the fields currently with ``description`` in their names.
- Removed TaggedBy attributes for measurements, for now.
- Measurement Value shouldn't be a JSON value in graphQL, but instead broken into all subparts.
- We should have different loggedIn attributes:
  - Measurements, measurables, and series should have an ``iAmOwner`` graphql attribute, which would be ``true`` if the viewing user is the owner of them.
  - Channels should have a ``myRole`` attribute, which would provide the role of the current user in that channel. ``None`` should be one option.
  - There should be a ``channelJoin`` and ``channelLeave`` mutation that effect the current user.
  - ``userUpdate`` should be changed to ``updateMe`` or similar, which could only be used on the current user (note: this should probably apply to the agent, not the being)
  - There should be an ``isMe`` attribute on ``Agent``.
  
## Direct API strategy
We need some planning of how humans would interact with the system via API. In the beginning, I imagine this would look something like giving out tokens to each person to user for authentication. Later, they should probably create bots to do this. Later, they should have restrictions for access.

## Series Functionality
Series can get pretty complicated, but are quite important. Will think about.

