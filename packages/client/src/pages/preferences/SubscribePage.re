[@react.component]
let make = (~loggedUser: Types.user) => {
  let agentId = Primary.Agent.AgentId.make(loggedUser.agentId);
  let (mutate, result, _) = Subscribe.Mutation.use();

  <SLayout head={SLayout.Header.textDiv(Lang.subscribePageCardTitle)}>
    <FC.PageCard.BodyPadding>
      {switch (result) {
       | Error(_error) => <p> {Lang.networkError |> Utils.ste} </p>
       | Data(_) => <p> {Lang.sucSubscribing |> Utils.ste} </p>
       | Loading(_) => <Spin />
       | _ =>
         mutate(
           ~variables=Subscribe.Query.make(~agentId, ())##variables,
           ~refetchQueries=[|"user"|],
           (),
         )
         |> ignore;

         <p> {Lang.subscribing |> Utils.ste} </p>;
       }}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};