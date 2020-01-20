[@react.component]
let make = (~loggedUser: Types.user) => {
  let agentId = Primary.Agent.AgentId.make(loggedUser.agentId);
  let (mutate, result, _) = Subscribe.Mutation.use();

  <SLayout head={<SLayout.TextDiv text=Lang.subscribePageCardTitle />}>
    <ForetoldComponents.PageCard.BodyPadding>
      {switch (result) {
       | Error(_error) => <Sorry />
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
    </ForetoldComponents.PageCard.BodyPadding>
  </SLayout>;
};