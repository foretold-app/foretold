[@react.component]
let make = (~loggedUser: Types.user) => {
  let agentId = Primary.Agent.AgentId.make(loggedUser.agentId);
  let (mutate, result, _) = Unsubscribe.Mutation.use();

  <SLayout head={<SLayout.TextDiv text={Lang.unsubscribePageCardTitle} />}>
    <FC.PageCard.BodyPadding>
      {switch (result) {
       | Error(_error) => <Sorry />
       | Data(_) => <p> {Lang.sucUnsubscribing |> Utils.ste} </p>
       | Loading(_) => <Spin />
       | _ =>
         mutate(
           ~variables=Unsubscribe.Query.make(~agentId, ())##variables,
           ~refetchQueries=[|"user"|],
           (),
         )
         |> ignore;

         <p> {Lang.unsubscribing |> Utils.ste} </p>;
       }}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};