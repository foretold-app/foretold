Ideas of things to do.

### Hide user/bot abstraction in API.
We have two agent types in the database, because both need different fields. But we generally just use the agentId on all other attributes. I think it could be cleaner if we cleaned this up in the graphQL layer, and didn't make the user or bot IDs accessible at that level, so they are completely hidden by the backend.

### Wrap all console logging coloring into a library. 
```js
 console.log(`\x1b[33m Rule (isChannelPublic) ` +
    `channelId "${_.get(context, 'channel.id')}", ` +
    `isChannelPublic "${JSON.stringify(isChannelPublic)}"` +
    ` \x1b[0m`);
```
